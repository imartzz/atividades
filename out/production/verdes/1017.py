# Coletar os dados
# Certifique-se de fechar o parênteses da função input()
tempo = float(input('Digite o tempo gasto em horas: '))
velocidade = float(input('Digite a velocidade média em km/h: '))
combustivel_gasto = 12.0

# Calcular a distância
distancia = velocidade * tempo

# Calcular litros gastos
litros_gastos = distancia / combustivel_gasto

# Retornar resultado usando a sintaxe correta de f-string
print(f"{litros_gastos:.3f}")