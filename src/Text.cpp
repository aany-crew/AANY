//
// Created by Noah Markowitz on 11/27/23.
//

#include "../include/Text.h"

using namespace std;

// Scan text for frequency of characters
void GetCharFrequency(unordered_map<uint8_t, uint64_t> &frequency_map, string& text)
{
    int i;
    int l = text.length();
    uint8_t c_num;

    // Iterate over each character in the text
    for (i=0; i<l; i++){
        c_num = (uint8_t)text[i];
        if (frequency_map.find(c_num) == frequency_map.end()){
            frequency_map[c_num] = 1;
        }
        else{
            frequency_map[c_num]++;
        }
    }

    return;
}

/*
HuffmanCodes - Map of char to bit representation
 Chunk - A boolean vector to store bits of the data (PBR)
 ChunkSize - Size of the chunk (int)
 TextPtr - Where in the text to read next
 Text - String of text to read and convert to bits
*/

// Each chunk will be a vector<bool>
int CompressText(unordered_map<char, string> &HuffmanCodes, vector<bool> &Chunk, int ChunkSize, int& TextPtr, string& Text){

    int TotalBits = 0; // Total bits converted thus far
    int ChunkArrayPtr = 0; // Pointer to where to store the next bit in the Chunk
    int char_code_len; // String length of the code for that character
    int char_code_ptr = 0; // Pointer to where in the code for that character you are
    char C; // Single character to scan from text
    string C_code; // The Huffman code corresponding to that char C
    int Text_len = Text.length();
    bool keep_going = true;

    while (keep_going){

        // Get the next character to scan and get its code and length of the code
        C = Text[TextPtr];
        C_code = HuffmanCodes[C];
        char_code_len = C_code.length();

        // Create a temporary storage for the bits
        vector<bool> tmp_bitarray(char_code_len,false);

        // Make sure there's enough storage space left in this chunk
        if ( (char_code_len+TotalBits) < ChunkSize && TextPtr < Text_len){

            // Convert code to bits
            while (char_code_ptr < char_code_len){
                Chunk[ChunkArrayPtr] = C_code[char_code_ptr] == '1';
                char_code_ptr++;
                ChunkArrayPtr++;
            }

            // Reset char_code_ptr and increment TextPtr
            char_code_ptr = 0;
            TextPtr++;
            TotalBits += char_code_len;
        } else {
            keep_going = false;
        }
        //cout << "Just did: " << C << endl;
        //cout << "Did an iter!" << endl;
    }

    return TotalBits;
}

string DecompressText(const shared_ptr<HuffmanTreeNode>& root, vector<bool> &Chunk, int NValidBits){

    // Current node in HuffmanTree pointing to
    shared_ptr<HuffmanTreeNode> current_node;
    int BitArrayPtr = 0; // Which bit in the chunk to read
    bool CurrentBit;
    string OutputText = "";
    char c;

    // Keep reading the Chunk until nothing left to read
    while(BitArrayPtr<NValidBits){
        current_node = root;

        // Keep traversing the tree until a character is found for the bits
        while(current_node->has_value == false){

            CurrentBit = Chunk[BitArrayPtr];
            BitArrayPtr++;

            current_node = CurrentBit==true ? root->right : root->right;
        }

        // Get the character
        c = current_node->value;

        // Append character to output
        OutputText = OutputText + c;
    }

    // Return string
    return OutputText;
}