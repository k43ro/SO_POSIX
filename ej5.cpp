#include "Functions.h"
#include <iostream>
using namespace std;
#include <cstring>


int main(){
    constexpr int MAX = 256;
    char str[MAX];

    cout << "Introduzca una cadena: ";
    
    Functions::read(str);

    cout << "Cadena normal: |" << str << "|" << endl;

    strcpy(str, Functions::strtrim(str));

    cout << "Cadena reducida: |" <<  str << "|" << endl;
}