#include "pidcontroller.h"

PIDController::PIDController(QObject *parent)
    : QObject{parent}
{

}

PIDController::~PIDController()
{

}

bool PIDController::init()
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
    kp = jsonConfig["kp"].toString().toFloat();
    kd = jsonConfig["kd"].toString().toFloat();
    ki = jsonConfig["ki"].toString().toFloat();

    return true;
}

float PIDController::getError()
{
    return error;
}

bool PIDController::setKp(float kp)
{
    this->kp = kp;

    return saveKPID();
}

float PIDController::getKp()
{
    return kp;
}

bool PIDController::setKd(float kd)
{
    this->kd = kd;

    return saveKPID();
}

float PIDController::getKd()
{
    return kd;
}

bool PIDController::setKi(float ki)
{
    this->ki = ki;

    return saveKPID();
}

float PIDController::getKi()
{
    return ki;
}

float PIDController::getRPMKp()
{
    return rpmKp;
}

float PIDController::getRPMKd()
{
    return rpmKd;
}

float PIDController::getRPMKi()
{
    return rpmKi;
}

bool PIDController::saveKPID()
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
    configFile.open(QIODevice::OpenModeFlag::WriteOnly);

    QJsonDocument jsonDocument;
    QJsonObject jsonConfig;

    // Datos
    jsonConfig.insert("kp", QJsonValue(QString::asprintf("%f", kp)));
    jsonConfig.insert("kd", QJsonValue(QString::asprintf("%f", kd)));
    jsonConfig.insert("ki", QJsonValue(QString::asprintf("%f", ki)));

    jsonDocument.setObject(jsonConfig);

    configFile.write(jsonDocument.toJson());
    configFile.flush();

    configFile.close();

    return true;
}

int8_t PIDController::getFreq(float setPointOD, float od, float temp)
{
    error_prev = error;
    error = setPointOD - od;

    rpmKp = error * kp;
    rpmKd = (error - error_prev) * kd;
    rpmKi += error * ki;

    return rpmKp + rpmKd + rpmKi;
}

void PIDController::resetPID()
{
    error = 0;
    error_prev = 0;
    error_int = 0;
}
