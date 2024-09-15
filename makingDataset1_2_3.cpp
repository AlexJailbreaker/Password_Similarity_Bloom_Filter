//Compilation Command : g++ makingDataset1_2_3.cpp -o first &&.\first.exe

#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
//Each password is 8 characters
//contains 104 passwords
// 4 passwords of alphabit
//Lower case letters
//104/4 = 26 
//a = ASCI =  97
//z = ASCI = 122 
int main(){
        ifstream in("../rockyou.txt");
        if (!in){
                cout << "Error opening file";
                return 1;
        }
        int i = 0;
        string buffer;
        char char_buffer ;
        int letter_range= 97;//asci for a 
        ofstream out("Dataset1.txt");
        while (i!=104){
                in>>buffer;
                char_buffer = buffer[0];
                if (char_buffer == letter_range){
                       if (buffer.length()== 12){
                                out << buffer << '\n';
                                i+=1;
                                if (i%4==0){
                                   letter_range+=1;     
                                }
                        } 
                } 
        }
        in.close();
        out.close();
}