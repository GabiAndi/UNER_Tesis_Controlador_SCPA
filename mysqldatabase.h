#ifndef MYSQLDATABASE_H
#define MYSQLDATABASE_H

#include <QObject>

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QDateTime>

#include <QDebug>

class MySQLDatabase : public QObject
{
        Q_OBJECT

    public:
        explicit MySQLDatabase(QObject *parent = nullptr);
        ~MySQLDatabase();

        bool init();

        bool insertSensorLvFosoValue(float value);
        bool insertSensorLvLodoValue(float value);
        bool insertSensorTempValue(float value);
        bool insertSensorODValue(float value);
        bool insertSensorPhAnoxValue(float value);
        bool insertSensorPhAireacionValue(float value);

        bool insertSensorMotorCurrentValue(float value);
        bool insertSensorMotorVoltajeValue(float value);
        bool insertSensorMotorTempValue(float value);
        bool insertSensorMotorVelocityValue(float value);

    private:
        // Configuracion
        inline static const QString configSubdir = "config";
        inline static const QString configFileName = "mysql.conf";

        // Valores de configuración
        QString databaseName;
        QString ip;
        int port;
        QString userName;
        QString userPassword;

        // Base de datos
        QSqlDatabase database;
};

#endif // MYSQLDATABASE_H
