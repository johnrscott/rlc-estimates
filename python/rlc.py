import pandas as pd
import numpy as np

df = pd.read_csv("rlc.csv", comment="#")

# Compute resonance frequency
df["fr"] = 1/(2 * np.pi * np.sqrt(df["L"]*df["C"]))


print(df)
