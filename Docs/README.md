# Documentação Técnica

## Visão Geral

O projeto consiste em um anemômetro digital desenvolvido para monitoramento da velocidade do vento em aplicações relacionadas à segurança em trabalho em altura. O sistema realiza a aquisição dos sinais do sensor, processa os dados e apresenta os resultados em um display, além de emitir alertas visuais e sonoros quando limites pré-estabelecidos são atingidos.

---

# Arquitetura do Sistema

```text
Sensor de Vento
       │
       ▼
Arduino Uno
       │
 ┌─────┼─────┐
 ▼     ▼     ▼
OLED  LED  Buzzer
```

---

# Componentes Utilizados

| Componente | Função |
|------------|---------|
| Arduino Uno | Controle principal |
| Sensor de velocidade do vento | Aquisição de dados |
| Display OLED | Exibição das informações |
| LED | Alerta visual |
| Buzzer | Alerta sonoro |
| Resistores | Proteção e condicionamento |

---

# Lógica de Funcionamento

1. O sensor gera pulsos de acordo com a velocidade do vento.
2. O Arduino realiza a contagem desses pulsos.
3. Os pulsos são convertidos para velocidade.
4. O valor é exibido no display.
5. Caso o limite seja ultrapassado:
   - LED é acionado;
   - Buzzer é acionado;
   - Alerta é exibido na tela.

---

# Critérios de Alarme

| Faixa | Situação |
|---------|-----------|
| Normal | Operação segura |
| Atenção | Velocidade elevada |
| Crítico | Acionamento do alarme |

---

# Testes Realizados

## Teste 1

Objetivo: verificar aquisição dos pulsos.

Resultado: leitura realizada com sucesso.

## Teste 2

Objetivo: validar exibição no display.

Resultado: valores apresentados corretamente.

## Teste 3

Objetivo: validar acionamento dos alarmes.

Resultado: LED e buzzer acionados conforme esperado.

---

# Limitações

- Dependência da calibração do sensor.
- Influência de ruídos elétricos.
- Necessidade de validações em ambiente real.

---

# Melhorias Futuras

- Registro de dados em cartão SD.
- Comunicação Wi-Fi.
- Aplicativo para monitoramento remoto.
- Alimentação por bateria.
- Dashboard web.

---

# Conclusão

O sistema desenvolvido demonstrou a viabilidade da medição da velocidade do vento utilizando uma plataforma microcontrolada de baixo custo, permitindo monitoramento em tempo real e emissão de alertas para apoio à segurança operacional.
