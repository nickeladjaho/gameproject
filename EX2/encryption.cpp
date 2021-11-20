#include "encryption.h"
#include <iostream>

//Constructor
Encryption::Encryption(int k) 
{   this->key=k;
    if(key<0) { key=0; } 
}

void Encryption::encrypt(string& txt)  {
    //function performs encryption, which takes a string message (plaintext), 
    //and converts it into a scrambled message (ciphertext)
    //the encryption scheme is the Caesar cipher.
    
    //add new username to private storage map
    string cpytxt(txt);
    userNames.insert(pair<string, string>(cpytxt, cpytxt));
        char space = 32;
    // traverse text 
    for (int i=0;i<txt.length();i++)   { 
        // Encrypt Uppercase letters 
        if (isupper(txt[i])) {
          if(txt[i] != space) { txt[i] = char(int(txt[i]+key-65)%26 +65); }
          else { txt[i] = space; }
        }
    // Encrypt Lowercase letters 
        else {
            if(txt[i] != space) { txt[i] = char(int(txt[i]+key-97)%26 +97); }
            else { txt[i] = space; }
        }
    }
    
}



void Encryption::decrypt(string& txt) {
     //function takes a ciphertext and turns it back into its original plaintext.
    int shift = 26 - key;
    char space = 32;
    // traverse text 
    for (int i=0;i<txt.length();i++) 
    {   char temp = txt[i];
        // apply transformation to each character 
        // Encrypt Uppercase letters 
        if (isupper(txt[i])) {
          if(txt[i] != space) { 
              txt[i] = char(int(txt[i]+shift-65)%26 +65);  
             }
          else { txt[i] = space; }
        }
    // Encrypt Lowercase letters 
        else {
            if(txt[i] != space) { 
                txt[i] = char(int(txt[i]+shift-97)%26 +97); }
            else { 
                txt[i] = space; 
            }
        }
    }
}

void Encryption::changeKey(int newKey) {
    this->key=newKey;
}

// definition of the Encyption (virtual) destructor
Encryption:: ~Encryption() {

}