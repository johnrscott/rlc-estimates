import numpy as np 
import random
import pandas as pd

# Set resistance, inertance and elastance 
print("Input desired values for R, I and I. "
      "Type a value and press enter.")
R = float(input("R = "))
I = float(input("I = "))
E = float(input("E = "))

# Compute resonance frequency for reference
fr = np.sqrt(E/I)/(2*np.pi)
print("The resonance frequency for these parameters is: fr = %.2fHz" % (fr))

# Set the maximum and minimum frequencies
print("\nThe pressure and flow data will be "
      "computed at specific equally\n"
      "spaced frequencies. Choose the smallest " 
      "and largest frequencies,\n"
      "and the number of points.")

while True:
    small_freq = float(input("Smallest Frequency (Hz) = "))
    large_freq = float(input("Largest Frequency (Hz) = "))
    num_points = int(input("Number of points = "))
    
    # Sanity check the input values
    if small_freq >= large_freq:
        print("\nThe smallest frequency must be strictly smaller\n"
              "than the largest frequency. Try again.\n")
    elif num_points <= 0:
        print("\nThe number of points must be strictly positive.\n"
              "Try again.\n")
    else:
        # Success, move on to the next step
        break
    

# Generate the frequency vector
f = np.linspace(small_freq, large_freq, num_points) # Frequency in Hertz
omega = 2*np.pi*f # Convert to radial frequency (rads/second)

# Compute the impedance
print("\nThe impedance Z(f) will be computed at", num_points ,
      "frequency point(s):")

# Print a selection of frequencies
if len(f) < 10:
    for n in range(len(f)-1):
        print("%.2fHz, " % (f[n]), end="")
    print("%.2fHz" % (f[-1]))
else:
    print("%.2fHz, %.2fHz, %.2fHz, ..., %.2fHz, %.2fHz."
          % (f[0], f[1], f[2], f[-2], f[-1]))

# Compute the impedance
Z = R + 1j*omega*I + E/(1j*omega)

# Compute random phases for the pressure signal
pressure_phase = np.random.uniform(low=0.0, high=2*np.pi, size=len(f))

# Assuming that the magnitude of the pressure signal is
# one, the magnitude of the flow signal in the frequency
# domain is given by the magnitude of the impedance Z(f)
flow_mag = abs(Z)

# To obtain the phase of the flow in the frequency domain,
# subtract the argument of the impedance from the phase of
# the pressure signal
flow_phase = pressure_phase - np.angle(Z)

# To generate the time domain version of the pressure and
# flow signals, it is necessary to know the sampling
# frequency and the desired number of samples
num_samples = int(input("\nInput the desired total number of "
                        "samples in the simulated data: "))
sampling_freq = float(input("Input the desired sampling frequency (Hz): "))

# The time domain pressure and flow signals are now constructed
# by summing sinusoidal components with the correct magnitude
# and phase (the Fourier series of the signal), and evaluating it
# and the correct sampling points.

data = {"time": [], "pressure": [], "flow": []}

for n in range(num_samples):

    # Store current timestamp
    t = n/sampling_freq
    data["time"].append(t)
    
    # Construct the pressure signal
    data["pressure"].append(np.cos(omega*t - pressure_phase).sum())
    
    # Construct the flow signal
    data["flow"].append((flow_mag * np.cos(omega*t - flow_phase)).sum())


# Make a pandas dataframe so that writing the csv file is easier
df = pd.DataFrame(data)

# Get a filename to store the simulated data
print("Choose a filename to store the simulated data\n(.csv will be appended)\n")
filename = input("Filename: ") + ".csv"
df.to_csv(filename, index=False)
print("Written",filename)
