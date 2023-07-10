import matplotlib.pyplot as plt

fig, ax = plt.subplots()

with open('textfiles/circles.txt', 'r') as f:
    circulos = [tuple(map(float,line.split()))for line in f]

puntos = []
with open('textfiles/data_g.txt', 'r') as f:
    puntos = [tuple(map(float, line.split())) for line in f]

# Dibujar círculos
for c in circulos:
    circ = plt.Circle((c[0], c[1]), c[2], fill=False)
    ax.add_artist(circ)

# Dibujar puntos
#x, y = zip(*puntos)
#ax.scatter(x, y, c = 'green')

x_1, y_1 = zip(*puntos[:(len(puntos)//2)])
ax.scatter(x_1, y_1, c = 'green')

x_2, y_2 = zip(*puntos[(len(puntos)//2):])
ax.scatter(x_2, y_2, c = 'orange')

# Ajustar el zoom
plt.axis('scaled')

# Mostrar gráfico
plt.show()
