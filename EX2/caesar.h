#ifndef CAESAR_H 
#define CAESAR_H 
#include "encryption.h"

class Caesar: public Encryption
{   //encode a string (name) using the caesar cipher
    public:
        Caesar(int);
        bool isEncrypted(string) override;
        
    private:
        //helper for isEncrypted function
        bool helperSearch(const string nme);
        
};

#endif