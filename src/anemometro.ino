#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS3232RTC.h>
#include <TimeLib.h>

// =====================================================
// CONFIGURACOES GERAIS
// =====================================================

#define PINO_ANEMOMETRO 2
#define PINO_LED 13
#define PINO_BUZZER 12

#define VELOCIDADE_LIMITE 40.0
#define INTERVALO_MEDICAO 1000UL

// =====================================================
// FILTROS
// =====================================================

// EMA para o alerta
#define ALPHA 0.20

// Filtro adaptativo para o LCD
#define ALPHA_LCD 0.35
#define LIMITE_MUDANCA_RAPIDA 2.0
#define ZONA_ESTABILIDADE 0.25

// =====================================================
// LCD 20x4 I2C
// =====================================================

LiquidCrystal_I2C lcd(0x20, 20, 4);

// =====================================================
// RTC DS3232 / DS3231 COMPATIVEL
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

// =====================================================
// VARIAVEIS DOS FILTROS
// =====================================================

float velocidadeEMA = 0.0;
float velocidadeLCD = 0.0;

bool primeiraLeituraEMA = true;
bool primeiroValorLCD = true;

// Controle do alerta intermitente
unsigned long ultimoPisca = 0;
bool estadoAlerta = false;

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

  if (abs(diferenca) >= LIMITE_MUDANCA_RAPIDA) {
    velocidadeLCD = velocidadeAtual;
    return velocidadeLCD;
  }

  float novaVelocidade =
    ALPHA_LCD * velocidadeAtual +
    (1.0 - ALPHA_LCD) * velocidadeLCD;

  if (abs(novaVelocidade - velocidadeLCD)
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

  float velocidadeReferencia =
    max(velocidadeAtual, velocidadeFiltrada);

  if (velocidadeReferencia >= 45.0) {

    digitalWrite(PINO_LED, HIGH);
    digitalWrite(PINO_BUZZER, HIGH);
    estadoAlerta = true;
  }

  else if (velocidadeReferencia >= 40.0) {

    if (millis() - ultimoPisca >= 500) {

      ultimoPisca = millis();
      estadoAlerta = !estadoAlerta;

      digitalWrite(PINO_LED, estadoAlerta);
      digitalWrite(PINO_BUZZER, estadoAlerta);
    }
  }

  else {

    digitalWrite(PINO_LED, LOW);
    digitalWrite(PINO_BUZZER, LOW);
    estadoAlerta = false;
  }
}

// =====================================================
// ATUALIZACAO LCD
// =====================================================

void atualizarLCD(float velocidade) {

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("ANEMOMETRO DIGITAL");

  lcd.setCursor(0,1);
  lcd.print("Vel: ");
  lcd.print(velocidade,1);
  lcd.print(" km/h");

  lcd.setCursor(0,2);

  if (velocidade >= VELOCIDADE_LIMITE)
    lcd.print("STATUS: ALERTA");
  else
    lcd.print("STATUS: NORMAL");

  lcd.setCursor(0,3);
  lcd.print("Limite: 40 km/h");
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);
  Wire.begin();

  lcd.init();
  lcd.backlight();

  lcd.clear();

  lcd.setCursor(3,0);
  lcd.print("ANEMOMETRO");

  lcd.setCursor(5,1);
  lcd.print("DIGITAL");

  lcd.setCursor(3,2);
  lcd.print("INICIANDO...");

  delay(1500);

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

    noInterrupts();

    unsigned long pulsos = contadorPulsos;
    contadorPulsos = 0;

    interrupts();

    float velocidadeAtual =
      converterVelocidade(pulsos);

    float velocidadeTela =
      filtrarVelocidadeLCD(velocidadeAtual);

    float velocidadeAlerta =
      aplicarEMA(velocidadeAtual);

    controlarAlerta(
      velocidadeAtual,
      velocidadeAlerta
    );

    atualizarLCD(velocidadeTela);

    Serial.print("Pulsos: ");
    Serial.print(pulsos);

    Serial.print(" | Medida: ");
    Serial.print(velocidadeAtual, 1);

    Serial.print(" km/h | LCD: ");
    Serial.print(velocidadeTela, 1);

    Serial.print(" km/h | EMA alerta: ");
    Serial.print(velocidadeAlerta, 1);

    Serial.println(" km/h");
  }
}
