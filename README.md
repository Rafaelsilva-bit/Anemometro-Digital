SISTEMA DE ALARME SONORO E VISUAL PARA SEGURANÇA EM
TRABALHO EM ALTURA, BASEADO NO MONITORAMENTO DA
VELOCIDADE DO VENTO

Projeto acadêmico de desenvolvimento de um anemômetro digital para medição e monitoramento da velocidade do vento, utilizando Arduino Uno, display OLED e simulação de circuito no Proteus.

## Objetivo

Desenvolver um sistema capaz de medir a velocidade do vento, apresentar os valores em tempo real em um display OLED e emitir alertas visuais e sonoros quando a velocidade atingir o limite estabelecido de 40 km/h.

## Funcionalidades

- Medição da velocidade do vento por meio da contagem de pulsos do anemômetro.
- Exibição da velocidade em km/h em um display OLED de 128 × 64 pixels.
- Calibração da conversão de pulsos para velocidade, utilizando valores de referência.
- Aplicação de filtros digitais para reduzir oscilações e melhorar a estabilidade das leituras.
- Atualização das medições em intervalos de um segundo.
- Acionamento de LED e buzzer quando a velocidade atinge ou ultrapassa 40 km/h.
- Desligamento dos alertas quando a velocidade fica abaixo do limite.
- Utilização de módulo RTC para suporte à data e hora do sistema.
- Monitoramento das leituras para auxiliar nos testes e na validação do projeto.

## Hardware e Componentes

- Arduino Uno
- Anemômetro simulado no Proteus
- Display OLED SSD1306, resolução de 128 × 64 pixels, comunicação I²C
- LED para sinalização visual
- Buzzer para sinalização sonora
- Módulo RTC DS3232/DS3231
- Circuito simulado no Proteus Design Suite

## Tecnologias Utilizadas

- **Arduino IDE:** desenvolvimento e compilação do código.
- **Linguagem C/C++:** programação do microcontrolador.
- **Proteus Design Suite:** montagem e simulação do circuito eletrônico.
- **Bibliotecas:** Adafruit GFX, Adafruit SSD1306, DS3232RTC, TimeLib e Wire.

## Funcionamento do Sistema

O anemômetro gera pulsos de acordo com a velocidade do vento. O Arduino contabiliza esses pulsos durante intervalos de um segundo e converte a quantidade medida em uma velocidade aproximada, utilizando a calibração definida no código.

Para melhorar a leitura, são utilizados filtros digitais que ajudam a reduzir pequenas oscilações sem comprometer a resposta às mudanças de velocidade.

O resultado é apresentado no display OLED em km/h. Quando a velocidade atinge ou ultrapassa 40 km/h, o sistema aciona o LED e o buzzer. Quando a velocidade fica abaixo desse limite, os alertas são desligados.

## Estrutura do Projeto

```text
Anemometro-digital/
│
├── src/          # Código-fonte do Arduino
├── imagens/      # Imagens do circuito e dos testes
└── README.md     # Documentação do projeto
```

*Observação: a estrutura acima é uma sugestão de organização e deve corresponder às pastas existentes no repositório.*

## Simulação e Testes

O projeto é desenvolvido e testado no Proteus, permitindo variar a velocidade do vento simulada e observar o comportamento do sistema.

Os testes incluem a verificação da calibração, da estabilidade das leituras no OLED, da resposta às mudanças de velocidade e do acionamento dos alertas ao atingir o limite de 40 km/h.

Nos testes realizados até o momento, o display apresentou pequenas oscilações em torno de 10 km/h e 40 km/h, mantendo uma resposta rápida às alterações de velocidade. Esses resultados correspondem às condições simuladas e não representam, por si só, uma validação física do equipamento.

## Aplicações

- Instrumentação e monitoramento ambiental.
- Estudos de velocidade do vento.
- Projetos acadêmicos de engenharia mecatrônica.
- Sistemas embarcados.
- Simulação e desenvolvimento de sistemas de medição.
- Monitoramento com alertas visuais e sonoros.

## Autores

- Rafael Pinheiro Silva
- Nicole Aguiar Costa Mazzoni
- Fabio Rodrigues Moreira
- Gustavo Henrique dos Santos Duarte
- Caio Silva Marinho

## Licença

Este projeto foi desenvolvido para fins acadêmicos e educacionais.
