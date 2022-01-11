#include "hmiusersmanager.h"

HMIUsersManager::HMIUsersManager(QObject *parent)
    : QObject{parent}
{
    // Se crea la carpeta en donde ira el archivo
    if (!QDir(usersSubdir).exists())
    {
        QDir().mkdir(usersSubdir);
    }
}

HMIUsersManager::~HMIUsersManager()
{

}

uint8_t HMIUsersManager::getNumberUsers()
{
    return userArray.size();
}

bool HMIUsersManager::loginUser(const QString &user, const QString &password)
{
    QJsonObject userLogin;

    userLogin.insert("user", QJsonValue(user));
    userLogin.insert("password", QJsonValue(password));

    return userArray.contains(userLogin);
}

QJsonArray HMIUsersManager::readUsers()
{
    // Se abre el archivo para leer los usuarios
    QFile usersFile(usersSubdir + "/users.log");

    usersFile.open(QIODevice::OpenModeFlag::ReadOnly);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(usersFile.readAll());

    usersFile.close();

    return jsonDocument.array();
}

bool HMIUsersManager::addUser(const QString &user, const QString &password)
{
    QJsonObject newUser;

    newUser.insert("user", QJsonValue(user));
    newUser.insert("password", QJsonValue(password));

    if (!userArray.contains(newUser))
    {
        userArray.append(newUser);

        writeUsers(userArray);

        return true;
    }

    return false;
}

bool HMIUsersManager::removeUser(const QString &user, const QString &password)
{
    QJsonObject userToRemove;

    userToRemove.insert("user", QJsonValue(user));
    userToRemove.insert("password", QJsonValue(password));

    if (userArray.contains(userToRemove))
    {
        for (int i = 0 ; i < getNumberUsers() ; i++)
        {
            if (userArray.at(i) == userToRemove)
            {
                userArray.removeAt(i);

                writeUsers(userArray);

                return true;
            }
        }
    }

    return false;
}

bool HMIUsersManager::renameUser(const QString &user, const QString &password,
                                 const QString &newUser, const QString &newPassword)
{
    QJsonObject userToRename;

    userToRename.insert("user", QJsonValue(user));
    userToRename.insert("password", QJsonValue(password));

    if (userArray.contains(userToRename))
    {
        for (int i = 0 ; i < getNumberUsers() ; i++)
        {
            if (userArray.at(i) == userToRename)
            {
                userArray.removeAt(i);

                addUser(newUser, newPassword);

                return true;
            }
        }
    }

    return false;
}

void HMIUsersManager::writeUsers(const QJsonArray &users)
{
    // Se abre el archivo para leer los usuarios
    QFile usersFile(usersSubdir + "/users.log");

    usersFile.open(QIODevice::OpenModeFlag::WriteOnly | QIODevice::OpenModeFlag::Truncate);

    QJsonDocument jsonDocument(users);

    usersFile.write(jsonDocument.toJson());
    usersFile.flush();

    usersFile.close();
}
