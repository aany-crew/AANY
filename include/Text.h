//
// Created by Noah Markowitz on 11/27/23.
//
#ifndef AANY_SCANNING_H
#define AANY_SCANNING_H

#include "../include/HuffmanTreeNode.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

using namespace std;

// Scan text and generate array of number of instances of each ascii character
int* GetCharFrequency(string& text);

/*
HuffmanCodes - Map of char to bit representation
 Chunk - A boolean vector to store bits of the data (PBR)
 ChunkSize - Size of the chunk (int)
 TextPtr - Where in the text to read next
 Text - String of text to read and convert to bits
*/
void CompressText(unordered_map<char, string> &HuffmanCodes, vector<bool> &Chunk, int ChunkSize, int& TextPtr, string& Text);

string DecompressText(const shared_ptr<HuffmanTreeNode>& root, vector<bool> &Chunk, int ChunkSize, int& BitArrayPtr);

#endif //AANY_SCANNING_H
