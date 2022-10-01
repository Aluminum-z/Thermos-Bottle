import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

tempVec = np.linspace(-10.0, 80.0, 50)

def getColorByTemp(temp):
    minTemp = 0
    maxTemp = 80.0
    midTemp = 20
    if (temp < minTemp):
        r = 0.0
        g = 0.6353
        b = 0.9098
    elif (temp < midTemp):
        r = 0.0
        g = 0.6353 + (1.0 - 0.6353) * ((temp - minTemp) / (midTemp - minTemp))
        b = 0.9098 + (1.0 - 0.9098) * ((temp - minTemp) / (midTemp - minTemp))
    elif (temp < maxTemp):
        r = 3.0 * (temp - midTemp) / (maxTemp - midTemp)
        if (r > 1.0):
            r = 1.0
        g = 1.0 - 1.5 * ((temp - midTemp) / (maxTemp - midTemp))
        if (g < 0.0):
            g = 0.0
        b = 1.0 - 3.0 * ((temp - midTemp) / (maxTemp - midTemp))
        if (b < 0.0):
            b = 0.0
    else:
        r = 1.0
        g = 1.0
        b = 1.0
    return (r, g, b)


fig, ax = plt.subplots()
ax.plot(tempVec)
x = 0.0
for temp in tempVec:
    ax.add_patch(patches.Rectangle(
        (x, 0), 1, 5, facecolor=getColorByTemp(temp), fill=True))
    x = x + 1.0
plt.show()
