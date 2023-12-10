//
// Created by Noah Markowitz on 12/9/23.
//

// CSV files: https://www.stats.govt.nz/large-datasets/csv-files-for-download/
// https://www.datablist.com/learn/csv/download-sample-csv-files
// https://loremipsum.de/index_en.html

#include "../include/HuffmanTreeNode.h"
#include "../include/Text.h"
#include "../include/BuildHuffmanTree.h"
#include "../include/GenerateHuffmanCodes.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <utility>
#include <chrono>

/*
Timing

chrono::time_point<chrono::steady_clock> start, stop;
chrono::duration<double> difference_in_time;
double difference_in_seconds; // Holds the final run time
start = chrono::steady_clock::now();
operation()
stop = chrono::steady_clock::now();
difference_in_time = stop - start;
difference_in_seconds = double(difference_in_time.count());
 */
// g++ -std=c++17 test/huffman_timing_tests.cpp src/Text.cpp src/HuffmanTreeNode.cpp src/BuildHuffmanTree.cpp src/GenerateHuffmanCodes.cpp -o bin/huffman_timing_tests.out
using namespace std;



void printHuffmanTree(const std::shared_ptr<HuffmanTreeNode>& root, int depth = 0) {
    if (!root) {
        return;
    }

    const int spaces = 4;

    printHuffmanTree(root->right, depth + 1);

    for (int i = 0; i < depth * spaces; ++i) {
        std::cout << ' ';
    }

    if (root->has_value) {
        std::cout << root->value << " (" << root->freq << ")" << std::endl;
    } else {
        std::cout << '-' << " (" << root->freq << ")" << std::endl;
    }

    printHuffmanTree(root->left, depth + 1);
}

void TimingTest(string fname, ofstream& outfile){

    char c;
    int f, i;
    string c_code;

    string pdir = "/Users/noahmarkowitz/Documents/GitHub/AANY/examples/";

    ifstream testfile;
    testfile.open(pdir + fname, ios::in);
    if (!testfile)
    {
        cout << "Error opening file " << endl;
        return;
    }
    outfile << fname << ",";

    std::string example_text((std::istreambuf_iterator<char>(testfile)), std::istreambuf_iterator<char>());
    testfile.close();
    //cout << example_text << endl;

    // Set up timing variables
    // t_start and t_stop gotten by: chrono::steady_clock::now();
    chrono::time_point<chrono::steady_clock> t_start, t_stop; // Start and stop times
    chrono::duration<double> difference_in_time; // Will be t_stop - t_start
    double difference_in_seconds; // Holds the final run time. double(difference_in_time.count());

    // Map for character frequency
    unordered_map<uint8_t, uint64_t> frequency_map;


    //cout << "PASS1" << endl;
    // Get frequency of each character
    t_start = chrono::steady_clock::now();

    GetCharFrequency(frequency_map, example_text);

    t_stop = chrono::steady_clock::now();
    difference_in_time = t_stop - t_start;
    double get_char_freq_time = double(difference_in_time.count());
    cout << "Getting char frequency took: " << get_char_freq_time << "s" << endl;

    // Spit out number of characters metric
    int n_characters = 0;
    int n_unique_characters = 0;
    unordered_map<uint8_t , uint64_t>::iterator it;
    for (it=frequency_map.begin(); it != frequency_map.end(); it++){
        n_characters += it->second;
        n_unique_characters++;
    }
    cout << "Unique Characters: " << n_unique_characters << endl << "Total Characters: " << n_characters << endl;

    outfile << n_characters << ",";
    outfile << n_unique_characters << ",";
    outfile << get_char_freq_time << ",";

    // Build the tree
    t_start = chrono::steady_clock::now();

    std::shared_ptr<HuffmanTreeNode> root = BuildHuffmanTree(frequency_map);

    t_stop = chrono::steady_clock::now();
    difference_in_time = t_stop - t_start;
    double build_tree_time = double(difference_in_time.count());
    cout << "Building tree took: " << build_tree_time << "s" << endl;
    outfile << build_tree_time << ",";

    //cout << "PASS3" << endl;
    // Generate numeric codes for each character in the tree
    t_start = chrono::steady_clock::now();

    unordered_map<char, string> HuffmanCodes = GenerateHuffmanCodes(root);

    t_stop = chrono::steady_clock::now();
    difference_in_time = t_stop - t_start;
    double generate_codes_time = double(difference_in_time.count());
    cout << "Generating Huffman Codes from tree took: " << generate_codes_time << "s" << endl;
    outfile << generate_codes_time << ",";

    // Convert huffman tree to array
    t_start = chrono::steady_clock::now();

    pair< vector<uint8_t>, vector<uint64_t> > HuffmanArrayRep = HuffmanTreeToArray(root);

    t_stop = chrono::steady_clock::now();
    difference_in_time = t_stop - t_start;
    double tree_to_array_time = double(difference_in_time.count());
    cout << "Converting tree to array took: " << tree_to_array_time << "s" << endl;
    outfile << tree_to_array_time << ",";


    // Convert array(s) back to unordered map

    t_start = chrono::steady_clock::now();

    unordered_map<uint8_t, uint64_t> frequency_map2;
    vector<uint8_t> characters = HuffmanArrayRep.first;
    vector<uint64_t> frequencies = HuffmanArrayRep.second;
    GetCharFrequency(frequency_map2,characters,frequencies);

    t_stop = chrono::steady_clock::now();
    difference_in_time = t_stop - t_start;
    double array_to_tree_time = double(difference_in_time.count());
    cout << "Converting array to tree took: " << array_to_tree_time << "s" << endl;
    outfile << array_to_tree_time << ",";


    // Compress text. First create a vector<bool> that has enough space for all the bits
    int n_bits = 0;
    //unordered_map<uint8_t, uint64_t> frequency_map;
    //unordered_map<char, string> HuffmanCodes

    for (it=frequency_map.begin(); it != frequency_map.end(); it++){
        c = (char)it->first;
        f = it->second;
        n_bits += f*HuffmanCodes[c].length();
    }


    t_start = chrono::steady_clock::now();

    vector<bool> chunk(n_bits,false);
    int textptr = 0;
    int NValidBits = CompressText(root, chunk, n_bits, textptr, example_text);

    t_stop = chrono::steady_clock::now();
    difference_in_time = t_stop - t_start;
    double text_compression_time = double(difference_in_time.count());
    cout << "Text Compression took: " << text_compression_time << "s" << endl;
    outfile << text_compression_time << ",";

    // Decompress text
    t_start = chrono::steady_clock::now();

    string decompressed_text = DecompressText(root, chunk, NValidBits);

    t_stop = chrono::steady_clock::now();
    difference_in_time = t_stop - t_start;
    double text_decompression_time = double(difference_in_time.count());
    cout << "Text Decompression took: " << text_decompression_time << "s" << endl;
    //cout << "Decompressed Text: " << decompressed_text << endl;
    outfile << text_decompression_time << endl;

    return;

}

int main(){

    // List all the files to use in an unordered map
    vector<string> files;
    files.push_back("lorem_ipsum_100.txt");
    files.push_back("lorem_ipsum_1000.txt");
    files.push_back("lorem_ipsum_10000.txt");
    files.push_back("lorem_ipsum_100000.txt");
    files.push_back("lorem_ipsum_1000000.txt");
    //files.push_back("lorem_ipsum_10000000.txt");
    files.push_back("organizations-100.csv");
    files.push_back("organizations-1000.csv");
    files.push_back("organizations-10000.csv");
    //files.push_back("annual-enterprise-survey-2021-financial-year-provisional-csv.csv");
    files.push_back("annual-enterprise-survey-2021-financial-year-provisional-size-bands-csv.csv");
    //files.push_back("machine-readable-business-employment-data-sep-2023-quarter.csv");
    files.push_back("business-operations-survey-2022-business-finance.csv");

    // Start output csv file
    ofstream outfile("output.csv");
    outfile << "file_name,n_chracters,n_unique_characters,get_freq_time,build_tree_time,generate_code_time,tree_to_array_time,array_to_tree_time,compression_time,decompression_time" << endl;

    vector<string>::iterator it;
    for (it=files.begin(); it != files.end(); it++){
        cout << "********************************************" << endl;
        cout << "Working on: " << *it << endl;
        TimingTest(*it, outfile);
    }
    // TimingTest(files[i], ofstream)
    outfile.close();
    return 0;

}