#include "huffman.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <string>
#include <bitset>
#include <cstdint>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    ~Node() {
        delete left;
        delete right;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) const {
        return a->freq > b->freq;
    }
};

Node* buildHuffmanTree(const unordered_map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;
    for (const auto& kv : freq) {
        minHeap.push(new Node(kv.first, kv.second));
    }
    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();
        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        minHeap.push(merged);
    }
    return minHeap.empty() ? nullptr : minHeap.top();
}

void buildCodes(Node* root, const string& prefix, unordered_map<char, string>& codes) {
    if (!root) return;

    if (!root->left && !root->right) {
        codes[root->ch] = (prefix.empty() ? "0" : prefix);
        return;
    }

    buildCodes(root->left, prefix + "0", codes);
    buildCodes(root->right, prefix + "1", codes);
}

void compress(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        cerr << "Error: cannot open input file\n";
        return;
    }
    string data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    unordered_map<char, int> freq;
    for (char c : data) freq[c]++;

    Node* root = buildHuffmanTree(freq);
    unordered_map<char, string> codes;
    buildCodes(root, "", codes);
    delete root;

    string encoded;
    for (char c : data) encoded += codes[c];

    ofstream out(outputFile, ios::binary);
    if (!out) {
        cerr << "Error: cannot open output file\n";
        return;
    }
    out << codes.size() << "\n";
    for (auto& kv : codes) {
        out << static_cast<int>(kv.first) << " " << kv.second << "\n";
    }
    out << "-1\n";

    int padding = 8 - (encoded.size() % 8);
    if (padding == 8) padding = 0;
    out.put(static_cast<char>(padding));

    uint8_t byte = 0;
    int bitCount = 0;
    for (char b : encoded) {
        byte = (byte << 1) | (b - '0');
        bitCount++;
        if (bitCount == 8) {
            out.put(static_cast<char>(byte));
            byte = 0;
            bitCount = 0;
        }
    }
    if (bitCount) {
        byte <<= (8 - bitCount);
        out.put(static_cast<char>(byte));
    }
    out.close();
}

void decompress(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in) {
        cerr << "Error: cannot open compressed file\n";
        return;
    }
    size_t tableSize;
    in >> tableSize;
    unordered_map<string, char> revCodes;
    for (size_t i = 0; i < tableSize; ++i) {
        int ascii;
        string code;
        in >> ascii >> code;
        revCodes[code] = static_cast<char>(ascii);
    }
    string marker;
    in >> marker; 
    in.get();

    char paddingChar;
    in.get(paddingChar);
    int padding = static_cast<unsigned char>(paddingChar);

    string encodedBits;
    char byteChar;
    while (in.get(byteChar)) {
        bitset<8> bits(static_cast<unsigned char>(byteChar));
        encodedBits += bits.to_string();
    }
    in.close();

    if (padding > 0 && padding <= 8) {
        encodedBits.erase(encodedBits.end() - padding, encodedBits.end());
    }

    ofstream out(outputFile, ios::binary);
    if (!out) {
        cerr << "Error: cannot open output file for decompression\n";
        return;
    }
    string buffer;
    for (char bit : encodedBits) {
        buffer += bit;
        auto it = revCodes.find(buffer);
        if (it != revCodes.end()) {
            out.put(it->second);
            buffer.clear();
        }
    }
    out.close();
}
