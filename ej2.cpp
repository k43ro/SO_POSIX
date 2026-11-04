#include <iostream>
#include <cstring>
using namespace std;

constexpr int MAX = 10;


int main() {
    char a[MAX];
    char b[MAX];
    cout << "Ingrese una cadena (a): ";
    cin.getline(a, MAX);
    cout << "Ingrese otra cadena (b): ";
    cin.getline(b, MAX);

    cout << "La cadena a es: " << a << "y tiene " << strlen(a) << " caracteres." << endl;
    cout << "La cadena b es: " << b << "y tiene " << strlen(b) << " caracteres." << endl;
}