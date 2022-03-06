#include "metricsmanager.h"

MetricsManager::MetricsManager(QObject *parent)
    : QObject{parent}
{

}

MetricsManager::~MetricsManager()
{
    delete database;

    delete timer;

    // Cierre del archivo de log
    logFile->println("Finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}

void MetricsManager::init()
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->println("Iniciado");

    // Inicio de la base de datos
    database = new MySQLDatabase(this);

    if (database->init())
    {
        logFile->println("Conectado correctamente al servidor");

        // Iniciamos el envio de datos
        timer = new QTimer(this);

        connect(timer, &QTimer::timeout, this, &MetricsManager::sendData);

        timer->start(2000);
    }

    else
    {
        logFile->println("Error de conexion");
    }
}

void MetricsManager::sendMetricSensorValue(Sensor sensor, float value)
{
    switch (sensor)
    {
        case Sensor::SENSOR_LV_FOSO:
            database->insertSensorLvFosoValue(value);

            break;

        case Sensor::SENSOR_LV_LODO:
            database->insertSensorLvLodoValue(value);

            break;

        case Sensor::SENSOR_TEMP:
            database->insertSensorTempValue(value);

            break;

        case Sensor::SENSOR_OD:
            database->insertSensorODValue(value);

            break;

        case Sensor::SENSOR_PH_ANOX:
            database->insertSensorPhAnoxValue(value);

            break;

        case Sensor::SENSOR_PH_AIREACION:
            database->insertSensorPhAireacionValue(value);

            break;

        case Sensor::SENSOR_MOTOR_CURRENT:
            database->insertSensorMotorCurrentValue(value);

            break;

        case Sensor::SENSOR_MOTOR_VOLTAJE:
            database->insertSensorMotorVoltajeValue(value);

            break;

        case Sensor::SENSOR_MOTOR_TEMP:
            database->insertSensorMotorTempValue(value);

            break;

        case Sensor::SENSOR_MOTOR_VELOCITY:
            database->insertSensorMotorVelocityValue(value);

            break;
    }
}

void MetricsManager::sendMetricSystemState(SystemState state, float value)
{

}

void MetricsManager::sendData()
{
    // Enviamos la peticion de estado de cada sensor
    emit getMetricSensorValue(Sensor::SENSOR_LV_FOSO);
    emit getMetricSensorValue(Sensor::SENSOR_LV_LODO);
    emit getMetricSensorValue(Sensor::SENSOR_TEMP);
    emit getMetricSensorValue(Sensor::SENSOR_OD);
    emit getMetricSensorValue(Sensor::SENSOR_PH_ANOX);
    emit getMetricSensorValue(Sensor::SENSOR_PH_AIREACION);

    emit getMetricSensorValue(Sensor::SENSOR_MOTOR_CURRENT);
    emit getMetricSensorValue(Sensor::SENSOR_MOTOR_VOLTAJE);
    emit getMetricSensorValue(Sensor::SENSOR_MOTOR_TEMP);
    emit getMetricSensorValue(Sensor::SENSOR_MOTOR_VELOCITY);

    // Enviamos la petición del estado del sistema
    //emit getMetricSystemState(SystemState::CONTROL_SYSTEM);
    //emit getMetricSystemState(SystemState::SETPOINT_OD);
}
