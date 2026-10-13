
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

// EMA aplicado somente ao alerta
#define ALPHA 0.20

// Estabilizacao da exibicao
#define ZONA_ESTABILIDADE 0.5

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
// RTC
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
// TEMPO
// =====================================================

unsigned long ultimaMedicao = 0;

// =====================================================
// VARIAVEIS DOS FILTROS
// =====================================================

float velocidadeEMA = 0.0;
float velocidadeExibida = 0.0;

bool primeiraLeituraEMA = true;
bool primeiraLeituraDisplay = true;

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
// ESTABILIZACAO DO VALOR NO OLED
// =====================================================

float estabilizarDisplay(float velocidadeAtual) {

  if (primeiraLeituraDisplay) {
    velocidadeExibida = velocidadeAtual;
    primeiraLeituraDisplay = false;

    return velocidadeExibida;
  }

  // Mantem o valor anterior se a diferenca for pequena
  if (abs(velocidadeAtual - velocidadeExibida)
      <= ZONA_ESTABILIDADE) {
    return velocidadeExibida;
  }

  velocidadeExibida = velocidadeAtual;

  return velocidadeExibida;
}

// =====================================================
// CONTROLE DO LED E BUZZER
// =====================================================

void controlarAlerta(float velocidade) {

  if (velocidade >= VELOCIDADE_LIMITE) {
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

  // Inicializa o OLED
  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C
      )) {

    Serial.println("ERRO: OLED nao encontrado!");

    while (true) {
      // Para a execucao se o display nao for encontrado
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

  // Saidas
  pinMode(PINO_LED, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  digitalWrite(PINO_LED, LOW);
  digitalWrite(PINO_BUZZER, LOW);

  // Entrada do anemometro
  pinMode(PINO_ANEMOMETRO, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(PINO_ANEMOMETRO),
    contarPulso,
    FALLING
  );

  // Inicializa a sincronizacao do RTC
  setSyncProvider(getRTC);

  if (timeStatus() == timeSet) {
    Serial.println("RTC sincronizado.");
  } else {
    Serial.println("RTC nao sincronizado; usando millis().");
  }

  // Inicia a medicao independente do RTC
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

    // Reinicia a janela de medicao a partir do instante atual
    ultimaMedicao = tempoAtual;

    // Copia e zera os pulsos com seguranca
    noInterrupts();
    unsigned long pulsos = contadorPulsos;
    contadorPulsos = 0;
    interrupts();

    // Velocidade calculada pela calibracao original
    float velocidadeAtual = converterVelocidade(pulsos);

    // Estabiliza apenas a indicacao do OLED
    float velocidadeTela =
      estabilizarDisplay(velocidadeAtual);

    // Filtra separadamente o alerta
    float velocidadeAlerta =
      aplicarEMA(velocidadeAtual);

    controlarAlerta(velocidadeAlerta);

    // Atualiza o OLED
    atualizarOLED(velocidadeTela);

    // Monitor Serial
    Serial.print("Pulsos: ");
    Serial.print(pulsos);

    Serial.print(" | Velocidade medida: ");
    Serial.print(velocidadeAtual, 1);
    Serial.print(" km/h");

    Serial.print(" | Velocidade OLED: ");
    Serial.print(velocidadeTela, 1);
    Serial.print(" km/h");

    Serial.print(" | EMA alerta: ");
    Serial.print(velocidadeAlerta, 1);
    Serial.println(" km/h");
  }
}
