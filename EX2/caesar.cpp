#include "caesar.h"

Caesar::Caesar(int k) 
:Encryption(k)
{ 
}

bool Caesar::isEncrypted(string username) {
    //check if a user's name is encrypted
    bool isEncoded;
    isEncoded = helperSearch(username);
    if(isEncoded) {
        return true;
    } 
    return false;
}


bool Caesar:: helperSearch(const string nme) {
    // use map defined in encrytion.h to determine if 
    // the given username is encrypted

    for(auto p: userNames) {
        Encryption::encrypt(p.second);
        if(p.second==nme) {
            return true;
        }
    }
    return false;
}
