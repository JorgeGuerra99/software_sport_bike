/**
 * @file user.h
 * @brief Clase Usuario
 * @details Contiene los datos y métodos de un usuario
 * @date 2023
 * @authors Bazán María, Guerra Jorge
 */

#ifndef USER_H
#define USER_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include "session.h"
#include "dialogwin.h"

/**
 * @details
 * Utilizado para contener errores de diferentes tipos: Usuario ya registrado, archivo no
 * encontrado, usuario/clave incorrecta.
 */
enum {REGISTERED_USER, FILE_ERROR, US_PASS_INCORRECT, FILE_USER_NO_EXISTENT};
using namespace std;

class User
{
public:
    /**
     * @brief User
     * @note Constructor para usuario nuevo
     */
    User ( string&, int&, char&, float&, float&, string& );
    /**
     * @brief User
     * @note Constructor para usuario existente
     */
    User ( string&, string& );
    /**
     * @brief User
     * @note Constructor copia
     */
    User (const User&);
    virtual ~User () { cout << "destructor de user"; }
    /**
     * @brief SaveData
     * @note Almacena los datos del archivo de usuario en el archivo correspondiente
     */
    void SaveData ();
    /**
     * @brief LoadData
     * @note Carga los datos del archivo de usuario existente
     * @return true/false dependiendo si el archivo existe
     */
    bool LoadData ();
    /**
     * @brief SaveLastSession
     * @note Almacena la última sesión en el vector de punteros
     */
    void SaveLastSession (Session*);
    vector <string> NameSessions ();
    Session* GetSession (const int& posSession) const { return sessions[posSession];}
    /**
     * @note Datos de usuario
     */
    string nameUsr;
    int age;
    char sex;
    float weight;
    float height;
private:
    /**
     * @brief ExtractData
     * @note Extrae los datos del usuario del archivo UserData
     */
    void ExtractData ();
    /**
     * @brief VerifReg
     * @param name : Nombre de usuario
     * @return : true si el usuario se encuentra registrado, false caso contrario
     */
    bool VerifReg (string& name);
    /**
     * @brief VerifUser
     * @param nameUs : Nombre de usuario
     * @param pass : Contraseña
     * @return : Booleano dependiendo si el usuario y contraseña son válidos o no
     */
    bool VerifUser (string &, string &);
    /**
     * @brief userData
     * @note Objeto del archivo que contiene los datos de cada usuario registrado
     */
    fstream userData;
    /**
     * @brief sessionsData
     * @note Objeto del archivo que contiene los datos de sesión del usuario
     */
    fstream sessionsData;
    string password;
    /**
     * @brief sessions
     * @note Vector de punteros a sesión utilizado para almacenar las diferentes clases de sesión
     */
    vector <Session *> sessions;
    Cardio* auxCardio;
    WeightLoss* auxWeightLoss;
    Free* auxFree;
    Session* auxSession;
    int numSessions = 0;
    int numCardio = 0;
    int numWei = 0;
    int numFree = 0;
};



#endif // USER_H
