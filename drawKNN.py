import matplotlib.pyplot as plt

fig, ax = plt.subplots()

with open('textfiles/closePoints.txt', 'r') as f:
    puntos_c = [tuple(map(float, line.split())) for line in f]

with open('textfiles/objPoint.txt', 'r') as f:
    obj = [tuple(map(float, line.split())) for line in f]

with open('textfiles/data_g.txt', 'r') as f:
    puntos = [tuple(map(float, line.split())) for line in f]

x, y = zip(*puntos)
ax.scatter(x, y, c='green')

x_, y_ = zip(*puntos_c)
ax.scatter(x_, y_, c = 'yellow')

x1, y1 = zip(*obj)
ax.scatter(x1, y1, c='purple')

# Ajustar el zoom
plt.axis('scaled')

# Mostrar gráfico
plt.show()
