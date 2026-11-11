#include "Functions.h"
#include <iostream>
using namespace std;


int main() {
    constexpr int MAX = 256;
    char in[MAX];
    char ** out;

    cout << "Introduzca una cadena: ";
    
    Functions::read(in);

    Functions::strsplit(in, " \t", out);

    for (int i = 0; out[i] != NULL; i++){
        cout << "Trozo " << i << ": " << out[i] << endl;
    }
}