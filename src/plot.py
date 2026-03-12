import subprocess
import matplotlib.pyplot as plt
import time

PID = "5482"

prices = []

plt.ion()

while True:
    cmd = f'gdb -p {PID} -batch -ex "p ltp"'
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)

    try:
        line = result.stdout.strip().split("=")[1]
        price = float(line)
        prices.append(price)
    except:
        continue

    plt.clf()
    plt.plot(prices)
    plt.title("Live LTP")
    plt.xlabel("ticks")
    plt.ylabel("price")

    plt.pause(0.1)
    time.sleep(0.1)