# AANY
Primary repo for project for EC504 2023F

## Members
* Adwait Kulkami
* Yuqi Jin
* Noah Markowitz
* Austin Jamias

---

## Resources used

* [ChatGPT](https://chat.openai.com/)
* [Geeks for Geeks](https://www.geeksforgeeks.org/)
* [Stack Overflow](https://stackoverflow.com/)
* Fall 2024 EC504 class notes

---

## Data

Sample data obtained from:

* https://www.lipsum.com/
* https://media.githubusercontent.com/media/datablist/sample-csv-files/main/files/organizations/organizations-100.csv
* https://www.stats.govt.nz/large-datasets/csv-files-for-download/
* https://www.datablist.com/learn/csv/download-sample-csv-files

---

## Instructions for Running the Code
* g++ -std=c++17 src/simple_server.cpp src/Text.cpp src/HuffmanTreeNode.cpp src/BuildHuffmanTree.cpp src/GenerateHuffmanCodes.cpp -o bin/simple_server.out
* ./bin/simple_server.out
* Enter Filename: (need to copy paste the path of the example)
* Enter Command(PUT/GET): (enter PUT)
* Will show something in terminal
* Enter Filename: (copy paste the same path before)
* Enter Command: (enter GET)
* Will show something in terminal
* These steps represent that user chooses a file to compress it and send it to server. Then user wants to retrieve a file from server by decompressing the compressed file
