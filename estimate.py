import pandas as pd
from matplotlib import mlab, pyplot as plt

df = pd.read_csv("results.dat")
print(df)

# First create power sectral densities for normalization
dt = 1/1024
(ps1, f) = mlab.psd(df['pressure'].to_numpy(), Fs=1./dt, scale_by_freq=False)
(ps2, f) = mlab.psd(df['flow'].to_numpy(), Fs=1./dt, scale_by_freq=False)

#(ps2, f) = mlab.psd(s2, Fs=1./dt, scale_by_freq=False)
plt.scatter(f, ps1)
plt.scatter(f, ps2)
plt.show()

