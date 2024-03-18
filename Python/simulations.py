import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from scipy.stats import bernoulli
import time


def binomial_price_sim(S0, T, N, u, r):
    dt = T / N
    d = 1 / u
    p = (np.exp(r * dt) - d) / (u - d)
    q = 1 - p
    disc = np.exp(-r * dt)

    rv = np.random.binomial(1, p, N)
    price = [S0]
    fig = plt.figure()
    viewer = fig.add_subplot(111)
    plt.ion()
    for i in range(N):
        if rv[i]:  # head
            S0 *= u
        else:
            S0 *= d

        price.append(S0)

        viewer.clear()
        viewer.plot(price)
        plt.pause(.1)  # Delay in seconds
        fig.canvas.draw()



if __name__ == "__main__":
    S0 = 100  # initial stock price
    K = 100  # strike price
    T = 1  # time to maturity in years
    r = 0.06  # annual risk-free rate
    N = 100  # number of time steps
    u = 1.1  # up-factor in binomial models
    d = 1 / u  # ensure recombining tree
    opttype = 'C'  # Option Type 'C' or 'P'

    binomial_price_sim(S0, T, N, u, r)
