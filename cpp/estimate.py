import pandas as pd
from matplotlib import mlab, pyplot as plt
import numpy as np

df = pd.read_csv("results.dat")
print(df)

# First create power sectral densities for normalization
dt = 1./1024


(autospectrum, f) = mlab.psd(df['pressure'].to_numpy(),
                             window=mlab.window_none,
                             NFFT=128, Fs=1./dt)
(crossspectrum, f) = mlab.csd(df['pressure'].to_numpy(), df['flow'].to_numpy(),
                              window=mlab.window_none,
                              NFFT=128, Fs=1./dt)

impedance = crossspectrum/autospectrum

#(ps2, f) = mlab.psd(s2, Fs=1./dt, scale_by_freq=False)
#plt.scatter(f, autospectrum)
#plt.scatter(f, crossspectrum)
plt.scatter(f, abs(impedance))
#plt.scatter(f, np.angle(impedance))

#plt.scatter(f, ps2)
plt.show()

