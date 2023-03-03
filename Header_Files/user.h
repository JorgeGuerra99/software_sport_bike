/**
 * @file user.h
 * @brief Clase Usuario: Contiene los datos y métodos de un usuario
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
 * @brief enum: Utilizado para contener errores de diferentes tipos
 * @details Usuario ya registrado, archivo no encontrado, usuario/clave incorrecta.
 */
enum {REGISTERED_USER, FILE_ERROR, US_PASS_INCORRECT, FILE_USER_NO_EXISTENT};
using namespace std;

class User
{
public:
    /**
     * @brief User - Constructor
     * @details Constructor para usuario nuevo
     * @param name: Nombre del usuario
     * @param ag: Edad del usuario
     * @param sx: Genero del usuario
     * @param wei: Peso del usuario
     * @param hei: Altura del usuario
     * @param pass: Constraseña del usuario
     */
    User ( const string& name,const int& ag, const char& sx, const float& wei, const float& hei, const string& pass );
    /**
     * @brief User - Constructor
     * @details Constructor para usuario existent4e
     * @param name: Nombre del usuario
     * @param pass: Contraseña del usuario
     */
    User ( const string& name, const string& pass );
    /**
     * @brief User - Constructor
     * @details Constructor copia
     * @param cUsre: Objeto del tipo usuario
     */
    User (const User& cUser);
    virtual ~User () { cout << "destructor de user"; }
    /**
     * @brief SaveData: Almacena los datos del archivo de usuario en el archivo correspondiente
     */
    void SaveData ();
    /**
     * @brief LoadData: Carga los datos del archivo de usuario existente
     * @return Retorna verdadero/falso dependiendo si el archivo existe
     */
    bool LoadData ();
    /**
     * @brief SaveLastSession: Almacena la última sesión en el vector de punteros
     */
    void SaveLastSession (Session*);
    /**
     * @brief NameSessions: Permite tener los nombres de las sesiones
     * @return Retorna el nombre de las sesiones ya realizadas
     */
    vector <string> NameSessions ();
    /**
     * @brief GetSession: Permite retornar una única sesión ya realizada por el usuario
     * @param posSession: Posición en la que se encuentra la sesión requerida
     * @return Retorna todos los datos relacionados a la sesión realizada
     */
    Session* GetSession (const int& posSession) const { return sessions[posSession];}
    /**
     * @brief dataUser: Estructura donde se almacena los datos del usuario
     */
    struct {
        string name;
        int age;
        char sex;
        float weight, height;
    } dataUser;
private:
    /**
     * @brief ExtractData: Extrae los datos del usuario del archivo UserData
     */
    void ExtractData ();
    /**
     * @brief VerifReg: Verifica si el nombre usuario ya está registrado
     * @param name: Nombre de usuario
     * @return Retorna verdadero si el nombre usuario se encuentra registrado
     */
    bool VerifReg (const string& name);
    /**
     * @brief VerifUser: Verifica el nombre de usuario y contraseña sean correcto
     * @param nameUs : Nombre del usuario
     * @param pass : Contraseña del usuario
     * @return : Booleano dependiendo si el usuario y contraseña son válidos o no
     */
    bool VerifUser (const string & nameUs, const string& pass);
    /**
     * @brief userData: Objeto del archivo que contiene los datos de cada usuario registrado
     */
    fstream userData;
    /**
     * @brief sessionsData: Objeto del archivo que contiene los datos de sesión del usuario
     */
    fstream sessionsData;
    /**
     * @brief password: Propiedad donde se almacena la contraseña
     */
    string password;
    /**
     * @brief sessions: Vector de punteros a sesión utilizado para almacenar las diferentes clases de sesión
     */
    vector <Session *> sessions;
    /**
     * @brief auxCardio: Puntero auxiliar del objeto cardio
     */
    Cardio* auxCardio;
    /**
     * @brief auxWeightLoss: Puntero auxiliar del objeto WeightLoss
     */
    WeightLoss* auxWeightLoss;
    /**
     * @brief auxFree: Puntero auxiliar del objeto Free
     */
    Free* auxFree;
    /**
     * @brief auxSession: Puntero auxiliar del objeto Session
     */
    Session* auxSession;
    /**
     * @brief numSessions: Propiedad con la cantidad de sesiones realizadas por el usuario
     */
    int numSessions = 0;
    /**
     * @brief numCardio: Propiedad con la cantidad de sesiones cardio realizadas por el usuario
     */
    int numCardio = 0;
    /**
     * @brief numWei: Propiedad con la cantidad de sesiones weightloss realizadas por el usuario
     */
    int numWei = 0;
    /**
     * @brief numFree: Propiedad con la cantidad de sesiones free realizadas por el usuario
     */
    int numFree = 0;
};



#endif // USER_H
