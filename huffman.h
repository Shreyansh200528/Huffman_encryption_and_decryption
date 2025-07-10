#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>

void compress(const std::string& inputFile, const std::string& outputFile);
void decompress(const std::string& inputFile, const std::string& outputFile);

#endif 