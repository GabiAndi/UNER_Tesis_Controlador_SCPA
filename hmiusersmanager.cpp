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

QList<QStringList> HMIUsersManager::listUsers()
{
    QList<QStringList> users;

    QJsonArray jsonUsers = readUsers();

    for (int i = 0 ; i < getNumberUsers() ; i++)
    {
        QStringList user;

        user.append(jsonUsers.at(i)["user"].toString());
        user.append(jsonUsers.at(i)["password"].toString());

        users.append(user);
    }

    return users;
}

bool HMIUsersManager::loginUser(const QString &userName, const QString &password)
{
    QJsonObject userLogin;

    userLogin.insert("user", QJsonValue(userName));
    userLogin.insert("password", QJsonValue(password));

    return readUsers().contains(userLogin);
}

bool HMIUsersManager::addUser(const QString &userName, const QString &password)
{
    QJsonArray users = readUsers();

    QJsonObject newUser;

    newUser.insert("user", QJsonValue(userName));
    newUser.insert("password", QJsonValue(password));

    if (!users.contains(newUser))
    {
        users.append(newUser);

        writeUsers(users);

        return true;
    }

    return false;
}

bool HMIUsersManager::removeUser(const QString &userName, const QString &password)
{
    QJsonArray users = readUsers();

    QJsonObject userToRemove;

    userToRemove.insert("user", QJsonValue(userName));
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

bool HMIUsersManager::renameUser(const QString &userName, const QString &password,
                                 const QString &newUserName, const QString &newPassword)
{
    QJsonArray users = readUsers();

    QJsonObject userToRename;

    userToRename.insert("user", QJsonValue(userName));
    userToRename.insert("password", QJsonValue(password));

    if (users.contains(userToRename))
    {
        for (int i = 0 ; i < getNumberUsers() ; i++)
        {
            if (users.at(i) == userToRename)
            {
                users.removeAt(i);

                addUser(newUserName, newPassword);

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

    // Si el archivo no existe se crea
    if (!usersFile.exists())
        usersFile.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::NewOnly);

    else
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

    // Si el archivo no existe se crea
    if (!usersFile.exists())
        usersFile.open(QIODevice::OpenModeFlag::WriteOnly | QIODevice::OpenModeFlag::NewOnly);

    else
        usersFile.open(QIODevice::OpenModeFlag::WriteOnly);

    QJsonDocument jsonDocument(users);

    usersFile.write(jsonDocument.toJson());
    usersFile.flush();

    usersFile.close();
}
