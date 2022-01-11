/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 10/01/2022                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase que lee los usuarios registrados en el sistema.     */
/*************************************************************/

#ifndef HMIUSERSMANAGER_H
#define HMIUSERSMANAGER_H

#include <QObject>

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

class HMIUsersManager : public QObject
{
        Q_OBJECT

    public:
        explicit HMIUsersManager(QObject *parent = nullptr);
        ~HMIUsersManager();

        QJsonArray readUsers();
        bool addUser(const QString &user, const QString &password);
        bool removeUser(const QString &user, const QString &password);
        bool renameUser(const QString &user, const QString &password,
                        const QString &newUser, const QString &newPassword);
        uint8_t getNumberUsers();
        bool loginUser(const QString &user, const QString &password);
        void writeUsers(const QJsonArray &users);

    private:
        // Usuarios
        const QString usersSubdir = "users";

        QJsonArray userArray;  // Usuarios
};

#endif // HMIUSERSMANAGER_H
