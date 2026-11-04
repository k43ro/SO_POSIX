#include <iostream>
#include <cstring>
using namespace std;

constexpr int MAX = 10;


int main() {
    char a[MAX + 1];
    char b[MAX + 1];
    cout << "Ingrese una cadena (a): ";

    cin.getline(a, MAX + 1);
    if (cin.fail()){
        cin.clear();
        cin.ignore(256, '\n');
    }
    
    cout << "Ingrese otra cadena (b): ";

    cin.getline(b, MAX + 1);
    if (cin.fail()){
        cin.clear();
        cin.ignore(256, '\n');
    }

    cout << "La cadena a es: " << a << " y tiene " << strlen(a) << " caracteres." << endl;
    cout << "La cadena b es: " << b << " y tiene " << strlen(b) << " caracteres." << endl;
}