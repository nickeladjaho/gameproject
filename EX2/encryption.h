/**
 * Encryption abstract class
 */

#ifndef _ENCRYPTION_H
#define _ENCRYPTION_H

#include <string>
#include <list>
#include <map>
using namespace std;

class Encryption
{
        public:

                /*
                 * Constructor. Stores the key for encryption
                 */
                Encryption(int k);

                //pure virtual function for filter
                virtual bool isEncrypted(string enc_message)=0;

                /*
                 * encrypts a message
                 */
                void encrypt(string&);

                /*
                 * decrypts a message
                 */

                void decrypt(string&);
                 
                /*
                 *  Change key for encryption
                 */
                void changeKey(int);
                        
                /*
                 * define virtual destructor to avoid memory leaks
                 * when deleting pointer in main program
                 */
                virtual ~Encryption ();

        private:
                //key for the encryption of a message
                int key;       
                
        protected:
                // stores plaintxt usernames
                map<string, string> userNames;
        
};

 #endif
