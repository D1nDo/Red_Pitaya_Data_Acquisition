import numpy as np
import matplotlib.pyplot as plt

# Binary file name
file_name = 'external_data.bin'

# 1. Open and read binary file
try:
    data_32bit = np.fromfile(file_name, dtype=np.uint32)
except IOError:
    raise Exception("Unable to open file!")

num_samples = len(data_32bit)
print(f"File read successfully! Found {num_samples} samples at 32-bit.")

# 2. Bit separation (Bitmasking and Shift)

# First 16 bits (Low part)
first_16bit_unsigned = data_32bit & 0x0000FFFF

# Second 16 bits (High part)
second_16bit_unsigned = data_32bit >> 16

# 3. Convert to SIGNED (int16)
# This converts raw bits into signed values (-32768 to +32767)
first_16bit_signed = first_16bit_unsigned.astype(np.uint16).view(np.int16)
second_16bit_signed = second_16bit_unsigned.astype(np.uint16).view(np.int16)

# 4. Plot the two separated channels
plt.figure("Separated Channel Analysis (16-bit Signed)", figsize=(10, 6))

# --- First 16 bits ---
plt.subplot(2, 1, 1)
plt.plot(first_16bit_signed, linewidth=1.2)
plt.grid(True)
plt.xlabel("Sample Index")
plt.ylabel("Amplitude (int16)")
plt.title("Channel 1 (First 16 bits - Low)")

# --- Second 16 bits ---
plt.subplot(2, 1, 2)
plt.plot(second_16bit_signed, linewidth=1.2)
plt.grid(True)
plt.xlabel("Sample Index")
plt.ylabel("Amplitude (int16)")
plt.title("Channel 2 (Second 16 bits - High)")

plt.tight_layout()
plt.show()
