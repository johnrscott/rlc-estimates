import pandas as pd
from matplotlib import mlab, pyplot as plt
import numpy as np

df_time = pd.read_csv("subject_0_time.csv")
df_freq = pd.read_csv("subject_0_freq.csv")

# Compute the sampling frequency assuming that the time
# steps in the file are uniform
dt = df_time['time'][1] - df_time['time'][0]
fs = 1/dt
S = len(df_time) # Number of samples
#print("A = {0:d}, B = {1:2f}".format(2, 3.5))
print("Sampling frequency of the simulated data is: %.2fHz" % (fs))
print("Number of samples in the simulated data is:", S)

# Compute the frequency resolution of the components in the
# simulated signal. This is important so as to choose the parameters
# of the spectral estimation correctly so that the frequency
# components in the spectra line up with those in the simulated
# data
df = df_freq['freq'][1] - df_freq['freq'][0]
print("Frequency resolution of the simulated data is: %.2fHz" % (df))

# The frequency resolution of the spectral estimation is given by
# fs/nfft. The length of the Fourier transform nfft should be chosen
# so that this resolution is equal to the resolution in the
# simulated data
nfft = int(fs/df) # Round to nearest integer

# Sanity check the transform length
if nfft >= S:
    print("\nThere are not enough sampled points to get the frequency"
          "\nresolution you need. Consider halving nfft.")
    exit()

print("Calculated length of Fourier transform:", nfft)

(autospectrum, f) = mlab.psd(df_time['pressure'].to_numpy(),
                             window=mlab.window_none,
                             NFFT=nfft, Fs=fs)
(crossspectrum, f) = mlab.csd(df_time['pressure'].to_numpy(),
                              df_time['flow'].to_numpy(),
                              window=mlab.window_none,
                              NFFT=nfft, Fs=fs)

# Make a pandas dataframe containing the full spectra results
data = {"freq": f, "auto": autospectrum, "cross": crossspectrum}
spectra_full = pd.DataFrame(data)

# Filter the full spectra to keep only those frequencies that
# are contained in the simulated signals. If the values of the
# spectra are used at frequencies not contained in the simulation
# then the estimated impedance will be wrong at those frequencies
# and consequently the estimate of R I and E will be wrong.
spectra = spectra_full[spectra_full["freq"].isin(df_freq["freq"])]

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
L = (N/c)*(V/omega).sum() - (a/c)*(V*omega).sum()
C = 1 / ((b/c)*(V/omega).sum() - (N/c)*(V*omega).sum())
print("R =", R)
print("L =", L)
print("C =", C)
