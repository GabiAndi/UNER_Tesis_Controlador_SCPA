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
#include <QMap>
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

        static bool addUser(const QString &userName, const QString &password);
        static bool removeUser(const QString &userName, const QString &password);
        static bool renameUser(const QString &userName, const QString &password,
                        const QString &newUserName, const QString &newPassword);
        static uint8_t getNumberUsers();
        static QList<QStringList> listUsers();
        static bool loginUser(const QString &userName, const QString &password);

    private:
        static QJsonArray readUsers();
        static void writeUsers(const QJsonArray &users);

        // Usuarios
        inline static const QString usersSubdir = "users";
        inline static const QString usersFileName = "users.txt";
};

#endif // HMIUSERSMANAGER_H
