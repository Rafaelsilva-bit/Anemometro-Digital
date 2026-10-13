
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DS3232RTC.h>
#include <TimeLib.h>

// =====================================================
// CONFIGURACOES GERAIS
// =====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

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

// Filtro adaptativo do OLED
#define ALPHA_OLED 0.35
#define LIMITE_MUDANCA_RAPIDA 2.0
#define ZONA_ESTABILIDADE 0.25

// =====================================================
// OLED
// =====================================================

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

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
float velocidadeOLED = 0.0;

bool primeiraLeituraEMA = true;
bool primeiroValorOLED = true;

// =====================================================
// INTERRUPCAO DO ANEMOMETRO
// =====================================================

void contarPulso() {
  contadorPulsos++;
}

// =====================================================
// CONVERSAO DOS PULSOS PARA KM/H
// CALIBRACAO ORIGINAL PRESERVADA
// =====================================================

float converterVelocidade(unsigned long pulsos) {

  if (pulsos == 0) {
    return 0.0;
  }

  if (pulsos <= 5) {
    return 5.0;
  }

  if (pulsos <= 9) {
    return 5.0 +
           (pulsos - 5.5) * 5.0 / 4.0;
  }

  if (pulsos <= 13) {
    return 10.0 +
           (pulsos - 9.5) * 5.0 / 4.0;
  }

  if (pulsos <= 17) {
    return 15.0 +
           (pulsos - 13.5) * 5.0 / 4.0;
  }

  if (pulsos <= 21) {
    return 20.0 +
           (pulsos - 17.5) * 5.0 / 4.0;
  }

  if (pulsos <= 25) {
    return 25.0 +
           (pulsos - 21.5) * 5.0 / 4.0;
  }

  if (pulsos <= 30) {
    return 30.0 +
           (pulsos - 25.5) * 5.0 / 5.0;
  }

  if (pulsos <= 34) {
    return 35.0 +
           (pulsos - 30.5) * 5.0 / 4.0;
  }

  if (pulsos <= 38) {
    return 40.0 +
           (pulsos - 34.5) * 5.0 / 4.0;
  }

  if (pulsos <= 42) {
    return 45.0 +
           (pulsos - 38.5) * 5.0 / 4.0;
  }

  if (pulsos <= 46) {
    return 50.0 +
           (pulsos - 42.5) * 5.0 / 4.0;
  }

  if (pulsos <= 50) {
    return 55.0 +
           (pulsos - 46.5) * 5.0 / 4.0;
  }

  return 60.0 +
         (pulsos - 50.5) * 5.0 / 4.0;
}

// =====================================================
// FILTRO ADAPTATIVO PARA O OLED
// =====================================================

float filtrarVelocidadeOLED(float velocidadeAtual) {

  // Primeira leitura: inicializa sem atraso
  if (primeiroValorOLED) {
    velocidadeOLED = velocidadeAtual;
    primeiroValorOLED = false;

    return velocidadeOLED;
  }

  float diferenca = velocidadeAtual - velocidadeOLED;

  // Mudanca significativa: acompanha rapidamente
  if (abs(diferenca) >= LIMITE_MUDANCA_RAPIDA) {
    velocidadeOLED = velocidadeAtual;

    return velocidadeOLED;
  }

  // Suaviza variacoes pequenas
  float novaVelocidade =
    ALPHA_OLED * velocidadeAtual +
    (1.0 - ALPHA_OLED) * velocidadeOLED;

  // Ignora pequenas oscilacoes na indicacao
  if (abs(novaVelocidade - velocidadeOLED)
      >= ZONA_ESTABILIDADE) {

    velocidadeOLED = novaVelocidade;
  }

  return velocidadeOLED;
}

// =====================================================
// FILTRO EMA PARA O ALERTA
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
// CONTROLE DO LED E BUZZER
// =====================================================

void controlarAlerta(
  float velocidadeAtual,
  float velocidadeFiltrada
) {

  // Liga imediatamente ao atingir o limite.
  // Mantem ligado enquanto o EMA continuar no limite.
  if (
    velocidadeAtual >= VELOCIDADE_LIMITE ||
    velocidadeFiltrada >= VELOCIDADE_LIMITE
  ) {
    digitalWrite(PINO_LED, HIGH);
    digitalWrite(PINO_BUZZER, HIGH);
  } else {
    digitalWrite(PINO_LED, LOW);
    digitalWrite(PINO_BUZZER, LOW);
  }
}

// =====================================================
// ATUALIZACAO DO OLED
// =====================================================

void atualizarOLED(float velocidade) {

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(4);
  display.setCursor(0, 15);
  display.print(velocidade, 1);

  display.setTextSize(1);
  display.setCursor(100, 32);
  display.print("km/h");

  display.display();
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);
  Wire.begin();

  // Inicializacao do OLED
  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C
      )) {

    Serial.println("ERRO: OLED nao encontrado!");

    while (true) {
      // Para a execucao se o OLED nao for encontrado
    }
  }

  // Tela inicial
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(34, 15);
  display.println("TIMER");

  display.setTextSize(1);
  display.setCursor(34, 40);
  display.println("INICIANDO...");

  display.display();

  delay(1000);

  // LED e buzzer
  pinMode(PINO_LED, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  digitalWrite(PINO_LED, LOW);
  digitalWrite(PINO_BUZZER, LOW);

  // Anemometro
  pinMode(PINO_ANEMOMETRO, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(PINO_ANEMOMETRO),
    contarPulso,
    FALLING
  );

  // RTC: sincronizacao opcional para o restante do projeto
  setSyncProvider(getRTC);

  if (timeStatus() == timeSet) {
    Serial.println("RTC sincronizado.");
  } else {
    Serial.println("RTC nao sincronizado; usando millis().");
  }

  // Inicio da medicao
  ultimaMedicao = millis();

  atualizarOLED(0.0);

  Serial.println("Anemometro iniciado.");
}

// =====================================================
// LOOP PRINCIPAL
// =====================================================

void loop() {

  unsigned long tempoAtual = millis();

  if (tempoAtual - ultimaMedicao >= INTERVALO_MEDICAO) {

    // Inicia uma nova janela de medicao
    ultimaMedicao = tempoAtual;

    // Copia e zera o contador de pulsos
    noInterrupts();
    unsigned long pulsos = contadorPulsos;
    contadorPulsos = 0;
    interrupts();

    // Velocidade calculada pela calibracao original
    float velocidadeAtual = converterVelocidade(pulsos);

    // OLED: resposta rapida com suavizacao adaptativa
    float velocidadeTela =
      filtrarVelocidadeOLED(velocidadeAtual);

    // EMA independente para o alerta
    float velocidadeAlerta =
      aplicarEMA(velocidadeAtual);

    // Controle do LED e buzzer
    controlarAlerta(
      velocidadeAtual,
      velocidadeAlerta
    );

    // Atualiza a tela
    atualizarOLED(velocidadeTela);

    // Diagnostico
    Serial.print("Pulsos: ");
    Serial.print(pulsos);

    Serial.print(" | Medida: ");
    Serial.print(velocidadeAtual, 1);

    Serial.print(" km/h | OLED: ");
    Serial.print(velocidadeTela, 1);

    Serial.print(" km/h | EMA alerta: ");
    Serial.print(velocidadeAlerta, 1);

    Serial.println(" km/h");
  }
}
