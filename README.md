# Sistema de Alarme Sonoro e Visual para Segurança em Trabalho em Altura Baseado no Monitoramento da Velocidade do Vento

Projeto acadêmico desenvolvido na disciplina de Sistemas Microcontrolados com o objetivo de monitorar a velocidade do vento em tempo real, utilizando um anemômetro digital baseado em Arduino Uno. O sistema exibe os valores medidos em um display OLED e aciona alertas visuais e sonoros quando os limites de segurança são atingidos.

---

## Objetivo

Desenvolver um sistema capaz de medir a velocidade do vento, apresentar os valores em tempo real em um display OLED e emitir alertas visuais e sonoros quando a velocidade atingir ou ultrapassar o limite de segurança estabelecido em 40 km/h.

---

## Funcionalidades

- Medição da velocidade do vento por meio da contagem de pulsos do anemômetro.
- Exibição da velocidade em km/h em display OLED.
- Conversão de pulsos para velocidade utilizando fatores de calibração.
- Aplicação de filtros digitais para redução de oscilações.
- Atualização automática das leituras a cada segundo.
- Acionamento de LED e buzzer em condições críticas.
- Desligamento automático dos alertas quando a velocidade retorna para níveis seguros.
- Utilização de módulo RTC para registro de data e hora.
- Monitoramento contínuo para validação e testes do sistema.

---

## Hardware e Componentes

- Arduino Uno
- Sensor de velocidade do vento (anemômetro)
- Display OLED SSD1306 128×64 (I²C)
- LED de sinalização
- Buzzer
- Módulo RTC DS3231/DS3232
- Resistores e componentes auxiliares
- Proteus Design Suite para simulação

---

## Tecnologias Utilizadas

- **Arduino IDE** para desenvolvimento do firmware.
- **C/C++** para programação embarcada.
- **Proteus Design Suite** para simulação eletrônica.
- **Adafruit GFX** para renderização gráfica.
- **Adafruit SSD1306** para controle do display OLED.
- **DS3232RTC** para gerenciamento de data e hora.
- **TimeLib** para manipulação temporal.
- **Wire** para comunicação I²C.

---

## Funcionamento do Sistema

O sensor gera pulsos proporcionais à velocidade do vento. O Arduino realiza a contagem desses pulsos em intervalos de um segundo e converte os valores obtidos para velocidade em km/h.

Após a aquisição dos dados, filtros digitais são aplicados para suavizar pequenas variações nas leituras e melhorar a estabilidade da exibição.

Os valores calculados são apresentados em tempo real no display OLED. Quando a velocidade medida atinge ou supera 40 km/h, o sistema aciona o LED e o buzzer para alertar sobre a condição de risco. Os alertas são desativados automaticamente quando a velocidade retorna abaixo do limite configurado.

---

## Estrutura do Projeto

```text
Anemometro-Digital/
│
├── src/
│   └── Código-fonte do Arduino
│
├── Docs/
│   ├── README.md
│   ├── Fluxograma.jpg
│   ├── Test.md
│   └── Historico.md
│
├── imagens/
│   ├── Esquema_Eletrico.jpg
│   ├── Prototipo_Proteus.jpg
│   └── Reed_Switch.jpg
│
├── LICENSE
└── README.md
```

---

## Fluxograma do Sistema

O fluxograma completo do funcionamento do sistema encontra-se na pasta:


Docs/Fluxograma.jpg


---

## Esquema Elétrico

O esquema elétrico utilizado para o desenvolvimento e validação do projeto encontra-se na pasta:





A montagem foi simulada e testada utilizando o Proteus Design Suite.

---

## Testes Realizados

Os procedimentos de validação e testes encontram-se documentados em:

```text
Docs/Test.md
```

Foram realizados testes de:

- Leitura do sensor.
- Conversão de pulsos para velocidade.
- Exibição dos dados no display OLED.
- Acionamento do LED.
- Acionamento do buzzer.
- Integração geral do sistema.

---

## Resultados

Os testes demonstraram o correto funcionamento do sistema em ambiente de simulação.

O anemômetro apresentou capacidade de monitoramento contínuo da velocidade do vento, com atualização periódica das leituras e acionamento adequado dos dispositivos de alerta sempre que o limite de segurança foi atingido.

As oscilações observadas foram compatíveis com o modelo de simulação utilizado e foram reduzidas pela aplicação dos filtros digitais implementados.

---

## Aplicações

- Monitoramento ambiental.
- Sistemas de segurança para trabalho em altura.
- Instrumentação eletrônica.
- Sistemas embarcados.
- Projetos acadêmicos de engenharia.
- Estudos relacionados à medição da velocidade do vento.

---

## Trabalhos Futuros

- Armazenamento de dados em cartão SD.
- Comunicação Wi-Fi para monitoramento remoto.
- Desenvolvimento de aplicativo móvel.
- Integração com plataformas IoT.
- Registro histórico das medições.
- Alimentação por bateria recarregável.

---

## Conclusão

O projeto atingiu os objetivos propostos ao desenvolver um sistema embarcado capaz de monitorar a velocidade do vento em tempo real e emitir alertas visuais e sonoros em situações potencialmente perigosas.

A integração entre sensores, processamento digital e dispositivos de sinalização demonstrou a viabilidade da solução para aplicações de monitoramento e segurança, servindo como base para futuras expansões e melhorias.

---

## Autores

- Rafael Pinheiro Silva
- Nicole Aguiar Costa Mazzoni
- Fabio Rodrigues Moreira
- Gustavo Henrique dos Santos Duarte
- Caio Silva Marinho

---

## Licença

Este projeto foi desenvolvido exclusivamente para fins acadêmicos e educacionais.
