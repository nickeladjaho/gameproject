#include "Gamer.h"
#include "Manager.h"
#include "caesar.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iterator> 
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <unistd.h>
using namespace std;

//protoypes of functions to processs the game
void playGame(vector<Gamer>&, Gamer&, string&, vector< vector<float> >&);
void gamerMainMenu(Gamer&, vector<Gamer>&, vector< vector<float> >&, string&,
                    Encryption*);
string queryUsertype(string&, int&);
void openFile(ifstream&, string);
void makeGamersObjs(vector<Gamer>&, ifstream&, vector<int>&,
                    vector<float>&, vector<float>&, vector<float>&,
                    Encryption*);
void makeManagerObjs(vector<Manager>&, ifstream&, Encryption*);
void getUserlevel(string&);
int getRandInt(const int&,const int&);
void gamerGuesses(int&, const int&, const int&, const int&, const string);
bool isManager(vector<Manager>&, string&);
void capitilize(string& s);
void gameMenuOpts();
bool isGamer(vector<Gamer>&, string&);
void oldGamer(vector<Gamer>&,int&, Gamer&);
void printGamerStats(vector<Gamer>&,float, Encryption*); 
int gamerHighestScore(Gamer& g, const int);
void topTen(vector< vector<float> >&, string&, float&);
void makeAllScoresVec(vector< vector<float> >&,vector<float>&, 
                      vector<float>&, vector<float>&); 
float strToFloat(string&);
int strToInt(string&);
void printErrMess();
string gameLevel(const int&);
void insertSort(vector<Gamer>&, vector<int>&, vector<int>&);
void ManagerMainMenu(vector<Manager>&, vector<Gamer>&,
                         bool&, Encryption*, Encryption*);
void bossMenuOpts();
void restGame();
void getExtremaIndexs(const string&, int&, int&);
void changeGlevels();
void defaultExtrema();
int countNumGamerScores(vector<Gamer>&, Gamer&);
int getGamerIndex(vector<Gamer>&, Gamer&);
int gamelvlIndex(const string&);
bool toPrintGS();
void sortPrintMaxScores (vector<Gamer>&, vector<int>&, vector <string>&, 
                          Encryption*);
void get_IOfilenames(string&, string&);
template <typename T>
void printPlainId(const T&, Encryption*);
template <typename T>
void newKeyEncrypt(Encryption*, vector<T>&, int); 

//global data structures
//min and max point for each difficulty level
int extremaVals[6]={0,10, 0,100, 0,1000};
//num of games gamer level a gamer has played
vector<int> num_iScores;  
//Binary matrix storing whether a gamer has played a particular game level
vector < vector<int> > gamersPlayed;

int main() {
    // main function directs the game environment.
    bool exitGame= false;

    while(!exitGame) {
        string gFileName, mFileName;
        string usertype, username, userNature; int regNo; int gindex;
            
        int defaultKey=10;
        Encryption* gamersEncoded; Encryption* managersEncoded;
        gamersEncoded = new Caesar(defaultKey);  
        managersEncoded = new Caesar(defaultKey);

        vector<Gamer> gamers; 
        vector<float> easyScores; vector<float> medScores; vector<float> hardScores;
        vector< vector<float> > allScores; 
        vector<Manager> managers;
        ifstream gamers_txt_file;
        ifstream managers_txt_file;

        get_IOfilenames(gFileName, mFileName);
        openFile(gamers_txt_file, gFileName);
        //note assumption (based on assignment instructions) on the struture
        //of the gamers and managers input file.
        //file processing functions makeGamerObj & makeManager 
        //assumes similar structure to example gamers.txt & managers.txt files.
        makeGamersObjs(gamers, gamers_txt_file, num_iScores, 
                        easyScores, medScores, hardScores, gamersEncoded); 
        makeAllScoresVec(allScores, easyScores, medScores, hardScores);
        openFile(managers_txt_file, mFileName);
        makeManagerObjs(managers, managers_txt_file, managersEncoded);
        usertype = queryUsertype(username, regNo);
        //divergence in program menus
        if(usertype=="Gamer") {
            gamersEncoded->encrypt(username);
            Gamer currGamer = Gamer(username, regNo);
            if(isGamer(gamers, username)) {  
                gindex = getGamerIndex(gamers, currGamer);
                currGamer = gamers[gindex];
                userNature = "Historic";
            } 
            else {
                userNature = "New";
                gamers.push_back(currGamer);
                gamersPlayed.push_back(vector<int>(3, 0));
                cout<<"Welcome new game user."<<endl;
            }
            gamerMainMenu(currGamer, gamers, allScores, userNature, gamersEncoded);
        }
        else {
            managersEncoded->encrypt(username); 
            Manager first(username, regNo); 
            auto it = managers.insert(managers.begin(), first);  
            ManagerMainMenu(managers, gamers, exitGame, managersEncoded, gamersEncoded);
        }
    }

    return 0;    

}

void get_IOfilenames(string& f1, string& f2) { 
  //get the required three file names from the user
  cout<<"Enter the name of the gamers text file you want the program to use: "<<endl;
  cin>>f1;
  cout<<"Enter the name of the managers text file you want the program to use: "<<endl;
  cin>>f2;
}


string queryUsertype(string& nme, int& regNum) {
    //find out if the user is a gamer or manager
    string regNoStr;
    string usertype;
    do {         
        cout<<"Are you a Gamer or Manager?"<<endl;
        cout<<"Please enter your selection: "<<endl;
        cin>>usertype; 
        capitilize(usertype);
    } while ((usertype!="Gamer") && (usertype!="Manager"));

    cout<<"Enter your username "
    "[Hint: username maybe your (space-seperated, capitalized first letters, name components) full name]: ";
    cout<<endl;
    cin.ignore();
    getline(cin, nme); 
    cout<<"Enter a four-digit integer as your registration number."<<endl; 
    cin>>regNoStr;  regNum = strToInt(regNoStr);
    return usertype;
}

void gamerMainMenu(Gamer& gamePlayer, vector<Gamer>& gamer_collec,
                    vector< vector<float> >& allGScores, string& userNature,
                    Encryption* gamersEncoded) {  
    //main menu functionality for the gamer
    int userOpts; string userOptsStr;
    int count=0; int gindex; 
    float uScore; string uScoreStr;
    bool exitProgram=false; bool shouldPrint=false;
    while(!exitProgram) {
        
        string defaultregNum ="0"; 
        float defaultscore=0;
        gameMenuOpts();
        cin>>userOptsStr; userOpts=strToInt(userOptsStr);
        switch(userOpts) {
            case 1: 
                playGame(gamer_collec, gamePlayer, userNature, allGScores); 
                break;
            case 2: 
                //output the gamer's statistics
                gindex = getGamerIndex(gamer_collec, gamePlayer);
                printPlainId(gamer_collec[gindex], gamersEncoded);
                cout<<gamer_collec[gindex];
                break;
            case 3: 
                //topTen func is required to have these two parameters
                //however, they will not be used as they are not needed to output
                //the top 10 statistics for each difficulty level
               topTen(allGScores,defaultregNum,defaultscore);
               cout<<endl;
               if(toPrintGS()) {
                   cout<<"Enter a score to see all Gamer stats >= to it: "<<endl;
                   cin>> uScoreStr; uScore=strToFloat(uScoreStr);             
                   printGamerStats(gamer_collec, uScore, gamersEncoded);
                }
                break;
            case 4:
                exitProgram=true;
                delete gamersEncoded;
                cout<<"Exiting game. Bye."<<endl;               
                exit(1);
            default:
                printErrMess();
            
        }
    }

} 


void openFile(ifstream& f, string filename) {
    //open gamers and managers txt files
    f.open(filename);
    if(!f) {
        cout << "Error. Input file "<<filename<<" could not be opened." <<endl;
        cout << "The program is terminated";
        exit(1);
    }
}

void makeGamersObjs(vector<Gamer>& gamers_collec, ifstream& g_txt_info, vector<int>& num_iScores,
                    vector<float>& easyScores, vector<float>& medScores, vector<float>& hardScores,
                    Encryption* gamersEncoded) {
    // Creates encrpyted (name) gamer objects from gamers txt file, and saves them in a vector.
    // Also stores diff level scores in a the appropiatly named vector.
    int count_numiScores=0;
    vector<string> strVec;
    string line, word, name;
    int u=0;
    while(getline(g_txt_info, line)) {
        istringstream iss(line);
        while(iss>>word) {
            strVec.push_back(word);
        }
        //each line of the txt file is said to have structure
        //regNo fName sName scores* (1-3 scores)
        name = strVec.at(1) + " " + strVec.at(2);
        gamersEncoded->encrypt(name);
        Gamer g = Gamer(name, strToInt(strVec.at(0))); 
        //indicate if each read gamer has played the game at a diff level
        //by using the binary matrix gamesPlayed
        gamersPlayed.push_back(vector<int>(3, 0));  
        for(int i=0, k=3; k<strVec.size(); i++, k++) {
            g.addScore(strToFloat(strVec.at(k)), gameLevel(i));
            //gamer has a score for a this game diff level
            gamersPlayed[u][i]=1;
            count_numiScores++;
           //use posisiton of score in text file to store scores in correct vector
           if(k==3) { easyScores.push_back(strToFloat(strVec.at(k))); }
           else if(k==4) {medScores.push_back(strToFloat(strVec.at(k))); }
           else if(k==5) {hardScores.push_back(strToFloat(strVec.at(k)));}
        }
        u++;
        num_iScores.push_back(count_numiScores);
        gamers_collec.push_back(g);
        strVec.clear();
        count_numiScores=0;
    }

    g_txt_info.close();
}

bool isGamer(vector<Gamer>& gamer_collec, string& username) {
    //find if the current user is a gamer and if so,
    //gets index of thus user's gamer instance in gamer_collec

    for(auto g: gamer_collec) {
        if(username==g.getName()) {
            return true;
        }
    }
    return false;
}

void makeAllScoresVec(vector< vector<float> >& allScores,vector<float>& eScores, 
                      vector<float>& mScores, vector<float>& hScores)  {
    //stores the diff level scores in a vector.
    allScores.push_back(eScores); 
    allScores.push_back(mScores); 
    allScores.push_back(hScores);     
}

float strToFloat(string& s) {
    return stof(s);
}

int strToInt(string& s) {
    return stoi(s);
}

void makeManagerObjs(vector<Manager>& manager_collec, ifstream& m_txt_info,
                      Encryption* bossesCiper) {
    // Creates Manager objects from managers txt file, and saves them in a vector.
    vector<string> strVec;
    string line, word, name;

    while(getline(m_txt_info, line)) {
        istringstream iss(line);
        while(iss>>word) {
             strVec.push_back(word);
        }
        //each line of the txt file is said to have structure
        //regNo fName sName
        name = strVec.at(1) + " " + strVec.at(2);
        bossesCiper->encrypt(name);
        Manager m = Manager(name, stoi(strVec.at(0)));
        
        manager_collec.push_back(m);
        strVec.clear();
    }
    m_txt_info.close();
}

void playGame(vector<Gamer>& gamers, Gamer& currGamer, string& userNature, vector< vector<float> >& allGScores) {
    //directs the playing of the defined guessing game
    int randNum, gamer_score; int glevelindex=0;
    string user_Gamelevel;
    getUserlevel(user_Gamelevel);
    int minP, maxP;
    getExtremaIndexs(user_Gamelevel, minP, maxP);
    randNum = getRandInt(minP, maxP);
    /* you may want to remove below commented line to view rand num 
     * as playing so you can "choose" your gamescore
     * cout<<"Random num is: "<<randNum<<endl;
     */
    gamerGuesses(gamer_score, randNum, minP, maxP, user_Gamelevel);
    int lvlIndex = gamelvlIndex(user_Gamelevel);
    allGScores.at(lvlIndex).push_back(gamer_score);
    
    if (userNature == "New") {
        //update gamers vectors to refelect new gamer stats
        gamersPlayed.pop_back();
        vector<int> boolrow(3, 0);
        boolrow.at(lvlIndex)=1;
        gamersPlayed.push_back(boolrow);
        //add game score to new gamer
        gamers.back().addScore(gamer_score, user_Gamelevel);
        num_iScores.push_back(1);
        userNature = "Historic";
    }
    else {
        //update a non-new gamer's stats
        int currGindex = getGamerIndex(gamers, currGamer);
        int difflvl = gamersPlayed.at(currGindex).at(lvlIndex);
        //only update game levels played if not played before
        if((difflvl==0) && num_iScores.at(currGindex) < 3) {
            gamersPlayed.at(currGindex).at(lvlIndex)=1; 
            num_iScores.at(currGindex)++;
        }
        else {
            //remove old game score at that diff lvl from vector of scores
            float rmscore = gamers[currGindex].getHighestScore(user_Gamelevel);
            auto it = find(allGScores.at(lvlIndex).begin(), allGScores.at(lvlIndex).end(), rmscore);
            allGScores.at(lvlIndex).erase(it);          
        }
        gamers[currGindex].addScore(gamer_score, user_Gamelevel);
    }
}


void printGamerStats(vector<Gamer>& gamers_collec, float score, Encryption* gamersCoded) {
    //required function to print sorted gamer stats for those 
    //with a score greater than the given paramater score;
    int scoresIndex=0; int gp_index=0;
    vector<Gamer> best; 
    vector<int> maxScores;
    //track best element indexs in gamePlayed vector;
    vector<int> gpIndexs;
    vector <string> names;
    int gamerHScore;
    for(auto g: gamers_collec) {
        gamerHScore = gamerHighestScore(g, gp_index);
        
       if(gamerHScore >= score) { 
           best.push_back(g); names.push_back(g.getName());
           gpIndexs.push_back(num_iScores.at(gp_index));
           maxScores.push_back(gamerHScore);
        }
        scoresIndex++;
        gp_index++;   
    }
    if(!best.empty()) {
        sortPrintMaxScores(best, maxScores, names, gamersCoded); 
    }

    else {
        cout<<"No gamers found with score greter than or equal to " 
              "the given score."<<endl;
    }

}

void sortPrintMaxScores (vector<Gamer>& best, vector<int>& maxScores, vector <string>& names, 
                          Encryption* gamersCoded) {
    //helper function for required printGamerstats 
    //sorts games collec for those gamers >= score given
    vector<int> cpyMS(maxScores);
    sort(maxScores.begin(), maxScores.end());
    //eliminste duplicares with 0 marker
    string readPlayer = "0";
    for(int i=maxScores.size()-1; i>=0; i--) {
        for(int j=0; j < best.size(); j++) {
            if( (maxScores.at(i) == cpyMS.at(j)) && (names.at(j)!=readPlayer) ) {
                names.at(j)=readPlayer;
                printPlainId(best.at(j), gamersCoded);
                cout<<best.at(j)<<endl;
                break;
            }
        }
    }
}

int gamerHighestScore(Gamer& g, const int rowIndex) {
    //helper for required printGamerstats function.
    //gets a gamer's highestest score irrespective of difficulty. 
    vector <float> iScores;
    for(int i =0; i < 3; i++) {
        //only check if user has played a game at a certain diff level
        if(gamersPlayed.at(rowIndex).at(i)==1) {
            iScores.push_back(g.getHighestScore(gameLevel(i)));
        }
    }
    if(!iScores.empty()) {
        sort(iScores.begin(), iScores.end());
        return iScores.back();
    }
    else{ return -1; }
}

void getUserlevel(string& game_level) {
    //User chooses game difficulty 
    do {
        cout<<"Type in the game difficulity : "<<endl;
        cout<<"Easy -> random number between "<<extremaVals[0]<<"-"<<extremaVals[1]<<endl;
        cout<<"Medium -> random number between "<<extremaVals[2]<<"-"<<extremaVals[3]<<endl;
        cout<<"Hard -> random number between "<<extremaVals[4]<<"-"<<extremaVals[5]<<endl;
        cin>>game_level;
        capitilize(game_level);
    } while ((game_level!="Easy") && (game_level!="Medium") && (game_level!="Hard"));
}

int getRandInt(const int& min, const int& max) {
    //return random num in range (min, max)
    srand (time (0));
    int rawrand = rand();
    return rawrand%(max-min+1) + min;
}

void gamerGuesses(int& gamerScore, const int& randInt, const int& minPoint, const int& maxPoint, 
                    const string level) {
    //As per the game rules, this section of the game
    //keeps asking the gamer for a guess number until he reaches the right answer.
    int guess;
    string guessStr;
    int count=0;
    gamerScore=1;
    int err;
    bool found=false; 
    while(!found) {
        do {
            if(count >= 1)  {printErrMess();}
            cout<<"Enter your guess for the random int."<<endl;
            cout<<"Range is integers in the closed interval ["<<minPoint<<","<<maxPoint<<"] :"<<endl;
            //read to string to guard against infinite loop 
            //caused by non-numeric characters input to an int type
            cin>>guessStr;
            guess=strToInt(guessStr);
            count++;
            cout<<endl;
        } while (guess < minPoint || guess > maxPoint);
            //reset count for err messages for next guesses
            count=0;
            //game cases for the gamer's guesses at the random number
            if(guess==randInt) {
                found=true;
                cout<<"Yay! you guessed the right number!"<<endl;
                cout<<"Your score (number of attempts) is " <<gamerScore<<endl;
                cout<<endl;
                break;
            }
            else if( (guess<randInt) && (level!="Easy") ) { 
                if(guess==0 || randInt==0) { cout<<"Your guess < random number"<<endl;} 
                else { 
                    err = floor(abs(randInt)/abs(guess));
                    cout<<"Your guess is smaller than the random number by a factor of (approx) "<<err<<endl;
                }   
            }
            else if( (guess>randInt) && (level!="Easy") ) {
                if(guess==0 || randInt==0) { cout<<"Your guess > random number."<<endl;} 
                else { 
                    err = floor(abs(guess)/abs(randInt));
                    cout<<"Your guess is greater than the random number by a factor of (approx) "<<err<<endl;
                }
            } 
            else{
                cout<<"No help for Easy level. Sorry! Try Again"<<endl;
            }           

        gamerScore++;
        cout<<endl;
    }
}

void topTen(vector< vector<float> >& allScores, string& regNum, float& score) {
    //second required function
    //to print to the screen the highest top 10 scores for each difficulty level
    //ascending sort each diff level score vector
    //note top10 only be possible if ten gamers have played each diff level
    sort(allScores.at(0).rbegin(), allScores.at(0).rend());
    sort(allScores.at(1).rbegin(), allScores.at(1).rend());
    sort(allScores.at(2).rbegin(), allScores.at(2).rend());
    cout<<"Top 10 Highest Score"<<endl;
    int TEN=10; 
    for(int k=0; k<allScores.size(); k++) {
        cout<<gameLevel(k)<<":";
        for(int j=0;(j< allScores.at(k).size()) && (j< TEN); j++) {
            cout<<" "<<allScores.at(k).at(j); 
        }
        cout<<endl;
    }
    
}

bool isManager(vector<Manager>& manager_collec, string& name) {
    //Finds if we have a manager saved in our vector of managers
    bool nameFound= false;
    for(auto m: manager_collec) {
        if(m.getName() == name) {
            nameFound=true;
            return nameFound;
        }
    }
    return false;
}


void capitilize(string& s) {
    //helper to ensure equality holds for lowcased strings
    s[0] = toupper(s[0]);
}

void printErrMess() {
    cout<<"User input is incorrect. Please try again."<<endl;
}

void gameMenuOpts() {
    //gamer menu options
    cout<<"Choose the number of the option you want."<<endl;
    cout<< "1) Play game"<<endl;
    cout<< "2) Your statistics"<<endl; 
    cout<< "3) Display top 10 scores for each game level"<<endl;
    cout<< "4) Exit game     " <<endl;
    cout<<">> ";
}

string gameLevel(const int& k) {
    //helper to map int to game difficulty level 
    string game_levels[3] = {"Easy", "Medium", "Hard"};
    return game_levels[k];
}

void ManagerMainMenu(vector<Manager>& bossPeeps, vector<Gamer>& gamers, bool& exitGame, 
                      Encryption* managersCoded, Encryption* gamersCoded) {
    //main menu for the manager
    int BossOpts; string BossOptsStr;
    string usertype="Manager"; 
    int userOpts; string userOptsStr;
    int count =0;  
    bool exitMenu=false;

    while(!exitMenu) {
        bossMenuOpts();
        cin>>userOptsStr;
        userOpts = strToInt(userOptsStr);
        if(userOpts==1) {
            //reset global vars
            defaultExtrema();
            num_iScores.clear();
            gamersPlayed.clear();
            delete gamersCoded; delete managersCoded;
            cout<<endl; cout<<"Game has reset."<<endl; cout<<endl;
            return;
        }
        else if(userOpts==2) { 
            //change game levls
            char gameplay;
            changeGlevels();
            cout<<"Enter y to let a gamer play now with new difficulty " 
                    "level. Press any other key otherwise."<<endl;
            cin>>gameplay;
            if(gameplay=='y') {
                exitMenu=true;
                num_iScores.clear();
                gamersPlayed.clear();
            }
        }
        else if(userOpts==3) {
            //rencrypt gamers and managers with new encrytion key 
            string newKstr; int newKey; 
            cout<<endl; cout<<"Enter your new key: "<<endl;
            cin>>newKstr; newKey=strToInt(newKstr);
            newKeyEncrypt(gamersCoded, gamers, newKey);
            newKeyEncrypt(managersCoded, bossPeeps, newKey);
        }    
        else if(userOpts==4) {
                string name, regNo;
                cout<<"Enter first and last name of the new manager entry"<<endl;
                cin.ignore();
                getline(cin, name);
                cout<<"Enter a four digit regNo: ";
                cin>>regNo;
                managersCoded->encrypt(name);
                Manager m = Manager(name, strToInt(regNo));
                bossPeeps.push_back(m);
                cout<<endl; cout<<"New Manager created"<<endl;
        }
        else if(userOpts==5) {
                //The exit flag vars changes are not strictly required for an exit.
                //Done for extra code clarity.       
                exitMenu=true; exitGame=true;               
                exit(1);             
        }
        else {
            printErrMess();
        }
        cout<<endl;
    }

    
    
}

void bossMenuOpts() {
    //manager menu options
    cout<<"Manager Menu"<<endl;
    cout<<"Choose the number of the option you want."<<endl;
    cout<< "1) Reset game"<<endl;
    cout<< "2) Change game levels"<<endl; 
    cout<< "3) Change security key" <<endl;
    cout<< "4) Add another manager" <<endl;
    cout<< "5) Exit game" <<endl;
    cout<<">> ";
}

void getExtremaIndexs(const string& glevel, int& minP, int& maxP) {
    //associate game difficulty level with min/max score interval
    if(glevel=="Easy") {
        minP=extremaVals[0]; maxP=extremaVals[1];
    }
    else if(glevel=="Medium") {
        minP=extremaVals[2]; maxP=extremaVals[3];
    }
    else{
        minP=extremaVals[4]; maxP=extremaVals[5];
    }
}

void defaultExtrema() {
    //reset to default scores for easy, med and hard game levels
    int SIZE=6;
    for (int i = 0, j=1; i <SIZE; i++){
        if((i%2)==0) {
            extremaVals[i]=0;
        }
        else {
            extremaVals[i]=pow(10, j);
            j++;
        }
    }
}

void changeGlevels() {
    //manager can then select to change the range of each level of difficulty
    //They can change either the minimum or/and the maximum values.
    int num_glevls=3; 
    int minP, maxP;
    for(int i=0, j=0; i < num_glevls; i++, j+=2) {
        cout<<"Input the min value for "<<gameLevel(i)<<" level"<<endl;
        cout<<"Current min is "<< extremaVals[j]<<endl; cout<<">> ";
        cin>>minP; 
        extremaVals[j] = minP;
        cout<<"Input the max value for "<<gameLevel(i)<<" level"<<endl;
        cout<<"Current max is "<< extremaVals[j+1]<<endl; cout<<">> "; 
        cin>>maxP;
        extremaVals[j+1] = maxP;
    }
    
}

int getGamerIndex(vector<Gamer>& gamer_collec, Gamer& currGamer) {
    //get index of gamer instance in gamer collec
    int gindex=0;
    string nme = currGamer.getName();
    for(auto g: gamer_collec) {
        if(nme==g.getName()) {
            break;
        }
        else { gindex++; }
    }
    return gindex;
}

int gamelvlIndex(const string& lvl) {   
    //helper function to map game diff level to its int code
    //gmelvls.insert( pair<string, string>("Easy", 0) );                         
    map<string, float> gmelvls;
    gmelvls.insert(pair<string, float>("Easy", 0)); 
    gmelvls.insert(pair<string, float>("Medium", 1));
    gmelvls.insert(pair<string, float>("Hard", 2));                         
    return gmelvls[lvl]; 
}

bool toPrintGS() {
    //determine if gamer wants to view gamers stats
    string ans;
    cout<<"Do you want to output sorted (by highest score) Gamers stats"<<endl;
    cout<<"That is max, min, avg scores of all gamers > your soon to be chosen game score: "<<endl;
    cout<<"Enter y to output these gamers stats. Otherwise, enter no: "<<endl;
    cin>>ans;
    if(ans=="y" || ans =="Y") {
        return true;
    }
    else {
        return false;
    }
}

template <typename T>
void printPlainId(const T& curr, Encryption* coded) {
    //decrypt gamer/manager's name & regNo before printing to screen
    T currcpy = curr;
    string plainNme = currcpy.getName();
    coded->decrypt(plainNme);
    currcpy.changeName(plainNme);
    cout<<"Name: "<<currcpy.getName()<<", "<<"Reg No: "<<currcpy.getRegNo()<<", ";
    
}

template <typename T>
void newKeyEncrypt(Encryption* codedOld, vector<T>& users, int newKey) {
    //update encryption with a new key given by a manager
    string dnme;
    Encryption* codeNew; 
    codeNew = new Caesar(newKey); 
    //encrypt all gamers/managers with new key
    //gamersEncoded->encrypt(name);
    for(int i=0; i < users.size(); i++) {
         dnme = users[i].getName();
         codedOld->decrypt(dnme);
         //encypt gamer/manager with new key
         codeNew->encrypt(dnme);
         users[i].changeName(dnme);
    }
    codedOld->changeKey(newKey);
    delete codeNew;
} 
