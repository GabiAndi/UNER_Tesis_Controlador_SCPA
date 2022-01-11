#include "hmiusersmanager.h"

HMIUsersManager::HMIUsersManager(QObject *parent)
    : QObject{parent}
{

}

HMIUsersManager::~HMIUsersManager()
{

}

uint8_t HMIUsersManager::getNumberUsers()
{
    return readUsers().size();
}

bool HMIUsersManager::loginUser(const QString &user, const QString &password)
{
    QJsonObject userLogin;

    userLogin.insert("user", QJsonValue(user));
    userLogin.insert("password", QJsonValue(password));

    return readUsers().contains(userLogin);
}

bool HMIUsersManager::addUser(const QString &user, const QString &password)
{
    QJsonArray users = readUsers();

    QJsonObject newUser;

    newUser.insert("user", QJsonValue(user));
    newUser.insert("password", QJsonValue(password));

    if (!users.contains(newUser))
    {
        users.append(newUser);

        writeUsers(users);

        return true;
    }

    return false;
}

bool HMIUsersManager::removeUser(const QString &user, const QString &password)
{
    QJsonArray users = readUsers();

    QJsonObject userToRemove;

    userToRemove.insert("user", QJsonValue(user));
    userToRemove.insert("password", QJsonValue(password));

    if (users.contains(userToRemove))
    {
        for (int i = 0 ; i < getNumberUsers() ; i++)
        {
            if (users.at(i) == userToRemove)
            {
                users.removeAt(i);

                writeUsers(users);

                return true;
            }
        }
    }

    return false;
}

bool HMIUsersManager::renameUser(const QString &user, const QString &password,
                                 const QString &newUser, const QString &newPassword)
{
    QJsonArray users = readUsers();

    QJsonObject userToRename;

    userToRename.insert("user", QJsonValue(user));
    userToRename.insert("password", QJsonValue(password));

    if (users.contains(userToRename))
    {
        for (int i = 0 ; i < getNumberUsers() ; i++)
        {
            if (users.at(i) == userToRename)
            {
                users.removeAt(i);

                addUser(newUser, newPassword);

                return true;
            }
        }
    }

    return false;
}

QJsonArray HMIUsersManager::readUsers()
{
    // Se crea la carpeta en donde ira el archivo
    if (!QDir(usersSubdir).exists())
    {
        QDir().mkdir(usersSubdir);
    }

    // Se abre el archivo para leer los usuarios
    QFile usersFile(usersSubdir + "/users.log");

    usersFile.open(QIODevice::OpenModeFlag::ReadOnly);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(usersFile.readAll());

    usersFile.close();

    return jsonDocument.array();
}

void HMIUsersManager::writeUsers(const QJsonArray &users)
{
    // Se crea la carpeta en donde ira el archivo
    if (!QDir(usersSubdir).exists())
    {
        QDir().mkdir(usersSubdir);
    }

    // Se abre el archivo para leer los usuarios
    QFile usersFile(usersSubdir + "/users.log");

    usersFile.open(QIODevice::OpenModeFlag::WriteOnly | QIODevice::OpenModeFlag::Truncate);

    QJsonDocument jsonDocument(users);

    usersFile.write(jsonDocument.toJson());
    usersFile.flush();

    usersFile.close();
}
