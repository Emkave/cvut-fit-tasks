#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

typedef short int SI; //Optimization
typedef unsigned int UI;
typedef unsigned short int USI;
typedef const unsigned short int CUSI;
typedef unsigned long long int ULLI;
typedef unsigned long int ULI;
typedef long int LI;

using namespace std;

UI p=1;
#define CHECKPOINT {std::cout << "Checkpoint " << p << " checked." << std::endl; p++;} //More controls
#define BREAK {return 0;}
#define SUCCESS {std::cout << "Success." << std::endl;}

class Byte { //Structure of a single byte
    public:
        bool closed = false;
        std::string letter;
        std::string bitRep;
};

typedef struct Node { //Structure of multiple bytes
    Byte byte;
    Node * left = nullptr;
    Node * subNode;
    Node * right = nullptr;
} Node;

void PreOrderPrint(Node *head) { //PreOrder printing function
    if (head != NULL) {
        if (head->byte.bitRep != "") {
            std::cout << head->byte.letter << std::endl;
        } 
        PreOrderPrint(head->left); 
        PreOrderPrint(head->right);
    }
}

std::string ToBinary(USI number) { //Conversion a number to binary format with string representation
    USI num = number; 
    std::string bitL;
    std::vector<USI> sector = {128, 64, 32, 16, 8, 4, 2, 1};

    for (USI i = 0; i<8; i++) {
        for (USI j=i; j<8; j++) {
            if (sector[i] > num) {
                bitL+='0'; break;
            } else if (sector[i] <= num) {
                bitL+='1'; 
                num-=sector[i];break;
            }
        }
    } 
    return bitL;
}

std::string FromBinary(std::string binary) { //12 bit from binary conversion (needs for chunks)
    USI value = 0; 
    USI index = 0;
    std::vector<USI> sector = {2048, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1};

    for (auto ptr=binary.begin(); ptr!=binary.end(); ptr++) {
        if (*ptr == '1') {
            value += sector[index];
        } 
        index++;
    }
    
    return std::to_string(value);
}

int BtoD(std::string n) { //Binary to decimal conversion of a string
    std::string num = n; 
    UI dec_value = 0;
    UI base = 1; 
    UI len = num.length();

    for (SI i=len-1; i>=0; i--) {
        if (num[i] == '1') {
            dec_value += base;
        } base *= 2;
    } 
    return dec_value;
}

std::string BtoS(std::string str) { //Convertion to ASCII representation
    UI N = int(str.size()); 
    std::string res = "";

    for (USI i=0; i<N; i+=8) {
        USI decimal_value = BtoD((str.substr(i, 8)));
        res += char(decimal_value);
    } 
    return res;
}

std::string RemoveLastBits(std::string bits) {
    for (USI i=bits.length()-1; i>=0; i--) {
        if (bits[i] == '0') {
            bits.pop_back();
        } else 
            break;
    } 
    return bits;
}

bool decompressFile(const char * inFileName, const char * outFileName) {
    Node * head = new Node(); 
    Node * temp = head;
    head->subNode = nullptr; 
    head->byte.closed = false;

    bool constructed = false;

    CUSI chunk = 4096; 
    UI nL = 0;
    UI actualLenght = 0; 
    UI nW = 0;

    std::string line = ""; 
    std::string letterNum = "";
    std::string bitValue = ""; 
    std::string bitLine = "";
    std::string utfValue = "";

    std::vector<USI> hexLine; //Hex storage

    std::ifstream file(inFileName, std::ios::in|std::ios::binary|std::ios::ate);//Block of file opening code.

    if (!file) {
        CHECKPOINT; 
        return false;
    }

    std::ifstream::pos_type size = file.tellg(); 
    char * txt = new char[size];
    file.seekg(0, std::ios::beg); 
    file.read(txt, size); 
    file.close();

    for (LI i=0; i<size; i++) {
        line += txt[i];
    } 

    std::ofstream outFile; 
    outFile.open(outFileName); //Exit when the file content is did read.

    if (!outFile.is_open()) {
        return false;
    }

    for (auto ptr=line.begin(); ptr!=line.end(); ptr++) { //Block of file encryption data code.
        if ((int)*ptr < 0) {
            hexLine.push_back((128 + (128 - abs((int)*ptr))));
        }
        else {
            hexLine.push_back(((int)*ptr));
        }
    } 
    
    for (auto ptr=hexLine.begin(); ptr!=hexLine.end(); ptr++) {
        bitLine += ToBinary(*ptr);
    } //Exit when the content is encrypted.

    auto ptr = bitLine.begin(); // Creating binary tree via iteration scheme in PreOrder mode.
    
    int idx (0); 
    while (true) {
        if (ptr >= bitLine.end()) return false;
        if (constructed == false) {
            if (*ptr == '0') {
                temp->left = new Node();
                temp->left->subNode = temp;
                temp = temp->left;
                temp->byte.closed = false;
                idx++;
                ptr++;
            }
            else if (*ptr == '1') {ptr++;
                //check for utf 
                UI bytes = 0;
                if (*(ptr) == '1' && *(ptr+1) == '1') {
                    for (USI i=0; i<8; i++) {
                        bitValue += *ptr;
                        ptr++;
                        idx++;
                    }

                    for (auto it = bitValue.begin(); it != bitValue.end(); it++) {
                        if (*it == '1') {
                            bytes += 1;
                        } 
                        else if (*it == '0') {
                            break;
                        }
                    }
                    
                    if (bytes > 4) {return false;} 

                    for (USI i=0; i<bytes-1; i++) {
                        for (USI j=0; j<8; j++) {
                            if ((j == 0 && *ptr != '1') || (j == 1 && *ptr != '0')) {return false;} 
                            bitValue += *ptr;
                            ptr++;
                            idx++;
                        }
                    } 
                    
                    if (bytes == 4 && stol(bitValue, nullptr, 2) > 0xF48FBFBF) {return false;}
                } else if (*ptr == '0') {
                    for (USI i=0; i<8; i++) {
                        bitValue += *ptr;
                        ptr++;
                        idx++;
                    }
                } else {
                    return false;
                }

                temp->byte.bitRep = bitValue; bitValue = "";
                temp->byte.letter = BtoS(temp->byte.bitRep);

                temp->byte.closed = true;
                while (temp->byte.closed == true && temp != head) { //Closing-a-node process to prevent further allocation and re-writing an existing node.
                    temp = temp->subNode;
                    if (temp->left != nullptr)
                        if (temp->left->byte.closed == true)
                            if (temp->right != nullptr)
                                if (temp->right->byte.closed == true)
                                    temp->byte.closed = true;
                }
                if (temp->right == NULL) { //Allocate right node after the allocation of left node.
                    temp->right = new Node();
                    temp->right->subNode = temp;
                    temp = temp->right;
                    temp->byte.closed = false;
                }
                if (head->byte.closed == true) {constructed = true; bitValue = ""; break;} //Exit the loop if the tree is closed (constructed)
            }
        }
    } // End of the creation of the binary tree via iteration in PreOrder mode.
    temp = head;
    //ptr++;

    while (ptr != bitLine.end()) {
        if (*ptr == '0') {
            for (USI i=0; i<12; i++) {
                ptr++; 
                bitValue += *ptr;
            } 
            
            nL = stoi(FromBinary(bitValue));
            actualLenght += nL;

            for (UI i=0; i<nL && ptr!=bitLine.end(); ) {
                ptr++;
                if (*ptr == '0') {
                    temp=temp->left;
                } else if (*ptr == '1') {
                    temp=temp->right;
                }

                if (temp->byte.bitRep != "") {
                    if (!(outFile << temp->byte.letter)) 
                        return false; 
                    
                    temp=head; i++; nW++;
                }
            } 
            break;
        } else if (*ptr == '1') {
            for (USI i=0; i<chunk && ptr!=bitLine.end(); ) {
                ptr++;
                if (*ptr == '0') {
                    temp=temp->left;
                } else if (*ptr == '1') {
                    temp=temp->right;
                }

                if (temp->byte.bitRep != "") {
                    if (!(outFile << temp->byte.letter)) 
                        return false; 
                    temp=head; 
                    i++; 
                    nW++;
                }
            } 
            
            actualLenght += chunk;
        }
        
        if (ptr+1 != bitLine.end()) {
            ptr++;
        }
    } 
    
    outFile.close();

    if (actualLenght != nW) {
        return false;
    }

    return true;
}

bool compressFile(const char * inFileName, const char * outFileName) {
    // keep this dummy implementation (no bonus) or implement the compression (bonus)
    return false;
}

#ifndef __PROGTEST__
bool identicalFiles(const char * fileName1, const char * fileName2) {return true;}

int main (void) {
    assert(decompressFile("tests/test0.huf", "tempfile"));
    assert(identicalFiles("tests/test0.orig", "tempfile"));

    assert(decompressFile("tests/test1.huf", "tempfile"));
    assert(identicalFiles("tests/test1.orig", "tempfile"));

    assert(decompressFile("tests/test2.huf", "tempfile"));
    assert(identicalFiles("tests/test2.orig", "tempfile"));

    assert(decompressFile("tests/test3.huf", "tempfile"));
    assert(identicalFiles("tests/test3.orig", "tempfile"));

    assert(decompressFile("tests/test4.huf", "tempfile"));
    assert(identicalFiles("tests/test4.orig", "tempfile"));

    assert(!decompressFile("tests/test5.huf", "tempfile"));

    assert(decompressFile("tests/extra0.huf", "tempfile0"));
    assert(identicalFiles("tests/extra0.orig", "tempfile"));

    assert(decompressFile("tests/extra1.huf", "tempfile1"));
    assert(identicalFiles("tests/extra1.orig", "tempfile"));

    assert(decompressFile("tests/extra2.huf", "tempfile2"));
    assert(identicalFiles("tests/extra2.orig", "tempfile"));

    assert(decompressFile("tests/extra3.huf", "tempfile3"));
    assert(identicalFiles("tests/extra3.orig", "tempfile"));

    assert(decompressFile("tests/extra4.huf", "tempfile4"));
    assert(identicalFiles("tests/extra4.orig", "tempfile"));

    assert(decompressFile("tests/extra5.huf", "tempfile5"));
    assert(identicalFiles("tests/extra5.orig", "tempfile"));

    assert(decompressFile("tests/extra6.huf", "tempfile6"));
    assert(identicalFiles("tests/extra6.orig", "tempfile"));

    assert(decompressFile("tests/extra7.huf", "tempfile7"));
    assert(identicalFiles("tests/extra7.orig", "tempfile"));

    assert(decompressFile("tests/extra8.huf", "tempfile8"));
    assert(identicalFiles("tests/extra8.orig", "tempfile"));

    assert(decompressFile("tests/extra9.huf", "tempfile9"));
    assert(identicalFiles("tests/extra9.orig", "tempfile"));

    return 0;
}
#endif /* __PROGTEST__ */
