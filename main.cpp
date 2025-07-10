#include "huffman.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Check if the input file contains only one repeated character (ignoring CR/LF)
bool isSingleCharFile(const string &inputFile, char &onlyChar, int &count) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        cerr << "Error: cannot open input file\n";
        return false;
    }

    // Read entire file
    string data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    

    unordered_map<char,int> freq;
    for (char c : data) freq[c]++;

    if (freq.size() == 1) {
        onlyChar = freq.begin()->first;
        count    = freq.begin()->second;
        return true;
    }
    return false;
}

// Write the special compressed form: 1 <count> <char>
void writeSingleCharCompressed(const string &outFile, char onlyChar, int count) {
    ofstream out(outFile, ios::binary);
    if (!out) {
        cerr << "Error: cannot open output file\n";
        return;
    }
    out << "1 " << count << " " << onlyChar;
}

// Read and decompress the special form
bool decompressSingleChar(const string &inFile, const string &outFile) {
    ifstream in(inFile, ios::binary);
    if (!in) {
        cerr << "Error: cannot open compressed file\n";
        return false;
    }

    int flag;
    in >> flag;
    if (flag != 1) return false;

    int cnt;
    char ch;
    in >> cnt >> ch;
    in.close();

    ofstream out(outFile, ios::binary);
    for (int i = 0; i < cnt; ++i) out.put(ch);
    return true;
}

int main() {
    const string input       = "input.txt";
    const string compressed  = "output.txt";
    const string decompressed = "reconstructed.txt";
    char onlyChar = 0;
    int count     = 0;

    // 1) If the file is a single repeated character, use shortcut
    if (isSingleCharFile(input, onlyChar, count)) {
        writeSingleCharCompressed(compressed, onlyChar, count);
        decompressSingleChar(compressed, decompressed);
        cout << "Single-character shortcut used.\n";
        return 0;
    }

    // 2) Otherwise, do full Huffman compress + decompress
    compress(input, compressed);
    cout << "Compression complete.\n";

    decompress(compressed, decompressed);
    cout << "Decompression complete.\n";
    return 0;
}
