#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int modInverse(int a,int m){
    int t1=0,t2=1,t,r1=m,r2=a; 
    while(r2!=0){ // applying extended euclidean algorithm to get t1 which is the mod inverse we are looking for
        int q,r;
        q=r1/r2;
        r=r1%r2;
        t=t1-t2*q; 
        r1=r2;
        r2=r;
        t1=t2;
        t2=t;
    }
    if(r1==1){
        while(t1<0){ // mod inverse must be bewteen 0 & m-1
            t1=t1+m;
    }
        return t1%m; // returning mod inverse of a and m with gcd of 1
    }
    else return -1; // not coprime so no mod inverse
}
int search(string s,char c){
    for(int i=0;i<s.length();i++){
        if(s[i]==c) return i;
    }
    return -1;
}

// Function to decrypt the Affine ciphered message
string affineDecrypt(string cipherText, int a, int b, int m,string alphabet) {
    string decryptedText = "";
    int a_inv = modInverse(a, m);

    if (a_inv == -1) {
        cout << "Modular inverse of 'a' does not exist!" << endl;
        return "";
    }

    for (char c : cipherText) {
        int L;
        L=(a_inv * (search(alphabet,c)-b))%m;
        while(L<0){
                L=L+m;
            }
        decryptedText += char(L+'A');
    }
    return decryptedText;
}

int main() {
    string cipherText,alphabet;
    int a, b;
    cout << "Enter the affine ciphered message: ";
    getline(cin, cipherText);
    cout << "Enter a: ";
    cin >> a;
    cout << "Enter b: ";
    cin >> b;
    cin.ignore();
    cout << "Enter Alphabet: ";
    getline(cin,alphabet);
    alphabet=" "+alphabet;
    int m = alphabet.size();  

    // Check if 'a' and 'm' are coprime
    if (modInverse(a, m) == -1) {
        cout << "Key 'a' must be coprime with the alphabet size!" << endl;
        return 1;
    }

    // Decrypt the message
    string decryptedMessage = affineDecrypt(cipherText, a, b, m,alphabet);

    if (!decryptedMessage.empty()) {
        cout << "The decrypted message is: " << decryptedMessage << endl;
    }

    return 0;
}
