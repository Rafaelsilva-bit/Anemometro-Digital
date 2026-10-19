# Documentação Técnica

* **Visão Geral**
  * Projeto: Anemômetro digital para monitoramento da velocidade do vento em tempo real.
  * Aplicação: Segurança em operações de trabalho em altura.
  * Funcionamento: Aquisição de pulsos via sensor Reed Switch, processamento por Arduino Uno, sincronização via RTC, exibição em display LCD 20x4 (I²C com PCF8574) e acionamento de LED (visual) e Buzzer (sonoro).

* **Arquitetura do Sistema**
  * Sensor (Reed Switch)
  * Arduino Uno
  * Dispositivos de saída:
    * Display LCD 20x4 (I²C)
    * LED (Alerta Visual)
    * Buzzer (Alerta Sonoro)
    * RTC DS3231/DS3232 (Tempo Real)

* **Componentes Utilizados**
  * **Arduino Uno:** Controle principal do sistema.
  * **Reed Switch:** Simulação da geração de pulsos do anemômetro.
  * **Display LCD 20x4:** Exibição das informações operacionais.
  * **PCF8574:** Interface I²C do LCD.
  * **Buzzer:** Alerta sonoro.
  * **LED:** Alerta visual.
  * **RTC DS3231/DS3232:** Sincronização temporal.
  * **Potenciômetro 10 kΩ:** Ajuste de contraste do LCD.
  * **Resistores:** Proteção e condicionamento do circuito.

* **Lógica de Funcionamento**
  * Geração de pulsos proporcionais à velocidade do vento pelo sensor Reed Switch.
  * Contagem de pulsos e aplicação de filtros digitais para suavização das leituras pelo Arduino.
  * Conversão dos pulsos em km/h utilizando fator de calibração.
  * Exibição do valor e do status atualizado no display LCD 20x4.
  * Verificação dos limites de segurança:
    * Menor que 40 km/h: Operação normal.
    * Entre 40 e 45 km/h: Alerta intermitente.
    * Maior ou igual a 45 km/h: Perigo / Alerta contínuo.

* **Critérios de Alarme (Níveis de Criticidade)**
  * **Menor que 40 km/h:** Status `NORMAL` (operação segura, sem alertas).
  * **Entre 40 e 45 km/h:** Status `ALERTA` (alerta intermitente com LED e Buzzer ativos).
  * **Maior ou igual a 45 km/h:** Status `PERIGO` (alerta contínuo ativo até a normalização).

* **Testes Realizados**
  * **Teste de Leitura:** Validação da aquisição de pulsos via Reed Switch e suavização por filtros digitais.
  * **Teste de Exibição:** Verificação da comunicação I²C e atualização em tempo real no display LCD 20x4.
  * **Teste de Limites:** Confirmação do acionamento correto do LED e do Buzzer nas faixas de 40 km/h (`ALERTA`) e 50 km/h (`PERIGO`).

* **Limitações**
  * Dependência da calibração mecânica e elétrica do sensor de pulsos.
  * Influência de ruídos no ambiente simulado/real.
  * Validação inicial restrita ao ambiente de simulação (Proteus Design Suite).

* **Melhorias Futuras**
  * Registro histórico das medições utilizando o RTC.
  * Armazenamento de dados em cartão SD.
  * Comunicação Wi-Fi e integração com plataformas IoT.
  * Desenvolvimento de aplicativo móvel para monitoramento remoto.
  * Utilização de sensores comerciais de maior precisão.

* **Conclusão**
  * Validação da viabilidade de um sistema microcontrolado de baixo custo para monitoramento do vento.
  * Êxito na clareza operacional via LCD 20x4 e resposta imediata dos alarmes para prevenção de acidentes em altura.
