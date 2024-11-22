#include <iostream>
#include <string>
#include <cctype>
#include <vector>

using namespace std;

// Function to compute the modular inverse of a number 'a' under modulo 'm' using Extended Euclidean Algorithm
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1; // If no modular inverse exists
}

// Function to decrypt the Affine ciphered message
string affineDecrypt(string cipherText, int a, int b, int m) {
    string decryptedText = "";
    int a_inv = modInverse(a, m);

    if (a_inv == -1) {
        cout << "Modular inverse of 'a' does not exist!" << endl;
        return "";
    }

    // Traverse through each character in the cipherText
    for (char c : cipherText) {
        if (isupper(c)) {
            // Decrypt uppercase letters (A-Z)
            decryptedText += char(((a_inv * (c - 'A' - b)) % m + m) % m + 'A');
        }
        else if (islower(c)) {
            // Decrypt lowercase letters (a-z)
            decryptedText += char(((a_inv * (c - 'a' - b)) % m + m) % m + 'a');
        }
        else if (isdigit(c)) {
            // Decrypt digits (0-9)
            decryptedText += char(((a_inv * (c - '0' - b)) % 10 + 10) % 10 + '0');
        }
        else if (ispunct(c) || isspace(c)) {
            // Non-alphabet characters (punctuation, spaces)
            decryptedText += c;
        }
    }

    return decryptedText;
}

int main() {
    string cipherText;
    int a, b;

    cout << "Enter the affine ciphered message: ";
    getline(cin, cipherText);
    
    cout << "Enter the key a (must be coprime with the alphabet size): ";
    cin >> a;
    
    cout << "Enter the key b (shift): ";
    cin >> b;
    
    // The size of the alphabet (26 for letters, 10 for digits, including punctuations will increase the size)
    int m = 26;  // For English alphabet, you can change this for more complex alphabets.

    // Decrypt the message
    string decryptedMessage = affineDecrypt(cipherText, a, b, m);

    if (!decryptedMessage.empty()) {
        cout << "The decrypted message is: " << decryptedMessage << endl;
    }

    return 0;
}
