import math

a = float(input("Informe A: "))
b = float(input("Informe B: "))
c = float(input("Informe C: "))

if a == 0 and b == 0 and c == 0:
    print("Igualdade confirmada")
elif a == 0 and b == 0 and c != 0:
    print("Coeficientes informados incorretamente")
elif a == 0 and b != 0:
    print("Esta é uma equação de primeiro grau.")
    x = -c / b
    print(f"Raiz real da equação: x = {x:.2f}")
elif a != 0:
    print("Esta é uma equação de segundo grau.")
    delta = b**2 - 4 * a * c
    if delta < 0:
        print("Esta equação não possui raízes reais.")
        print(delta)
    elif delta == 0:
        print("Esta equação possui duas raízes reais iguais.")
        x = -b / (2 * a)
        print(f"x' = x'' = {x:.2f}")
    else: # delta > 0
        print("Esta equação possui duas raízes reais diferentes.")
        x1 = (-b + math.sqrt(delta)) / (2 * a)
        x2 = (-b - math.sqrt(delta)) / (2 * a)
        print(f"x' = {x1:.2f}, x'' = {x2:.2f}")