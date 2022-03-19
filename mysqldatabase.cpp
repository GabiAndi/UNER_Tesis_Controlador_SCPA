#include "mysqldatabase.h"

MySQLDatabase::MySQLDatabase(QObject *parent)
    : QObject{parent}
{

}

MySQLDatabase::~MySQLDatabase()
{

}

bool MySQLDatabase::init()
{
    // Si la carpeta no existe se crea y se devuelve fail
    if (!QDir(configSubdir).exists())
    {
        QDir().mkdir(configSubdir);

        return false;
    }

    // Se abre el archivo para leer la configuracion
    QFile configFile(configSubdir + "/" + configFileName);

    // Si el archivo no existe se devuelve fail
    if (!configFile.exists())
    {
        return false;
    }

    // Si todo esta ok se procede a leer el archivo
    configFile.open(QIODevice::OpenModeFlag::ReadOnly);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(configFile.readAll());

    configFile.close();

    QJsonObject jsonConfig = jsonDocument.object();

    // Datos
    databaseName = jsonConfig["databaseName"].toString();
    ip = jsonConfig["ip"].toString();
    port = jsonConfig["port"].toInt();
    userName = jsonConfig["userName"].toString();
    userPassword = jsonConfig["userPassword"].toString();

    // Inicio de la base de datos
    database = QSqlDatabase::addDatabase("QMYSQL");

    database.setDatabaseName(databaseName);
    database.setHostName(ip);
    database.setPort(port);
    database.setUserName(userName);
    database.setPassword(userPassword);

    return database.open();
}

bool MySQLDatabase::insertSensorLvFosoValue(float value)
{
    if (!database.isOpen())
        return false;

    // Enviamos la metrica al servidor
    database.exec(QString::asprintf("insert into lvFoso(date, medicion) values(current_timestamp, %f)",
                                    value));

    return true;
}

bool MySQLDatabase::insertSensorLvLodoValue(float value)
{
    if (!database.isOpen())
        return false;

    // Enviamos la metrica al servidor
    database.exec(QString::asprintf("insert into lvLodo(date, medicion) values(current_timestamp, %f)",
                                    value));

    return true;
}

bool MySQLDatabase::insertSensorTempValue(float value)
{
    if (!database.isOpen())
        return false;

    // Enviamos la metrica al servidor
    database.exec(QString::asprintf("insert into temp(date, medicion) values(current_timestamp, %f)",
                                    value));

    return true;
}

bool MySQLDatabase::insertSensorODValue(float value)
{
    if (!database.isOpen())
        return false;

    // Enviamos la metrica al servidor
    database.exec(QString::asprintf("insert into od(date, medicion) values(current_timestamp, %f)",
                                    value));

    return true;
}

bool MySQLDatabase::insertSensorPhAnoxValue(float value)
{
    if (!database.isOpen())
        return false;

    // Enviamos la metrica al servidor
    database.exec(QString::asprintf("insert into phAnox(date, medicion) values(current_timestamp, %f)",
                                    value));

    return true;
}

bool MySQLDatabase::insertSensorPhAireacionValue(float value)
{
    if (!database.isOpen())
        return false;

    // Enviamos la metrica al servidor
    database.exec(QString::asprintf("insert into phAireacion(date, medicion) values(current_timestamp, %f)",
                                    value));

    return true;
}

bool MySQLDatabase::insertSensorMotorCurrentValue(float value)
{
    if (!database.isOpen())
        return false;

    // Enviamos la metrica al servidor
    database.exec(QString::asprintf("insert into motorCurrent(date, medicion) values(current_timestamp, %f)",
                                    value));

    return true;
}

bool MySQLDatabase::insertSensorMotorVoltajeValue(float value)
{
    if (!database.isOpen())
        return false;

    // Enviamos la metrica al servidor
    database.exec(QString::asprintf("insert into motorVoltaje(date, medicion) values(current_timestamp, %f)",
                                    value));

    return true;
}

bool MySQLDatabase::insertSensorMotorTempValue(float value)
{
    if (!database.isOpen())
        return false;

    // Enviamos la metrica al servidor
    database.exec(QString::asprintf("insert into motorTemp(date, medicion) values(current_timestamp, %f)",
                                    value));

    return true;
}

bool MySQLDatabase::insertSensorMotorVelocityValue(float value)
{
    if (!database.isOpen())
        return false;

    // Enviamos la metrica al servidor
    database.exec(QString::asprintf("insert into motorVelocity(date, medicion) values(current_timestamp, %f)",
                                    value));

    return true;
}
