import serial 
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

ser = serial.Serial('COM6', 19200)

fig, ax = plt.subplots()
xdata, ydata = [], []
ln, = ax.plot([], [], 'ro')

def init():
    ax.set_xlim(0, 1)
    ax.set_ylim(-.01, 1)
    ax.set_xticks([])
    return ln,

def update(frame):
    xdata.append( frame )
    ax.set_xlim(0, frame+1)

    data = int(ser.readline().decode("ascii"))
    data = (((data / 4095)*3)-2.421)/.066
    print(f"i = {' ' if data < 0 else ''}{data:1.3f} A", end='\r')

    ydata.append( data )
    ln.set_data(xdata, ydata)
    return ln,

ani = animation.FuncAnimation(fig, update,init_func=init, blit=True, interval=0)
plt.show()
print()

#plt.savefig("courant_filtre.png")
save = input("Save? (y/n): ")
if save == "y":
    with open("./filtrage/courant_filtre.py", "w") as f:
        f.write("courant = [\n")
        for i in ln.get_data()[1]:
            f.write(f"\t{i},\n")
        f.write("]")