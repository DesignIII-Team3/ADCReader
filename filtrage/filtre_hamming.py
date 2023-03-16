import numpy as np
import matplotlib.pyplot as plt

def getHammingFilter(f_s, f_c, N):
    if N % 2 == 0:  # s'assurer que N soit impair
        raise ValueError("N doit être impair")

    D = 3.21 # dB impose par fenetre de Hamming

    delta_f = (D * f_s) / (N -1)

    w_c = 2 * np.pi * f_c / f_s

    # Calcul de la valeur de N
    delta_f = 1 + (D*f_s)/delta_f

    # Calcul de la valeur de M  
    M = (N - 1) / 2

    n = np.arange(-M, M+1)

    # calcul de la réponse impulsionnelle
    h = (w_c/np.pi) * np.sinc( (w_c/np.pi) * (np.arange(-M, M+1) / f_s)) * np.hamming(N)

    return (h, n)

def adaptGainUnitaire(h):
    N = len(h)
    H = np.fft.fft(h, N)
    H /= np.max(H)

    return np.real(np.fft.ifft(H))

if __name__ == "__main__":
    h, n = getHammingFilter(10, 5, 25)

    N = len(h)
    H = np.fft.fft(h, N)
    H = np.fft.fftshift(H)

    H_unitaire = H / np.max(H)

    w = np.arange(-N/2, N/2) * 2 * np.pi / N

    module = np.abs(H)
    module_unitaire = np.abs(H_unitaire) 

    # valeur max de module
    print(max(module))

    plt.stem(w/np.pi, module, linefmt='r-',markerfmt='ro')
    plt.stem(w/np.pi, module_unitaire, linefmt='b-',markerfmt='bo')
    plt.show()

    plt.stem(n, adaptGainUnitaire(h) )
    plt.show()