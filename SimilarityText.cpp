//Compilation command : g++ SimilarityText.cpp SHA256.cpp md5.cpp -o first &&.\first.exe
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;
#include "SHA256.h"

const int BLOOM_FILTER_SIZE = 1000;


//SHA256 Function by SystemGlitch : https://github.com/System-Glitch/SHA256
string generateSHA256(string plaintext){
    stringstream sha1;
    SHA256 sha;
    sha.update(plaintext);
    std::array<uint8_t, 32> digest = sha.digest();
    sha1 << SHA256::toString(digest) << std::endl;
    return sha1.str();
    // cout << sha1.str() << endl;
}


// // In C++ long long is 64bits 8 bytes SHA256 = 32 bytes 
// // So this function divides hashSHA256 to index in bloom filter
// Pupose : convert SHA256 to integer index  
unsigned long long hashSHA256_to_index(const std::string& hexStr) {
    unsigned long long result = 0;
    for (size_t i = 0; i < hexStr.length(); ++i) {
        result = result * 16 + (hexStr[i] >= '0' && hexStr[i] <= '9' ? hexStr[i] - '0' :
                                hexStr[i] >= 'a' && hexStr[i] <= 'f' ? hexStr[i] - 'a' + 10 : 0);
    }
    return result;
}


string get2Gram (string inputString){
        string output;
        for (int i = 0 ; i < inputString.size()-1 ; i++){
                output = output + inputString.substr(i , 2) + ' ';
        }
        //substracting 1 allows to remove the final space character
        return output.substr(0,output.size()-1);
}
void place_word_in_bloom_filter(string inputString,bool * BIT_ARRAY){
        string plaintext_withspace_prefix = inputString.insert(0,"_");
        string finalone = plaintext_withspace_prefix.insert (plaintext_withspace_prefix.size(),1,'_');
        string twoGram = get2Gram(finalone);
        stringstream ss (twoGram);
        //Stream contains _O OM MA AR R_ space seperated
        string buffer;
        //Tokeniser
        while (ss>>buffer){
                //counting the number of 2grams global_count++;
                string SHA256 = generateSHA256(buffer);
                BIT_ARRAY[hashSHA256_to_index(SHA256)%BLOOM_FILTER_SIZE] = 1;
        }
        
}


//Purpose : Find the number of common true values in two bloom filters
int getNumofCommonTrueValues(bool * atom1 , bool * atom2){
  int counter = 0;
  for (int i = 0 ; i < BLOOM_FILTER_SIZE ; i++){
    // if atom1 at i is 1 and atom2 at i is 1 so sum is 2
    //so they have common true 
    if (atom1[i] + atom2[i] == 2){ 
      counter++;
    }    
  }
  return counter;
}

//Purpose : Find the number of true values in bloom filters/ atom
int count_Num_of_True(bool * atom){
  int counter =0;
  for (int i = 0 ; i <BLOOM_FILTER_SIZE ; i++ ){
    if (atom[i]==1){
      counter ++;
    }
  }
  return counter;
}

// Purpose : Takes user input as string and populate in heap memory
bool* bloom_filter_string_to_pointer(string bloomfilter){
  // Must request heap for memory and must populate with 0 else there will be junk values
  bool * bloomfilterAtom = new bool[BLOOM_FILTER_SIZE]{0};
  for (int i = 0 ; i <BLOOM_FILTER_SIZE ; i++ ){
    *(bloomfilterAtom+i)=bloomfilter[i]-48;
    // Test : cout << *(bloomfilterAtom+i);
  }
  return bloomfilterAtom;
}


// Purpose : Calculate Jaccard coff based on two bloom filters
double getJaccardCof (bool * atom1 , bool * atom2){
    int commons = getNumofCommonTrueValues(atom1,atom2);
    int trues_in_atom1 =count_Num_of_True(atom1);
    int trues_in_atom2 = count_Num_of_True(atom2);
    // Jaccard coff formula can be understood from code line bellow
    double jaccard = commons/(double)(trues_in_atom1+trues_in_atom2-commons);
    return jaccard;
}

// // Purpose : helps option 1 to print on console the bloom filter of password
// void ProduceBloomFilter_cout_console(string password){
//     bool * atom1 =  getAtom(password);
//     // iterate throuh bloomfilter and print each value on console
//     for (int i = 0 ; i <1000 ; i++){
//       cout << *(atom1+i);
//     }

// }

bool* Make_file_2grams_BloomFilter(string filename){
        bool * BIT_ARRAY = new bool[BLOOM_FILTER_SIZE]{0};
        ifstream in (filename.c_str());
        if (!in){
                cout << "Error: FIle not exist";
        }
        string password;
        while (in>>password){
                // Each password in placed in bitarray
                place_word_in_bloom_filter(password,BIT_ARRAY);
        }
        in.close();
        return BIT_ARRAY;
}
int main(){
        cout << "Welcome to Similarity Dataset test \n";
        cout << "Note: This  Similarity test is conducted on bases of presences of bigram(2gram)\n";
        cout << "Comparisions Available:\n1. Dataset1 Vs Dataset2\n2. Dataset1 Vs Dataset3\n";
        cout << "3. Dataset2 Vs Dataset3\n";
        int integer;
        cout << "Choose number ";
        cin >> integer;
        string file1;string file2;
        switch (integer){
                case 1:
                      file1   = "Dataset1.txt";
                        file2 = "Dataset2.txt";
                        break;
                case 2:
                      file1   = "Dataset1.txt";
                        file2 = "Dataset3.txt";
                        break;
                case 3:
                      file1   = "Dataset2.txt";
                        file2 = "Dataset3.txt";
                        break;
        }
        bool * bloom1 = Make_file_2grams_BloomFilter(file1.c_str());
        bool * bloom2 = Make_file_2grams_BloomFilter(file2.c_str());
        double similarity = getJaccardCof(bloom1,bloom2);
        cout << "Jaccard Similarity Between files: \n"<<similarity*100<<'\n';
        system("pause");
        delete [] bloom1;
        delete [] bloom2;
        // cout << Number of 2 grams global_count;
        return 0;
}