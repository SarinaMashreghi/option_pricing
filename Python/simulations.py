import numpy as np

import imageio
import os
import cv2



def binomial_price_sim(frame_number, S0=100, T=1, N=100, u=1.1, r=0.6):
    x.append(frame_number)
    dt = T / N
    d = 1 / u
    p = (np.exp(r * dt) - d) / (u - d)
    q = 1 - p
    disc = np.exp(-r * dt)

    rv = np.random.binomial(1, p, N)
    #fig = plt.figure()
    #viewer = fig.add_subplot(111)
    #plt.ion()
    for i in range(N-1):
        if rv[i]:  # head
            S0 *= u
        else:
            S0 *= d

        y.append(S0)

        #viewer.clear()
        #viewer.plot(price)
        #plt.pause(.1)  # Delay in seconds
        #fig.canvas.draw()


def animate(frame_number, price=0):
    x.append(frame_number)
    y.append(price)
    line.set_xdata(x)
    line.set_ydata(y)
    return line,

def write_video(video_name, image_dir):
    image_files = [os.path.join(image_dir, f) for f in os.listdir(image_dir) if
                   f.endswith('.png') or f.endswith('.jpg')]
    image_files.sort(key=lambda x: int(x[len(image_dir):-4]))

    writer = imageio.get_writer(video_name, fps=20)
    for filename in image_files:
        image = imageio.imread(filename)
        writer.append_data(image)
    writer.close()

    #for filename in image_files:
    #    os.remove(filename)


def write_video_2(video_name, image_dir):
    image_files = [os.path.join(image_dir, f) for f in os.listdir(image_dir) if
                   f.endswith('.png') or f.endswith('.jpg')]
    image_files.sort()

    frame = cv2.imread(image_files[0])
    height, width, layers = frame.shape

    video = cv2.VideoWriter(video_name, cv2.VideoWriter_fourcc(*'DIVX'), 100, (width, height))

    for image in image_files:
        video.write(cv2.imread(image))

    cv2.destroyAllWindows()
    video.release()


if __name__ == "__main__":
    S0 = 100  # initial stock price
    K = 100  # strike price
    T = 1  # time to maturity in years
    r = 0.06  # annual risk-free rate
    N = 1000  # number of time steps
    u = 1.1  # up-factor in binomial models
    d = 1 / u  # ensure recombining tree
    opttype = 'C'  # Option Type 'C' or 'P'

    write_video("test_30.mp4", "../CPP/build/test/")


