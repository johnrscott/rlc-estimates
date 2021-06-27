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
  fprintf("%.2fHz\n", f(end))
else
  fprintf("%.2fHz, %.2fHz, %.2fHz, ..., %.2fHz, %.2fHz.\n",
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

# To obtain the phase of the flow in the frequency domain,
# subtract the argument of the impedance from the phase of
# the pressure signal
flow_phase = pressure_phase - angle(Z);

# To generate the time domain version of the pressure and
# flow signals, it is necessary to know the sampling
# frequency and the desired number of samples
num_samples = input("\nInput the desired total number of samples in the simulated data: ");
sampling_freq = input("Input the desired sampling frequency (Hz): ");

pressure_mag = ones(1, length(f))
data = [f' Z' pressure_mag' pressure_phase' flow_mag' flow_phase']

# Get a filename to store the simulated data and write the csv file
fprintf("Choose a filename to store the frequency domain data\n(.csv will be appended)\n")
filebase = input("Filename: ","s");
filename = strcat(filebase, ".csv");
fid = fopen(filename, 'w+');
fprintf(fid,"freq,Z,pressure_mag,pressure_phase,flow_mag,flow_phase\n");
fclose(fid);
csvwrite(filename,data,"-append");
fprintf("Written %s\n",filename);

# The time domain pressure and flow signals are now constructed
# by summing sinusoidal components with the correct magnitude
# and phase (the Fourier series of the signal), and evaluating it
# and the correct sampling points.

##for n in range(num_samples):
time = [];
pressure = [];
flow = [];
for n = 1:num_samples
  ## Store current timestamp
  t = (n-1)/sampling_freq;
  time = [time; t]; # Append to time vector
  
  ## Construct the pressure signal at time t
  p = sum(cos(omega*t - pressure_phase)); # Sum over Fourier components
  pressure = [pressure; p];
  
  ## Construct the flow signal at time t
  f = sum(flow_mag .* cos(omega*t - flow_phase));
  flow = [flow; f];
end

time
pressure
flow

data = [time pressure flow]

## Get a filename to store the simulated data and write the csv file
fprintf("Choose a filename to store the time domain data\n(.csv will be appended)\n")
filebase = input("Filename: ","s");
filename = strcat(filebase, ".csv");
fid = fopen(filename, 'w+');
fprintf(fid,"time,pressure,flow\n");
fclose(fid);
csvwrite(filename,data,"-append");
fprintf("Written %s\n",filename);
