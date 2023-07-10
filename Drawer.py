import matplotlib.pyplot as plt

# Leer valores del archivo txt
with open('textfiles/circle.txt', 'r') as f:
     centro_x, centro_y, radio = map(float, f.readline().split())

# Crear figura y ejes
fig, ax = plt.subplots()

# Dibujar círculo
circulo = plt.Circle((centro_x, centro_y), radio, fill=False)
ax.add_artist(circulo)

# Leer puntos del archivo txt
puntos = []
with open('textfiles/data_g.txt', 'r') as f:
    puntos = [tuple(map(float, line.split())) for line in f]

puntos1 = puntos[:(len(puntos)//2)]
puntos2 = puntos[(len(puntos)//2):]

# Dibujar puntos
#x, y = zip(*puntos)
#ax.scatter(x, y)

x_1, y_1 = zip(*puntos1)
ax.scatter(x_1, y_1, c = 'green')

x_2, y_2 = zip(*puntos2)
ax.scatter(x_2, y_2, c = 'orange')

# Leer puntos extremos del archivo txt
with open('textfiles/e_points.txt', 'r') as f:
    puntos_ = [tuple(map(float, line.split())) for line in f]

# Dibujar puntos extremos
#x_, y_ = zip(*puntos_)
#ax.scatter(x_, y_, c = 'purple')

# Ajustar el zoom
plt.axis('scaled')

# Mostrar gráfico
plt.show()
