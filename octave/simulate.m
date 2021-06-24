# Set resistance, inertance and elastance 
fprintf("Input desired values for R, L and C. \
Type a value and press enter.\n")

R = input("R = ");
L = input("L = ");
C = input("C = ");

# Compute resonance frequency for reference
fr = 1 / (2 * pi * sqrt(L*C));
fprintf("The resonance frequency for these parameters is: \
fr = %.2fHz\n", fr)

## Set the maximum and minimum frequencies
fprintf("\nThe pressure and flow data will be \
computed at specific equally\n\
spaced frequencies. Choose the smallest \
and largest frequencies,\nand the number of points.\n")

while true
  small_freq = input("Smallest Frequency (Hz) = ");
  large_freq = input("Largest Frequency (Hz) = ");
  num_points = input("Number of points = ");

  ## Sanity check the input values
  if small_freq >= large_freq
    fprintf("\nThe smallest frequency must be strictly smaller\n than the largest frequency. Try again.\n")
  elseif num_points <= 0
    fprintf("\nThe number of points must be strictly positive.\nTry again.\n")
  else
    ## Success, move on to the next step
    break
  end
end

## Generate the frequency vector
f = linspace(small_freq, large_freq, num_points); # Frequency in Hertz
omega = 2*pi*f; # Convert to radial frequency (rads/second)

## Compute the impedance
fprintf("\nThe impedance Z(f) will be computed at %d frequency point(s):\n", num_points)

## Print a selection of frequencies
if length(f) < 10
    for n = 1:length(f)-1
      fprintf("%.2fHz, ", f(n))
    end
    fprintf("%.2fHz", f(end))
else
  fprintf("%.2fHz, %.2fHz, %.2fHz, ..., %.2fHz, %.2fHz.",
	  f(1), f(2), f(3), f(end-1), f(end))
end

## Compute the impedance
Z = R + 1j*omega*L + 1./(1j*omega*C);

## Compute random phases for the pressure signal
pressure_phase = 2 * pi * rand(1,length(f));

# Assuming that the magnitude of the pressure signal is
# one, the magnitude of the flow signal in the frequency
# domain is given by the magnitude of the impedance Z(f)
flow_mag = abs(Z);

# To generate the time domain version of the pressure and
# flow signals, it is necessary to know the sampling
# frequency and the desired number of samples
num_samples = input("\nInput the desired total number of samples in the simulated data: ");
sampling_freq = input("Input the desired sampling frequency (Hz): ");

pressure_mag = ones(1, length(f))
T = table(f, Z, pressure_mag)

