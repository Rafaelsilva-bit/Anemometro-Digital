#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3232RTC.h>
#include <TimeLib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// =====================================================
// CONFIGURACOES GERAIS
// =====================================================

#define PINO_ANEMOMETRO 2
#define PINO_LED 13
#define PINO_BUZZER 12

#define VELOCIDADE_LIMITE 40.0
#define VELOCIDADE_ALERTA_CONTINUO 45.0

#define INTERVALO_MEDICAO 1000UL
#define INTERVALO_PISCA 500UL

// =====================================================
// FILTROS
// =====================================================

#define ALPHA 0.20
#define ALPHA_LCD 0.35
#define LIMITE_MUDANCA_RAPIDA 2.0
#define ZONA_ESTABILIDADE 0.25

// =====================================================
// LCD 2004B COM PCF8574
// =====================================================

LiquidCrystal_I2C lcd(0x20, 20, 4);

char ultimaLinhaLCD[4][21] = {};

// =====================================================
// RTC DS3232 / DS3231
// =====================================================

DS3232RTC rtc;

time_t getRTC() {
  return rtc.get();
}

// =====================================================
// CONTADOR DE PULSOS
// =====================================================

volatile unsigned long contadorPulsos = 0;

// =====================================================
// CONTROLE DE TEMPO
// =====================================================

unsigned long ultimaMedicao = 0;
unsigned long ultimoPisca = 0;

// =====================================================
// VARIAVEIS DOS FILTROS
// =====================================================

float velocidadeEMA = 0.0;
float velocidadeLCD = 0.0;

bool primeiraLeituraEMA = true;
bool primeiroValorLCD = true;

// =====================================================
// ESTADO DO ALERTA
// =====================================================

bool estadoAlerta = false;
bool alertaIntermitenteAtivo = false;

// =====================================================
// INTERRUPCAO DO ANEMOMETRO
// =====================================================

void contarPulso() {
  contadorPulsos++;
}

// =====================================================
// CONVERSAO DOS PULSOS PARA KM/H
// =====================================================

float converterVelocidade(unsigned long pulsos) {

  if (pulsos == 0) return 0.0;

  if (pulsos <= 5)
    return 5.0;

  if (pulsos <= 9)
    return 5.0 + (pulsos - 5.5) * 5.0 / 4.0;

  if (pulsos <= 13)
    return 10.0 + (pulsos - 9.5) * 5.0 / 4.0;

  if (pulsos <= 17)
    return 15.0 + (pulsos - 13.5) * 5.0 / 4.0;

  if (pulsos <= 21)
    return 20.0 + (pulsos - 17.5) * 5.0 / 4.0;

  if (pulsos <= 25)
    return 25.0 + (pulsos - 21.5) * 5.0 / 4.0;

  if (pulsos <= 30)
    return 30.0 + (pulsos - 25.5) * 5.0 / 5.0;

  if (pulsos <= 34)
    return 35.0 + (pulsos - 30.5) * 5.0 / 4.0;

  if (pulsos <= 38)
    return 40.0 + (pulsos - 34.5) * 5.0 / 4.0;

  if (pulsos <= 42)
    return 45.0 + (pulsos - 38.5) * 5.0 / 4.0;

  if (pulsos <= 46)
    return 50.0 + (pulsos - 42.5) * 5.0 / 4.0;

  if (pulsos <= 50)
    return 55.0 + (pulsos - 46.5) * 5.0 / 4.0;

  return 60.0 + (pulsos - 50.5) * 5.0 / 4.0;
}

// =====================================================
// FILTRO ADAPTATIVO DO LCD
// =====================================================

float filtrarVelocidadeLCD(float velocidadeAtual) {

  if (primeiroValorLCD) {
    velocidadeLCD = velocidadeAtual;
    primeiroValorLCD = false;
    return velocidadeLCD;
  }

  float diferenca = velocidadeAtual - velocidadeLCD;

  // Acompanha rapidamente mudancas maiores
  if (fabs(diferenca) >= LIMITE_MUDANCA_RAPIDA) {
    velocidadeLCD = velocidadeAtual;
    return velocidadeLCD;
  }

  // Suaviza pequenas variacoes
  float novaVelocidade =
    ALPHA_LCD * velocidadeAtual +
    (1.0 - ALPHA_LCD) * velocidadeLCD;

  // Ignora oscilacoes muito pequenas
  if (fabs(novaVelocidade - velocidadeLCD)
      >= ZONA_ESTABILIDADE) {
    velocidadeLCD = novaVelocidade;
  }

  return velocidadeLCD;
}

// =====================================================
// FILTRO EMA PARA ALERTA
// =====================================================

float aplicarEMA(float leituraAtual) {

  if (primeiraLeituraEMA) {
    velocidadeEMA = leituraAtual;
    primeiraLeituraEMA = false;
    return velocidadeEMA;
  }

  velocidadeEMA =
    ALPHA * leituraAtual +
    (1.0 - ALPHA) * velocidadeEMA;

  return velocidadeEMA;
}

// =====================================================
// CONTROLE LED E BUZZER
// =====================================================

void controlarAlerta(
  float velocidadeAtual,
  float velocidadeFiltrada
) {

  // Usa a maior leitura para evitar atrasar o alerta
  float velocidadeReferencia =
    (velocidadeAtual > velocidadeFiltrada)
    ? velocidadeAtual
    : velocidadeFiltrada;

  unsigned long agora = millis();

  // ABAIXO DE 40 KM/H: DESLIGADO
  if (velocidadeReferencia < VELOCIDADE_LIMITE) {

    digitalWrite(PINO_LED, LOW);
    digitalWrite(PINO_BUZZER, LOW);

    estadoAlerta = false;
    alertaIntermitenteAtivo = false;

    return;
  }

  // A PARTIR DE 45 KM/H: LIGADO CONTINUAMENTE
  if (velocidadeReferencia >= VELOCIDADE_ALERTA_CONTINUO) {

    digitalWrite(PINO_LED, HIGH);
    digitalWrite(PINO_BUZZER, HIGH);

    estadoAlerta = true;
    alertaIntermitenteAtivo = false;

    return;
  }

  // ENTRE 40 E 44,9 KM/H: ALERTA INTERMITENTE
  if (!alertaIntermitenteAtivo) {

    alertaIntermitenteAtivo = true;
    estadoAlerta = true;
    ultimoPisca = agora;

    digitalWrite(PINO_LED, HIGH);
    digitalWrite(PINO_BUZZER, HIGH);

    return;
  }

  // Alterna LED e buzzer a cada 500 ms
  if (agora - ultimoPisca >= INTERVALO_PISCA) {

    ultimoPisca = agora;
    estadoAlerta = !estadoAlerta;

    digitalWrite(PINO_LED, estadoAlerta ? HIGH : LOW);
    digitalWrite(PINO_BUZZER, estadoAlerta ? HIGH : LOW);
  }
}

// =====================================================
// ATUALIZA UMA LINHA SEM PISCAR O LCD
// =====================================================

void atualizarLinhaLCD(byte linha, const char* texto) {

  if (linha >= 4) return;

  char buffer[21];

  memset(buffer, ' ', 20);
  buffer[20] = '\0';

  strncpy(buffer, texto, 20);

  if (strncmp(buffer, ultimaLinhaLCD[linha], 20) != 0) {

    lcd.setCursor(0, linha);
    lcd.print(buffer);

    memcpy(ultimaLinhaLCD[linha], buffer, 21);
  }
}

// =====================================================
// ATUALIZACAO DO LCD COM STATUS AUTOMATICO
// =====================================================

void atualizarLCD(float velocidade) {

  char linhaVelocidade[21];
  char valorVelocidade[10];

  // Linha 1: nome do sistema
  atualizarLinhaLCD(0, "ANEMOMETRO DIGITAL");

  // Converte o numero decimal para texto
  dtostrf(velocidade, 5, 1, valorVelocidade);

  // Monta a linha da velocidade
  snprintf(
    linhaVelocidade,
    sizeof(linhaVelocidade),
    "Vento: %s km/h",
    valorVelocidade
  );

  // Linha 2: velocidade medida
  atualizarLinhaLCD(1, linhaVelocidade);

  // Linha 3: status automatico
  if (velocidade >= VELOCIDADE_ALERTA_CONTINUO) {

    atualizarLinhaLCD(2, "STATUS: PERIGO");

  } else if (velocidade >= VELOCIDADE_LIMITE) {

    atualizarLinhaLCD(2, "STATUS: ALERTA");

  } else if (velocidade >= 30.0) {

    atualizarLinhaLCD(2, "STATUS: FORTE");

  } else if (velocidade >= 20.0) {

    atualizarLinhaLCD(2, "STATUS: ATENCAO");

  } else {

    atualizarLinhaLCD(2, "STATUS: NORMAL");
  }

  // Linha 4: limite configurado
  atualizarLinhaLCD(3, "Limite: 40 km/h");
}

// =====================================================
// TELA DE INICIALIZACAO
// =====================================================

void mostrarTelaInicial() {

  lcd.clear();

  memset(ultimaLinhaLCD, 0, sizeof(ultimaLinhaLCD));

  lcd.setCursor(3, 0);
  lcd.print("ANEMOMETRO");

  lcd.setCursor(4, 1);
  lcd.print("DIGITAL");

  lcd.setCursor(3, 2);
  lcd.print("INICIANDO...");

  delay(1500);

  lcd.clear();

  memset(ultimaLinhaLCD, 0, sizeof(ultimaLinhaLCD));
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);
  Wire.begin();

  lcd.init();
  lcd.backlight();

  mostrarTelaInicial();

  pinMode(PINO_LED, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  digitalWrite(PINO_LED, LOW);
  digitalWrite(PINO_BUZZER, LOW);

  pinMode(PINO_ANEMOMETRO, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(PINO_ANEMOMETRO),
    contarPulso,
    FALLING
  );

  // Sincronizacao do RTC
  setSyncProvider(getRTC);

  if (timeStatus() == timeSet)
    Serial.println("RTC sincronizado.");
  else
    Serial.println("RTC nao sincronizado; usando millis().");

  ultimaMedicao = millis();

  atualizarLCD(0.0);

  Serial.println("Anemometro iniciado.");
}

// =====================================================
// LOOP PRINCIPAL
// =====================================================

void loop() {

  unsigned long tempoAtual = millis();

  if (tempoAtual - ultimaMedicao >= INTERVALO_MEDICAO) {

    ultimaMedicao = tempoAtual;

    // Copia e zera o contador com seguranca
    noInterrupts();

    unsigned long pulsos = contadorPulsos;
    contadorPulsos = 0;

    interrupts();

    // Calcula a velocidade pela calibracao original
    float velocidadeAtual = converterVelocidade(pulsos);

    // Filtra a velocidade exibida
    float velocidadeTela =
      filtrarVelocidadeLCD(velocidadeAtual);

    // Filtra a velocidade usada pelo alerta
    float velocidadeAlerta =
      aplicarEMA(velocidadeAtual);

    // Controla LED e buzzer
    controlarAlerta(
      velocidadeAtual,
      velocidadeAlerta
    );

    // Atualiza velocidade e status do LCD
    atualizarLCD(velocidadeTela);

    // Diagnostico no monitor serial
    Serial.print("Pulsos: ");
    Serial.print(pulsos);

    Serial.print(" | Medida: ");
    Serial.print(velocidadeAtual, 1);

    Serial.print(" km/h | LCD: ");
    Serial.print(velocidadeTela, 1);

    Serial.print(" km/h | EMA: ");
    Serial.print(velocidadeAlerta, 1);

    Serial.println(" km/h");
  }
}