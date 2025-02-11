import random
import string
import time
import matplotlib.pyplot as plt
from typing import List, Tuple

# Define the Triplet structure
class Triplet:
    def __init__(self, offset: int, length: int, next_char: str):
        self.offset = offset
        self.length = length
        self.next_char = next_char

    def __repr__(self):
        return f"({self.offset}, {self.length}, '{self.next_char}')"

# LZ77 Encoding function
def encode_lz77(data: str) -> List[Triplet]:
    encoded = []
    pos = 0

    while pos < len(data):
        max_len = 0
        best_offset = 0

        for offset in range(1, pos + 1):
            length = 0
            while pos + length < len(data) and data[pos - offset + length] == data[pos + length]:
                length += 1

            if length > max_len:
                max_len = length
                best_offset = offset

        next_char = data[pos + max_len] if pos + max_len < len(data) else ''
        encoded.append(Triplet(best_offset, max_len, next_char))
        pos += max_len + 1

    return encoded

# LZ77 Decoding function
def decode_lz77(encoded: List[Triplet]) -> str:
    text = ''

    for triplet in encoded:
        start_pos = len(text) - triplet.offset
        for i in range(triplet.length):
            text += text[start_pos + i]
        if triplet.next_char:
            text += triplet.next_char

    return text

# Generate random test data
def generate_test_data(length: int, alphabet: str = string.ascii_lowercase) -> str:
    return ''.join(random.choice(alphabet) for _ in range(length))

# Measure execution time
def measure_time(func, *args):
    start_time = time.perf_counter()
    result = func(*args)
    end_time = time.perf_counter()
    return result, end_time - start_time

# Main test function
def main():
    lengths = [100, 1000, 10000, 50000, 100000, 200000, 500000]
    encode_times = []
    decode_times = []

    for length in lengths:
        test_data = generate_test_data(length)

        # Measure encoding time
        encoded, encode_time = measure_time(encode_lz77, test_data)
        encode_times.append(encode_time)

        # Measure decoding time
        _, decode_time = measure_time(decode_lz77, encoded)
        decode_times.append(decode_time)

        print(f"Length: {length} | Encode time: {encode_time:.6f}s | Decode time: {decode_time:.6f}s")

    # Plot results
    plt.figure(figsize=(10, 6))
    plt.plot(lengths, encode_times, label='Encoding Time', marker='o')
    plt.plot(lengths, decode_times, label='Decoding Time', marker='s')
    plt.xlabel('Input Length')
    plt.ylabel('Time (seconds)')
    plt.title('LZ77 Algorithm Efficiency')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    main()
