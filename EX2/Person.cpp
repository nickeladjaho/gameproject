#include "Person.h"
#include <string>
using namespace std;

Person::Person(const string &name)
{   this->name = name;
}

string Person::getName() const
{   return name;
}

void Person::changeName(const string &newName)
{   name = newName;
}
