#include "Header_Files/user.h"

User::User(string &name, int &ag, char &sx, float &wei, float &hei, string &pass)
{
    //Método de registro de usuario. Llena datos del usuario en el archivo UserData.txt
    if (!VerifReg(name))
    {
        userData.open("UserData.txt", ios::app);
        //registro de usuario
        nameUsr = name;
        age = ag;
        sex = sx;
        weight = wei;
        height = hei;
        password = pass;
        userData << nameUsr << " " << age << " " << sex << " " << weight << " " << height << " " << password << endl;
        cout << "datos exportados" << endl;
    } else
    {
        throw (int (REGISTERED_USER));
    }
}

User::User(string &name, string &pass)
{
    if (VerifUser(name, pass))
    {
        nameUsr = name;
        password = pass;
        ExtractData();
    }
    else
    {
        throw (int (US_PASS_INCORRECT));
    }
}

void User::ExtractData()
{
    userData.open("UserData.txt", ios::in);
    if (!userData)
    {
        cout << "ERROR al abrir UserData.txt" << endl;
        throw (int (FILE_ERROR));
    }
    string line, lineAux, sweigth, sheigth, ssex, sage;
    int auxPos;
    while (!userData.eof())
    {
        getline (userData, line);
        lineAux = line;
        auxPos = line.find_first_of(" ");
        if (auxPos != (int) string::npos)
        {
            line.resize(auxPos);
            if (line == nameUsr)
            {
                lineAux.resize(lineAux.find_last_of(" "));
                sweigth.append (lineAux.begin() + lineAux.find_last_of(" ") +1, lineAux.end());
                lineAux.resize (lineAux.find_last_of(" "));
                sheigth.append (lineAux.begin() + lineAux.find_last_of(" ") + 1, lineAux.end());
                lineAux.resize (lineAux.find_last_of(" "));
                ssex.append (lineAux.begin() + lineAux.find_last_of(" ") + 1, lineAux.end());
                lineAux.resize (lineAux.find_last_of(" "));
                sage.append (lineAux.begin() + lineAux.find_last_of(" ") + 1, lineAux.end());

                weight = std::stof(sweigth);
                height = std::stof(sheigth);
                age = std::stoi(sage);
                sex = ssex [0];
            }
        }
    }
}

bool User::VerifReg(string &name)
{
    //Método que devuelve un valor booleano indicativo si el usuario se encuentra o no ya registrado
    userData.open("UserData.txt", ios::app);
    if (!userData)
    {
        cout << "ERROR al abrir UserData.txt" << endl;
        throw (int (FILE_ERROR));
    }
    string line;
    int auxPos;
    while (!userData.eof())
    {
        getline (userData, line);
        auxPos = line.find_first_of (" ");
        if (auxPos != (int) string::npos) //si encuentra el " "
        {
            line.resize(auxPos);
            if (line == name)
            {
                cout << "ERROR: Este usuario ya se encuentra registrado" << endl;
                userData.close();
                return true;
            }
        }
    }
    userData.close();
    return false;
}

bool User::VerifUser (string &name, string &pass)
{
    userData.open("UserData.txt", ios::app); //creo que esto es para que si no existe, lo crea
    userData.close ();
    userData.open("UserData.txt", ios::in);
    if (!userData)
    {
        cout << "ERROR al abrir UserData.txt" << endl;
        throw (int (FILE_ERROR));
    }
    string line, lineAux, passAux;
    int auxPos;
    while (!userData.eof())
    {
        getline (userData, line);
        lineAux = line;
        auxPos = line.find_first_of(" ");
        if (auxPos != (int) string::npos)
        {
            line.resize(auxPos);
            if (line == name)
            {
                passAux.append(lineAux.begin()+lineAux.find_last_of(" ") + 1, lineAux.end());
                if (pass == passAux)
                {
                    userData.close();
                    return true;
                }
            }
        }
    }
    userData.close();
    return false;
}
