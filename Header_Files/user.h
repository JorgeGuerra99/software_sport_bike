//------------------------------------ CLASE USUARIO --------------------------------------------------------
// Contiene los datos de cada usuario y los métodos de inicio de sesión o registro.
// Contiene el objeto sessions
#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "session.h"

enum {REGISTERED_USER, FILE_ERROR, US_PASS_INCORRECT, FILE_USER_NO_EXISTENT};
using namespace std;

class User
{
public:
    User ( string&, int&, char&, float&, float&, string& );
    User ( string&, string& );
    User (const User&); //constructor copia
    virtual ~User () { cout << "destructor de user"; }
    void SaveData ();
    bool LoadData ();
    void SaveLastSession (Session*);
    string nameUsr;
    int age;
    char sex;
    float weight;
    float height;
private:
    void FileReg ();
    void ExtractData ();
    /**
     * @brief VerifReg
     * @param name
     * @return
     */
    bool VerifReg (string& name);
    /**
     * @brief VerifUser
     * @param nameUs
     * @param pass
     * @return
     */
    bool VerifUser (string &, string &);
    fstream userData;
    fstream sessionsData;
    string password;
    vector <Session *> sessions;
    Cardio* auxCardio;
    Session* auxSession;
    int numSessions = 0;
    int numCardio = 0;
    int numWei = 0;
    int numFree = 0;
    bool fileExist = true;
};



#endif // USER_H
