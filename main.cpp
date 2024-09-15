//Compilation Command : g++ main.cpp SHA256.cpp md5.cpp -o main &&.\main.exe 
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <iomanip>
using namespace std;
#include "SHA256.h"
#include "md5.h"

const int BLOOM_FILTER_SIZE = 1000;

const float REJECTION_THRESHOLD =  0.4569;

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
// It iterates through each character of the hexadecimal string, converting it 
//into its decimal equivalent and accumulating it into result using base-16 arithmetic. 
// Pupose : Convert MD5 Hash to integer index
unsigned long long hashMD5_to_index(const std::string& hexStr) {
    unsigned long long result = 0;
    for (size_t i = 0; i < hexStr.length(); ++i) {
        result = result * 16 + (hexStr[i] >= '0' && hexStr[i] <= '9' ? hexStr[i] - '0' :
                                hexStr[i] >= 'a' && hexStr[i] <= 'f' ? hexStr[i] - 'a' + 10 : 0);
    }
    return result;
}

//An atom is a bloom filter of size 1000
//Must deallocate memory
bool* getAtom(string plaintext){
  // Bloomfilter in heap must be initialized to zero to remove junck values
  bool * bloomfilterAtom = new bool[BLOOM_FILTER_SIZE]{0};
  //This insert _ before string
  string plaintext_withspace_prefix = plaintext.insert(0,"_");
  //This insert _ after string
  string finalone = plaintext_withspace_prefix.insert (plaintext_withspace_prefix.size(),1,'_');
  // Result _STRING_
  for (int i = 0 ; i <finalone.size()-1 ; i++ ){
    // Testing: cout << finalone.substr(i,2)<<endl;
    string SHA256Digest = generateSHA256(finalone.substr(i,2));
    unsigned long long SHA256Digest_index = hashSHA256_to_index(SHA256Digest);
    string MD5Digest = md5(finalone.substr(i,2));
    //MD5 by Frank Thiolo (thilo@unix-ag.org): http://www.zedwood.com/article/cpp-md5-function
    unsigned long long MD5Digest_index = hashMD5_to_index(MD5Digest);
    //RUN 15 hash functions
    int bloom_index;
    for (int y = 0 ; y < 15 ; y++){
      bloom_index = (MD5Digest_index+(y*SHA256Digest_index))%BLOOM_FILTER_SIZE;
      bloomfilterAtom[bloom_index]= 1;
    }
  }
  return bloomfilterAtom;
  // Very important to prevent memory leaks to deallocate memory from heap
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

// Purpose : helps option 1 to print on console the bloom filter of password
void ProduceBloomFilter_cout_console(string password){
    bool * atom1 =  getAtom(password);
    // iterate throuh bloomfilter and print each value on console
    for (int i = 0 ; i <1000 ; i++){
      cout << *(atom1+i);
    }

}

int main()
{
  while (true){
    cout << "\nPassword Tool Options (1 or 2 or 3):"<< '\n';
    cout << "1 : Produce Bloom Filter" << '\n';
    cout << "2 : Compare similarity between two Password Bloom Filter" << '\n';
    cout << "3 : Analyse password similarity given p and modification into p1 , p2 etc" << '\n';
    cout << "4 : Exit" << '\n';
    int opt;
    cout << "Choose Choice: ";
    cin>>opt;
    string password_op1,p1,p2;
    string bloom1;
    string bloom2;
    bool * p1_atom;
    bool *p2_atom; // An atom is a bloom filter pointer boolen array of size 1000
    double similarity;
    double jaccard_coff;
    switch(opt){
      case 1:
        cout << "Generating Bloom Filter" << endl;
        cout << "Enter password text:(NO SPACE) ";
        cin >> password_op1;
        cout << "Bloom Filter for " << password_op1 << ':'<< endl;
        ProduceBloomFilter_cout_console(password_op1);
        break;
      case 2:
        cout << "Enter 1st BloomFilter (size 1000): ";
        cin >> bloom1;
        cout << "Enter 2nd BloomFilter (size 1000): ";
        cin >> bloom2;
        p1_atom =  bloom_filter_string_to_pointer(bloom1);
        p2_atom =  bloom_filter_string_to_pointer(bloom2);
        similarity = getJaccardCof(p1_atom,p2_atom);
        cout << "\nPassword Similarity index (Jaccard coffient): " << similarity;
        if (similarity>=REJECTION_THRESHOLD){
          cout << "\nThis Bloom filter similarity index high\n";
          cout << "\nResult : Rejection\n";
        }
        else{
          cout << "\nThis Bloom filter similarity index low\n";
          cout << "\nResult : Accepted\n";          
        }

        delete [] p1_atom;
        delete [] p2_atom;// Very important to prevent memory leaks to deallocate memory from heap
        break;
      case 3:
        cout << "Enter new Password: ";
        cin>>p1;
        cout << "Change Password: ";
        cin>>p2;
        p1_atom =  getAtom(p1);//Pointer to bloomFilterAtom is used
        p2_atom = getAtom(p2);//Pointer to bloomFilterAtom is used
        jaccard_coff = getJaccardCof(p1_atom,p2_atom);
        cout << "\nThe Jaccard coefficient : "<< jaccard_coff << '\n';
        if (jaccard_coff>=REJECTION_THRESHOLD){
          cout << "\nThis Password similarity index high\n";
          cout << "\nResult : Rejection\n";
        }
        else{
          cout << "\nThis Password similarity index low\n";
          cout << "\nResult : Accepted\n";          
        }
        delete [] p1_atom; // Very important to prevent memory leaks to deallocate memory from heap
        delete [] p2_atom;
        break;
      case 4:
            system("pause");
            return 0;
            break;
    };
    cin.clear();//Clearing stream if any input errors
    cin.ignore(1000,'\n');
  } // end whille
    cout << '\n';
    system("pause");
    return 0;

}





