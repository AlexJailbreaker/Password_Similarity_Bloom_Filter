//Compilation Command : g++ makingBeta1_2_3.cpp SHA256.cpp md5.cpp -o first &&.\first.exe
#include <iostream>
#include <sstream>
using namespace std;
#include "SHA256.h"
#include "md5.h"
#include <string>
const int BLOOM_FILTER_SIZE = 1000;


string generateSHA256(string plaintext){
    stringstream sha1;
    SHA256 sha;
    sha.update(plaintext);
    std::array<uint8_t, 32> digest = sha.digest();
    sha1 << SHA256::toString(digest) << std::endl;
    return sha1.str();
    // cout << sha1.str() << endl;
}

unsigned long long hashSHA256_to_index(const std::string& hexStr) {
    unsigned long long result = 0;
    for (size_t i = 0; i < hexStr.length(); ++i) {
        result = result * 16 + (hexStr[i] >= '0' && hexStr[i] <= '9' ? hexStr[i] - '0' :
                                hexStr[i] >= 'a' && hexStr[i] <= 'f' ? hexStr[i] - 'a' + 10 : 0);
    }
    return result;
}

unsigned long long hashMD5_to_index(const std::string& hexStr) {
    unsigned long long result = 0;
    for (size_t i = 0; i < hexStr.length(); ++i) {
        result = result * 16 + (hexStr[i] >= '0' && hexStr[i] <= '9' ? hexStr[i] - '0' :
                                hexStr[i] >= 'a' && hexStr[i] <= 'f' ? hexStr[i] - 'a' + 10 : 0);
    }
    return result;
}


bool* getAtom(string plaintext){ // Atom = Bloom filter
  // Bloomfilter in heap must be initialized to zero to remove junck values
  bool * bloomfilterAtom = new bool[BLOOM_FILTER_SIZE]{0};
  
  string plaintext_withspace_prefix = plaintext.insert(0,"_");
  
  string finalone = plaintext_withspace_prefix.insert (plaintext_withspace_prefix.size(),1,'_');
  
  for (int i = 0 ; i <finalone.size()-1 ; i++ ){
    // Testing: cout << finalone.substr(i,2)<<endl;
    string SHA256Digest = generateSHA256(finalone.substr(i,2));
    unsigned long long SHA256Digest_index = hashSHA256_to_index(SHA256Digest);
    string MD5Digest = md5(finalone.substr(i,2));
    unsigned long long MD5Digest_index = hashMD5_to_index(MD5Digest);
    //RUN 15 hash functions
    int bloom_index;
    for (int y = 0 ; y < 15 ; y++){
      bloom_index = (MD5Digest_index+(y*SHA256Digest_index))%BLOOM_FILTER_SIZE;
      bloomfilterAtom[bloom_index]= 1;
    }
  }
  return bloomfilterAtom;
  //For each iteration, hence for each bigram run 15 hash functions
  /*_O
    OM
    MA
    AR
    RR
    R_*/
  // The functions g and h  will  be MD5 and SHA256. 
  // H(b) = g(b) + j*h(b) where j is from 0 to 14 (15 hash functions)
}

#include <fstream>
#include <sstream>
int main(){
        string array[104][2];
        string datasetFileName = "Dataset3.txt";
        string BetaFileName =  "Beta3.txt";
        ifstream in (datasetFileName.c_str());
        string buffer;
        int counter = 0;
        while (!in.eof()){
          in >> buffer;
          array[counter][0] = buffer;
          
          bool * atom1 = getAtom(buffer); // Bloom filter
          stringstream ss;
          for (int i = 0 ; i <1000 ; i++){
                  ss << *(atom1+i);
          }
          array[counter][1] = ss.str();
          counter+=1;
        }
        in.close();
        delete [] atom1;
        system("pause");
        ofstream out(BetaFileName.c_str());
        for (int i = 0 ; i < 104 ; i++){
          out << array[i][0] << " " <<array[i][1] << '\n'; 
        }
        out.close();
}       