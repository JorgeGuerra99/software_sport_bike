#include "Header_Files/user.h"

User::User( const string& name,const int& ag, const char& sx, const float& wei, const float& hei, const string& pass)
{
    //Método de registro de usuario. Llena datos del usuario en el archivo UserData.txt
    if (!VerifReg(name))
    {
        userData.open("UserData.txt", ios::app);
        //registro de usuario
        dataUser.name = name;
        dataUser.age = ag;
        dataUser.sex = sx;
        dataUser.weight = wei;
        dataUser.height = hei;
        password = pass;
        userData << dataUser.name << " " << dataUser.age << " " << dataUser.sex << " " << dataUser.weight << " " << dataUser.height << " " << password << endl;
        cout << "datos exportados" << endl;
    } else
    {
        throw (int (REGISTERED_USER));
    }
}

User::User( const string& name, const string& pass)
{
    if (VerifUser(name, pass))
    {
        dataUser.name = name;
        password = pass;
        ExtractData();
        if (LoadData()) cout << "Sesión iniciada" << endl;
    }
    else
    {
        throw (int (US_PASS_INCORRECT));
    }
}

User::User(const User& cUser)
{
    dataUser.name = cUser.dataUser.name;
    dataUser.age = cUser.dataUser.age;
    dataUser.sex = cUser.dataUser.sex;
    dataUser.weight = cUser.dataUser.weight;
    dataUser.height = cUser.dataUser.height;
}

void User::SaveData()
{
    string filename = "data_";
    filename += dataUser.name;
    filename += ".txt";
    sessionsData.open (filename, ios::out);
    if (!sessionsData)
    {
        cout << "Error al abrir el archivo" << endl;
    }
    for (int i = 0; i < (int) sessions.size(); i++)
    {
        sessionsData << *sessions [i];
    }
    sessionsData.close();
    this->LoadData();
}

bool User::LoadData()
{
    string filename = "data_";
    filename+= dataUser.name + ".txt";
    sessionsData.open(filename, ios::in);
    if (!sessionsData)
    {
        return false;
    }
    string line;
    sessions.clear();
    while (!sessionsData.eof())
    {
        getline (sessionsData,line);
        if (line.find("CARDIO") != string::npos)
        {
            cout << "Se creo una cardio" << endl;
            auxCardio = new Cardio ();
            sessionsData >> *auxCardio;
            sessions.push_back(auxCardio);
        } else if (line.find("WEIGHTLOSS") != string::npos)
        {
            cout << "Se creo una weightloss" << endl;
            auxWeightLoss = new WeightLoss ();
            sessionsData >> * auxWeightLoss;
            sessions.push_back(auxWeightLoss);
        } else if (line.find("FREE") != string::npos)
        {
            auxFree = new Free ();
            sessionsData >> * auxFree;
            sessions.push_back(auxFree);
        }
    }

    sessionsData.close();
    return true;
}

void User::SaveLastSession(Session* ses)
{
    sessions.push_back(ses);
    SaveData();
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
            if (line == dataUser.name)
            {
                lineAux.resize (lineAux.find_last_of(" "));
                sheigth.append (lineAux.begin() + lineAux.find_last_of(" ") + 1, lineAux.end());
                lineAux.resize(lineAux.find_last_of(" "));
                sweigth.append (lineAux.begin() + lineAux.find_last_of(" ") +1, lineAux.end());
                lineAux.resize (lineAux.find_last_of(" "));
                ssex.append (lineAux.begin() + lineAux.find_last_of(" ") + 1, lineAux.end());
                lineAux.resize (lineAux.find_last_of(" "));
                sage.append (lineAux.begin() + lineAux.find_last_of(" ") + 1, lineAux.end());

                dataUser.weight = std::stof(sweigth);
                dataUser.height = std::stof(sheigth);
                dataUser.age = std::stoi(sage);
                dataUser.sex = ssex [0];
            }
        }
    }
}

bool User::VerifReg( const string& name)
{
    //Método que devuelve un valor booleano indicativo si el usuario se encuentra o no ya registrado
    userData.open ("UserData.txt", ios::app);
    userData.close();
    userData.open("UserData.txt", ios::in);
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

bool User::VerifUser (const string& name, const string& pass)
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

vector <string> User::NameSessions ()
{
    vector <string> aux;
    for ( int i = 0 ; i < (int) sessions.size(); i++ )
    {
        aux.push_back(sessions[i]->nameSession) ;
    }
    return aux;
}
