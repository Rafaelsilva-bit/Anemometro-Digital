# Testes Realizados

## Teste 1 - Leitura do Sensor

### Objetivo
Verificar se o sensor de velocidade do vento gera pulsos corretamente.

### Procedimento
- Conectou-se o sensor ao Arduino.
- Foi realizada a leitura dos pulsos pela porta serial.
- Os valores foram observados durante o funcionamento.

### Resultado
A leitura dos pulsos foi realizada com sucesso.

### Status
✅ Aprovado

---

## Teste 2 - Exibição no Display OLED

### Objetivo
Verificar se os dados são exibidos corretamente.

### Procedimento
- O sistema foi inicializado.
- Foram observados os valores mostrados no display.

### Resultado
Os valores foram exibidos corretamente e atualizados em tempo real.

### Status
✅ Aprovado

---

## Teste 3 - Acionamento do LED de Alerta

### Objetivo
Verificar o funcionamento do alerta visual.

### Procedimento
- Foi simulada uma velocidade acima do limite configurado.

### Resultado
O LED foi acionado quando o limite foi ultrapassado.

### Status
✅ Aprovado

---

## Teste 4 - Acionamento do Buzzer

### Objetivo
Verificar o funcionamento do alerta sonoro.

### Procedimento
- Foi simulada uma condição de velocidade crítica.

### Resultado
O buzzer foi acionado conforme esperado.

### Status
✅ Aprovado

---

## Teste 5 - Funcionamento Geral do Sistema

### Objetivo
Validar a integração entre todos os componentes.

### Procedimento
- O sistema completo foi executado.
- Foram monitoradas as leituras e os alarmes.

### Resultado
O sistema operou corretamente, exibindo os valores medidos e acionando os alertas quando necessário.

### Status
✅ Aprovado
