import pandas as pd
from matplotlib import mlab, pyplot as plt
import numpy as np

df = pd.read_csv("time.csv")
df_freq = pd.read_csv("freq.csv")
print(df_freq)

# First create power sectral densities for normalization
nfft = 128
dt = df['time'][1] - df['time'][0]
fs = 1/dt
print("dt =", dt)
print("fs =", fs)

(autospectrum, f) = mlab.psd(df['pressure'].to_numpy(),
                             window=mlab.window_none,
                             NFFT=nfft, Fs=fs)
(crossspectrum, f) = mlab.csd(df['pressure'].to_numpy(), df['flow'].to_numpy(),
                              window=mlab.window_none,
                              NFFT=nfft, Fs=fs)

# Restrict the spectra to the frequencies contained in the simulation
data = {"freq": f, "auto": autospectrum, "cross": crossspectrum}
spectra = pd.DataFrame(data)
print(spectra)
spectra = spectra[spectra["freq"].isin(df_freq["freq"])]
print(spectra)

#exit()
# Get the radial frequency
omega = 2 * np.pi * spectra["freq"]

# Compute the impedance using Z = Sxy/Sxx
impedance = spectra["cross"]/spectra["auto"]
U = np.real(impedance)
V = np.imag(impedance)

# Use the Tsai and Pimmel formulae to compute R, I and E. The formulae
# will only work if the frequency components in the simulated data line
# up with the spectral density calculations.

# Compute a, b and c
N = len(omega)
a = (1/(omega**2)).sum()
b = (omega**2).sum()
c = N**2 - a*b

# Compute the estimates
R = U.sum()/N
E = (b/c)*(V/omega).sum() - (N/c)*(V*omega).sum()
I = (N/c)*(V/omega).sum() - (a/c)*(V*omega).sum()
print("R =", R)
print("I =", I)
print("E =", E)
