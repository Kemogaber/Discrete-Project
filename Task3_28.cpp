#include <iostream>
#include <string>

using namespace std;

//encrypt a single character function
char encryptChar(char ch, int a, int b, const string &alphabet) {
    int length = alphabet.length();
    int index = 0;
    bool check = false;

for(int i = 0; i<length;i++){ //loop to make sure character exists in alphabet
    if(alphabet[i]==ch){
        index = i;
        check = true; }
}

    if (check) { //checking if find function returned no position 
        int new_index = (a * index + b) % length;
        return alphabet[new_index];
    }
    return ch; //if didn't exist -> return character itself without modification
}

//encrypt the message using encryptChar function
string encryptMessage(const string &message, int a, int b, const string &alphabet) {
    string final = "";
    for (char ch : message) {
        final += encryptChar(ch, a, b, alphabet);
    }
    return final;
}

int main() {
    //samoan alphabet --> length : 18 (17 characters + space ( index 0 ) )

    string alphabet = " AEIOUFGLMNPSTVHKR"; // changing alphabet to be generic and fit different cases


    //example 
    string message = "OU TE INU KOFE I LE TAEAO";

    //cipher keys
    int a = 11;  
    int b = 17; 

    string final = encryptMessage(message, a, b, alphabet);
    cout << "Original Message:" << message << endl;
    cout << "Encrypted Message:" << final << endl; //printing final results

    return 0;
}
