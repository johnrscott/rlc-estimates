import pandas as pd
from matplotlib import mlab, pyplot as plt
import numpy as np

df = pd.read_csv("results.csv")
print(df)

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
print(f)
exit()
# Get the radial frequency
omega = 2 * np.pi * f

# Compute the impedance using Z = Sxy/Sxx
impedance = crossspectrum/autospectrum
U = np.real(impedance)
V = np.imag(impedance)

# Remove the DC component of the spectral densities (where f = 0)
U = np.delete(U, 0)
V = np.delete(V, 0)
omega = np.delete(omega,0)

# Also remove the component at Fs, which is aliased to DC.
for n in range:
    U = np.delete(U, -1)
    V = np.delete(V, -1)
    omega = np.delete(omega,-1)

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
