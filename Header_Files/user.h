#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "session.h"

enum {REGISTERED_USER, FILE_ERROR, US_PASS_INCORRECT};
using namespace std;

class User
{
public:
    User ( string&, int&, char&, float&, float&, string& );
    User ( string&, string& );
    ~User () { cout << "destructor de user"; }
    void SaveData () const;
    void LoadData ( string& );
    string nameUsr;
    int age;
    char sex;
    float weight;
    float height;
private:
    void FileReg ();
    void ExtractData ();
    bool VerifReg (string &);
    bool VerifUser (string &, string &);
    fstream userData;
    //vector < Session > sessions;
    string password;
    int numSessions = 0;
};



#endif // USER_H
