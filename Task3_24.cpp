#include <iostream>
#include <string>

using namespace std;

// Encrypt a single character using the Affine Cipher
char encryptChar(char ch, int a, int b, const string &alphabet) {
    int m = alphabet.length();
    int index = alphabet.find(ch);
    if (index != string::npos) { //check if the index exists in the alphabet
        int encryptedIndex = (a * index + b) % m;
        return alphabet[encryptedIndex];
    }
    return ch; 
}
//Encrypt the whole message by calling encryptChar
string encryptMessage(const string &message, int a, int b, const string &alphabet) {
    string encryptedMessage = "";
    for (char ch : message) {
        encryptedMessage += encryptChar(ch, a, b, alphabet);
    }
    return encryptedMessage;
}

int main() {
    // Samoan alphabet 
    string alphabet = " AEIOUFGLMNPSTVHKR";

    // Message to encrypt
    string message = "OU TE INU KOFE I LE TAEAO";

    // Affine cipher keys
    int a = 11;  
    int b = 17; 

    string encryptedMessage = encryptMessage(message, a, b, alphabet);
    cout << "Original Message:" << message << endl;
    cout << "Encrypted Message:" << encryptedMessage << endl;

    return 0;
}
