#include "Functions.h"
#include <iostream>
using namespace std;
#include <fstream>
#include <cstring>

int main(){
    char * tests[256];
    char buffer[256];
    char * in;
    char * out;
    ifstream file("ej6_mandatos.txt");


    int i = 0;
    if(file.is_open()){
        while (i < 256 && file.getline(buffer,256)){
            tests[i] = new char[256];
            strcpy(tests[i],buffer);
            i++;
        }
    }
    
    for (int n = 0; n < i; n++){
        strcpy(buffer,tests[n]);
        getredir(buffer, in, out);
        cout << "|" << (buffer != NULL ? buffer : "NULL") << "|";
        cout << (in != NULL ? in : "NULL") << "|";
        cout << (out != NULL ? out : "NULL") << "|" << endl;
    }
    getredir(NULL, in, out);
}