import matplotlib.pyplot as plt
import numpy as np
import musures, courant_filtre
import filtre_hamming as filtrage

x = courant_filtre.courant
#x = musures.courant_03A

fs = 100
fc = 50
N = 51
h, n = filtrage.getHammingFilter(fs, fc, N)

h = filtrage.adaptGainUnitaire(h)

x = np.array(x)
y = np.convolve(x, h)

# Créer un vecteur d'indices pour x et y
debut_permanent = len(h)
fin_permanent = len(x)-1
n = np.arange(debut_permanent, fin_permanent)
print(debut_permanent, fin_permanent)

"""
# Afficher le signal d'entrée et le produit de convolution sur le même graphe avec stem
plt.stem(x[debut_permanent:fin_permanent], linefmt='r-', markerfmt='ro', label='Signal d\'entrée')
plt.stem(y[debut_permanent:fin_permanent], linefmt='b-', markerfmt='bo', label='Produit de convolution')

plt.xlabel('Indice')
plt.ylabel('Amplitude')
plt.title('Convolution')
plt.legend(['Signal d\'entrée', 'Produit de convolution'])
plt.show()
"""

#x = (((x / 4095)*3)-2.408)/.066
#y = (((y / 4095)*3)-2.408)/.066

y_med = np.median(y) * 0.066
print(y_med)

plt.axhline(y=0.3, color='g', linestyle='-')

plt.stem(x[debut_permanent:fin_permanent], linefmt='r-', markerfmt='ro', label='Signal d\'entrée')
plt.stem(y[debut_permanent:fin_permanent], linefmt='b-', markerfmt='bo', label='Produit de convolution')

plt.xlabel('Indice')
plt.ylabel('Amplitude')
plt.title('Convolution')
plt.legend(['Signal d\'entrée', 'Produit de convolution'])
plt.show()

print(f"#define N {len(h)}")
print("double filtre[] = {" , end="")
count = 0
for i in h:
    print(f"{i:1.5f},", end=" ")
    count += 1

    if count == 4:
        print()
        count = 0
print("};")