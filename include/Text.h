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
void GetCharFrequency(unordered_map<uint8_t, uint64_t> &frequency_map, string& text);

void GetCharFrequency(unordered_map<uint8_t, uint64_t> &frequency_map, vector<uint8_t> &characters, vector<uint64_t> &frequencies);

void GetCharFrequency(unordered_map<uint8_t, uint64_t> &frequency_map, uint8_t data_block[]);
/*
HuffmanCodes - Map of char to bit representation
 Chunk - A boolean vector to store bits of the data (PBR)
 ChunkSize - Size of the chunk (int)
 TextPtr - Where in the text to read next
 Text - String of text to read and convert to bits
*/
//int CompressText(unordered_map<char, string> &HuffmanCodes, vector<bool> &Chunk, int ChunkSize, int& TextPtr, string& Text);

int CompressText(const shared_ptr<HuffmanTreeNode>& root, vector<bool> &Chunk, int ChunkSize, int& TextPtr, string& Text);

void CompressText(const shared_ptr<HuffmanTreeNode>& root, vector<bool> &Chunk, uint8_t data_block[]);

string DecompressText(const shared_ptr<HuffmanTreeNode>& root, vector<bool> &Chunk, int NValidBits);

#endif //AANY_SCANNING_H
