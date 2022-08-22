#include "Header_Files/user.h"

User::User(string &name, int &ag, char &sx, float &wei, float &hei, string &pass)
{
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
    userData.open("UserData.txt", ios::app);
    string line;
    int auxPos;
    while (!userData.eof())
    {
        getline (userData, line);
        auxPos = line.find_first_of (" ");
        if (auxPos != (int) string::npos)
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
    userData.open("UserData.txt", ios::app);
    userData.close ();
    userData.open("UserData.txt", ios::in);
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
