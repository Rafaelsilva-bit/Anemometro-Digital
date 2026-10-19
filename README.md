# Sistema de Alarme Sonoro e Visual para Segurança em Trabalho em Altura Baseado no Monitoramento da Velocidade do Vento

Projeto desenvolvido na disciplina de **Sistemas Microcontrolados** com o objetivo de monitorar a velocidade do vento em tempo real e emitir alertas visuais e sonoros quando condições de risco forem identificadas.

O sistema utiliza um **Arduino Uno**, um sensor baseado em **Reed Switch**, um **display OLED** para visualização dos dados e dispositivos de alerta para aumentar a segurança em operações de trabalho em altura.

---

## Integrantes

- Rafael Pinheiro Silva
- Nicole Aguiar Costa Mazzoni
- Fabio Rodrigues Moreira
- Gustavo Henrique dos Santos Duarte
- Caio Silva Marinho

---

## Objetivo

Desenvolver um sistema embarcado capaz de:

- Medir a velocidade do vento em tempo real;
- Exibir as informações ao usuário por meio de um display OLED;
- Alertar operadores quando forem detectadas velocidades potencialmente perigosas;
- Auxiliar na prevenção de acidentes em atividades realizadas em altura.

---

## Justificativa

Trabalhos realizados em altura estão sujeitos a riscos adicionais quando há ventos intensos. O monitoramento contínuo da velocidade do vento é uma medida importante para garantir a segurança operacional.

Este projeto propõe o desenvolvimento de um sistema de baixo custo capaz de realizar esse monitoramento, fornecendo informações em tempo real e alertas automáticos quando os limites de segurança forem ultrapassados.

---

## Funcionalidades

- Medição automática da velocidade do vento;
- Conversão dos pulsos gerados pelo sensor em km/h;
- Exibição dos dados em display OLED;
- Atualização das leituras a cada segundo;
- Aplicação de filtros digitais para suavização das leituras;
- Alerta visual utilizando LED;
- Alerta sonoro utilizando buzzer;
- Registro de data e horário utilizando RTC;
- Monitoramento em tempo real.

---

## Hardware Utilizado

| Componente | Função |
|------------|---------|
| Arduino Uno | Controle principal do sistema |
| Reed Switch | Simulação da geração de pulsos do anemômetro |
| Display OLED SSD1306 | Exibição das informações |
| Buzzer | Alerta sonoro |
| LED | Alerta visual |
| RTC DS3231/DS3232 | Controle de data e hora |
| Resistores | Proteção do circuito |
| Proteus | Simulação e validação do projeto |

---

## Tecnologias Utilizadas

- Arduino IDE
- Linguagem C/C++
- Proteus Design Suite
- Adafruit GFX
- Adafruit SSD1306
- Wire
- TimeLib
- DS3232RTC

---

## Funcionamento

O sistema realiza a contagem dos pulsos gerados pelo sensor conectado ao Arduino.

Esses pulsos são processados e convertidos em velocidade do vento utilizando um fator de calibração definido durante os testes.

Após o processamento:

1. A velocidade é calculada.
2. O valor é exibido no display OLED.
3. O sistema verifica o limite de segurança.
4. Caso a velocidade seja superior a 40 km/h:
   - o LED é acionado;
   - o buzzer é acionado;
   - o alerta permanece ativo enquanto a condição de risco existir.

---

## Fluxograma do Sistema

O fluxograma completo encontra-se na pasta:

```text
Docs/Fluxograma.jpg
```

---

## Esquema Elétrico

O circuito foi desenvolvido e validado utilizando o Proteus Design Suite.

O esquema completo encontra-se em:

![Protótipo no Proteus](imagens/Prototipo%20Proteus)

---

## Estrutura do Repositório

```text
Anemometro-Digital
│
├── src/
│   └── Código-fonte do projeto
│
├── Docs/
│   ├── README.md
│   ├── Fluxograma.jpg
│   ├── historico.md
│   └── testes.md
│
├── imagens/
│   ├── Prototipo Proteus.jpeg
│   ├── Reed switch.jpeg
│   └── Imagem do Projeto feita por IA.jpg
│
├── LICENSE
│
└── README.md
```

---

## Metodologia de Desenvolvimento

O desenvolvimento do projeto foi dividido nas seguintes etapas:

1. Levantamento dos requisitos.
2. Pesquisa sobre sensores e métodos de medição.
3. Desenvolvimento do circuito no Proteus.
4. Implementação do software embarcado.
5. Integração entre hardware e software.
6. Testes e validação.
7. Documentação técnica.

---

## Testes Realizados

Os procedimentos de teste encontram-se documentados em:

```text
Docs/testes.md
```

Entre os testes realizados destacam-se:

- Teste de leitura do sensor;
- Teste de exibição no OLED;
- Teste de acionamento do LED;
- Teste de acionamento do buzzer;
- Teste de integração do sistema;
- Teste de comportamento em velocidades críticas.

---

## Resultados Obtidos

Os testes demonstraram que o sistema foi capaz de:

- Identificar alterações na velocidade simulada do vento;
- Atualizar os dados em tempo real;
- Exibir corretamente as informações no display OLED;
- Acionar os dispositivos de alerta quando a velocidade ultrapassa o limite configurado;
- Operar de forma estável em ambiente de simulação.

---

## Aplicações

O projeto pode ser aplicado em:

- Monitoramento ambiental;
- Segurança em trabalho em altura;
- Sistemas embarcados educacionais;
- Instrumentação eletrônica;
- Projetos de automação e monitoramento.

---

## Trabalhos Futuros

- Armazenamento das medições em cartão SD;
- Comunicação Wi-Fi;
- Integração com plataformas IoT;
- Desenvolvimento de aplicativo móvel;
- Monitoramento remoto via internet;
- Utilização de sensores comerciais de maior precisão.

---

## Conclusão

O projeto atingiu os objetivos estabelecidos ao desenvolver um sistema de monitoramento da velocidade do vento com capacidade de emitir alertas sonoros e visuais em situações consideradas de risco.

Os resultados obtidos demonstram a viabilidade da utilização de plataformas microcontroladas de baixo custo para aplicações de monitoramento e segurança, servindo como base para futuras melhorias e expansões do sistema.

---

## Documentação Complementar

A documentação técnica do projeto pode ser encontrada na pasta:

```text
Docs/README.md
```

Documentos disponíveis:

- README técnico;
- Fluxograma;
- Histórico de desenvolvimento;
- Relatório de testes.

---

## Licença

Este projeto foi desenvolvido exclusivamente para fins acadêmicos e educacionais.
