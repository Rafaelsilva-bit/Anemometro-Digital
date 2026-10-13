
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

// Atualizacao da velocidade a cada 1000 ms
#define INTERVALO_MEDICAO 1000UL

// =====================================================
// FILTRO EMA - UTILIZADO SOMENTE NO ALERTA
// =====================================================

#define ALPHA 0.20

// =====================================================
// DISPLAY OLED
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
// CONTROLE DO TEMPO COM MILLIS()
// =====================================================

unsigned long ultimaMedicao = 0;

// =====================================================
// VARIAVEIS DO FILTRO EMA
// =====================================================

float velocidadeEMA = 0.0;
bool primeiraLeitura = true;

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
           (pulsos - 5.5) *
           (10.0 - 5.0) /
           (9.5 - 5.5);
  }

  if (pulsos <= 13) {
    return 10.0 +
           (pulsos - 9.5) *
           (15.0 - 10.0) /
           (13.5 - 9.5);
  }

  if (pulsos <= 17) {
    return 15.0 +
           (pulsos - 13.5) *
           (20.0 - 15.0) /
           (17.5 - 13.5);
  }

  if (pulsos <= 21) {
    return 20.0 +
           (pulsos - 17.5) *
           (25.0 - 20.0) /
           (21.5 - 17.5);
  }

  if (pulsos <= 25) {
    return 25.0 +
           (pulsos - 21.5) *
           (30.0 - 25.0) /
           (25.5 - 21.5);
  }

  if (pulsos <= 30) {
    return 30.0 +
           (pulsos - 25.5) *
           (35.0 - 30.0) /
           (30.5 - 25.5);
  }

  if (pulsos <= 34) {
    return 35.0 +
           (pulsos - 30.5) *
           (40.0 - 35.0) /
           (34.5 - 30.5);
  }

  if (pulsos <= 38) {
    return 40.0 +
           (pulsos - 34.5) *
           (45.0 - 40.0) /
           (38.5 - 34.5);
  }

  if (pulsos <= 42) {
    return 45.0 +
           (pulsos - 38.5) *
           (50.0 - 45.0) /
           (42.5 - 38.5);
  }

  if (pulsos <= 46) {
    return 50.0 +
           (pulsos - 42.5) *
           (55.0 - 50.0) /
           (46.5 - 42.5);
  }

  if (pulsos <= 50) {
    return 55.0 +
           (pulsos - 46.5) *
           (60.0 - 55.0) /
           (50.5 - 46.5);
  }

  return 60.0 +
         (pulsos - 50.5) *
         (60.0 - 55.0) /
         (50.5 - 46.5);
}

// =====================================================
// FILTRO EMA PARA O ALERTA
// =====================================================

float aplicarEMA(float leituraAtual) {

  if (primeiraLeitura) {
    velocidadeEMA = leituraAtual;
    primeiraLeitura = false;

    return velocidadeEMA;
  }

  velocidadeEMA =
    (ALPHA * leituraAtual) +
    ((1.0 - ALPHA) * velocidadeEMA);

  return velocidadeEMA;
}

// =====================================================
// CONTROLE DO LED E DO BUZZER
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

  // Velocidade
  display.setTextSize(4);
  display.setCursor(0, 15);
  display.print(velocidade, 1);

  // Unidade
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

  // Comunicacao I2C
  Wire.begin();

  // Inicializacao do OLED
  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C
      )) {

    Serial.println("ERRO: OLED nao encontrado!");

    while (true) {
      // Interrompe a execucao se o OLED falhar
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

  // Configuracao do LED
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW);

  // Configuracao do buzzer
  pinMode(PINO_BUZZER, OUTPUT);
  digitalWrite(PINO_BUZZER, LOW);

  // Configuracao do anemometro
  pinMode(PINO_ANEMOMETRO, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(PINO_ANEMOMETRO),
    contarPulso,
    FALLING
  );

  // Inicializacao da sincronizacao com o RTC.
  // O programa nao depende do RTC para medir o intervalo.
  setSyncProvider(getRTC);

  if (timeStatus() == timeSet) {
    Serial.println("RTC sincronizado.");
  } else {
    Serial.println(
      "Aviso: RTC nao sincronizado. Medicao por millis()."
    );
  }

  // Inicio do intervalo de medicao
  ultimaMedicao = millis();

  // Exibe zero ate a primeira medicao
  atualizarOLED(0.0);

  Serial.println("Anemometro iniciado.");
}

// =====================================================
// LOOP PRINCIPAL
// =====================================================

void loop() {

  unsigned long tempoAtual = millis();

  // Executa uma medicao a cada segundo,
  // sem depender do horario do RTC.
  if (tempoAtual - ultimaMedicao >= INTERVALO_MEDICAO) {

    // Atualiza a referencia de tempo
    ultimaMedicao += INTERVALO_MEDICAO;

    // Copia e zera o contador com as interrupcoes pausadas
    noInterrupts();
    unsigned long pulsos = contadorPulsos;
    contadorPulsos = 0;
    interrupts();

    // Converte os pulsos usando a calibracao original
    float velocidadeAtual = converterVelocidade(pulsos);

    // Aplica EMA apenas para o sistema de alerta
    float velocidadeAlerta = aplicarEMA(velocidadeAtual);

    // Controla o LED e o buzzer
    controlarAlerta(velocidadeAlerta);

    // Atualiza o OLED com a velocidade sem EMA
    atualizarOLED(velocidadeAtual);

    // Exibe os dados no Monitor Serial
    Serial.print("Pulsos: ");
    Serial.print(pulsos);

    Serial.print(" | Velocidade OLED: ");
    Serial.print(velocidadeAtual, 1);
    Serial.print(" km/h");

    Serial.print(" | EMA alerta: ");
    Serial.print(velocidadeAlerta, 1);
    Serial.println(" km/h");
  }
}

