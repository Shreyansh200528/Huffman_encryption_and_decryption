# Huffman Compression and Decompression
This project implements a simple **Huffman Encoding and Decoding** algorithm in C++. The Huffman coding algorithm is a lossless data compression algorithm which assigns codewords to data (characters) in a variable-length way that depends on the frequency of the character in the input data. It minimizes the total size of any text file although it maintains all original contents.

## Features

-  Compresses text files using Huffman coding.
-  Decompresses encoded files to restore original content.
-  Handles special edge case: files containing only one unique character.
-  Compact and easy-to-understand codebase.

## How It Works

### 1. Compression

- Frequency of each character is counted.
- A Huffman tree is built based on frequency.
- Each character is assigned a binary code.
- The original content is encoded into bits.
- A header (code table + padding info) is saved for decoding.

### 2. Decompression

- Reads the Huffman code table and encoded bit stream.
- Decodes the bits using the code table.
- Outputs the original file content.

### 3. Single Character Shortcut

If the input file contains only one unique character (e.g., `aaaaaaa`), the program writes a special marker to the compressed file and skips building a Huffman tree. This saves overhead wastage.
## Usage
- Compression of .txt and .log files to improve storage efficiency in large databases.
- Reduces the original file size by up to 60%.

### Build
Compile the project with g++:
g++ -o huffman.exe main.cpp huffman.cpp
### Running
./huffman.exe





