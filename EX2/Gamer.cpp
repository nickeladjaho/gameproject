#include "Gamer.h"
#include <cctype>
#include <utility>
#include <iostream>
#include <algorithm>
using namespace std;

//non-member function prototypes
float avgScore(const map<string, float>& );
bool compare(pair<string, float> , pair<string, float>);

Gamer::Gamer(const string &name, int regNo)
:Person(name)
{
	this->name=name;
	this->regNo=regNo;
}


int Gamer:: getRegNo() const {
    return this->regNo;
}

 void Gamer:: addScore(float score,string difficulty) {
    // Overwrites duplicate score irrespective of difficulty level.
    scores[difficulty]=score; 
}

float Gamer:: getHighestScore(string difficulty) const {
    //get the score of a game at a particular difficulty level 
    try {
        if(scores.find(difficulty) == scores.end()) {
            throw NoScoreException(name, difficulty); 
        }
        else {                
            return scores.at(difficulty);
        }
    }
    catch (NoScoreException e) {
        cout<<e.uName()<<" has no score (yet) for " <<e.diffLvl()<<" level of difficulty"<<endl;
        return -1;
    }
}

ostream& operator<<(ostream &str, const Gamer &s) {
    //overload << to print a gamer's game stats
    pair <string, float> minScore;  pair <string, float> maxScore;

    if(!s.scores.empty()) {
        float avgscore = avgScore(s.scores);
        minScore = *min_element(s.scores.begin(), s.scores.end(), compare);
        maxScore = *max_element(s.scores.begin(), s.scores.end(), compare);        
        str<<"Avg Score: "<<avgscore<<", "<<"Min Score: "<<minScore.second<<", "<<"MaxScore: "<<maxScore.second;
    }
    else {
        str<<"Has no scores.";
    }
    str<<endl;
    return str;

}

float avgScore(const map<string, float>& scores) {
    //calculate a gamer's average score
    //could use stl accumulate function here
    int sum=0; int n=0;
    for(auto s: scores) {
        sum+=s.second;
        n++;
    }
    return static_cast <float> (sum)/n; 
}

bool compare(pair<string, float> p,pair<string, float> q ) {
    //comparator function for order algorithms on scores map
    return p.second < q.second;
}