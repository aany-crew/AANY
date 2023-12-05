//
// Created by Noah Markowitz on 11/27/23.
//

#include "../include/Text.h"

using namespace std;

// Scan text for frequency of characters
int* GetCharFrequency(string& text)
{
    int i;
    int n;
    char c;
    int l = text.length();

    // Generate array
    //int arr[128] = {0};
    int* arr = new int[128];
    for (i=0; i<128; i++){
        arr[i] = 0;
    }

    // Iterate over each character in the text
    for (i=0; i<l; i++){
        c = text[i];
        n = int(c);
        arr[n] += 1;
    }

    return arr;
}

/*
HuffmanCodes - Map of char to bit representation
 Chunk - A boolean vector to store bits of the data (PBR)
 ChunkSize - Size of the chunk (int)
 TextPtr - Where in the text to read next
 Text - String of text to read and convert to bits
*/

// Each chunk will be a vector<bool>
void CompressText(unordered_map<char, string> &HuffmanCodes, vector<bool> &Chunk, int ChunkSize, int& TextPtr, string& Text){

    int TotalBits = 0; // Total bits converted thus far
    int ChhunkArrayPtr = 0; // Pointer to where to store the next bit in the Chunk
    int char_code_len; // String length of the code for that character
    int char_code_ptr = 0; // Pointer to where in the code for that character you are
    char C; // Single character to scan from text
    string C_code; // The Huffman code corresponding to that char C

    bool keep_going = true;

    while (keep_going){

        // Get the next character to scan and get its code and length of the code
        C = Text[TextPtr];
        C_code = HuffmanCodes[C];
        char_code_len = C_code.length();

        // Create a temporary storage for the bits
        vector<bool> tmp_bitarray(char_code_len,false);

        // Make sure there's enough storage space left in this chunk
        if (char_code_len + TotalBits < ChunkSize){

            // Convert code to bits
            while (char_code_ptr < char_code_len){
                Chunk[ChhunkArrayPtr] = C_code[char_code_ptr] == '1' ? true : false;
                char_code_ptr++;
                ChhunkArrayPtr++;
            }

            // Reset char_code_ptr and increment TextPtr
            char_code_ptr = 0;
            TextPtr++;
        } else {
            keep_going = false;
        }
    }
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