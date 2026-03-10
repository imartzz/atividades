import matplotlib.pyplot as plt

# Configuração de estilo para parecer moderno (estilo "Seminário")
plt.style.use('bmh')

# Criando a figura com 3 subplots
fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(10, 18))
plt.subplots_adjust(hspace=0.4)

# ---------------------------------------------------------
# GRÁFICO 1: MUNDO (Pizza)
# Dados baseados no Pew Research (aprox.)
labels_mundo = ['Cristãos (31%)', 'Muçulmanos (25%)', 'Sem Religião (16%)', 'Hindus (15%)', 'Outros (13%)']
sizes_mundo = [31, 25, 16, 15, 13]
colors_mundo = ['#4682B4', '#2E8B57', '#A9A9A9', '#DAA520', '#B0C4DE']

ax1.pie(sizes_mundo, labels=labels_mundo, autopct='%1.1f%%', startangle=140, colors=colors_mundo, textprops={'fontsize': 11})
ax1.set_title('Religiões no Mundo: As Duas Gigantes', fontsize=14, fontweight='bold')

# ---------------------------------------------------------
# GRÁFICO 2: BRASIL (Barras Agrupadas - Transição)
# Dados: Queda Católica vs Ascensão Evangélica (IBGE/Datafolha/Pew)
anos = ['2010', '2022 (Est.)']
catolicos = [64.6, 56.7]
evangelicos = [22.2, 26.9]

x = range(len(anos))
width = 0.35

rects1 = ax2.bar([i - width/2 for i in x], catolicos, width, label='Católicos', color='#4682B4')
rects2 = ax2.bar([i + width/2 for i in x], evangelicos, width, label='Evangélicos', color='#DC143C')

ax2.set_ylabel('Porcentagem da População')
ax2.set_title('Brasil em Transição: A Mudança do Cenário', fontsize=14, fontweight='bold')
ax2.set_xticks(x)
ax2.set_xticklabels(anos)
ax2.legend()
ax2.bar_label(rects1, padding=3, fmt='%.1f%%')
ax2.bar_label(rects2, padding=3, fmt='%.1f%%')

# ---------------------------------------------------------
# GRÁFICO 3: BELO HORIZONTE (Barras Horizontais - Atualizado)
# Dados estimados pós-2010 para corrigir o dado de 2000
religioes_bh = ['Católicos', 'Evangélicos', 'Sem Religião', 'Espíritas', 'Outras']
porcentagem_bh = [53.1, 27.3, 9.8, 3.9, 5.9]
colors_bh = ['#4682B4', '#DC143C', '#A9A9A9', '#9370DB', '#B0C4DE']

y_pos = range(len(religioes_bh))

ax3.barh(y_pos, porcentagem_bh, color=colors_bh)
ax3.set_yticks(y_pos)
ax3.set_yticklabels(religioes_bh)
ax3.invert_yaxis()  # Maior em cima
ax3.set_xlabel('Porcentagem Estimada')
ax3.set_title('Cenário Atual em Belo Horizonte (Estimativa)', fontsize=14, fontweight='bold')

# Adicionando os valores nas barras
for i, v in enumerate(porcentagem_bh):
    ax3.text(v + 0.5, i, f'{v}%', va='center', fontweight='bold')

plt.show()