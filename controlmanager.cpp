#include "controlmanager.h"

ControlManager::ControlManager(QObject *parent)
    : QObject{parent}
{

}

ControlManager::~ControlManager()
{
    setPoints->active = false;
    pidTimer->stop();
    frequencyDriver->stop();

    delete sensors;
    delete setPoints;

    delete frequencyDriver;
    delete pidController;

    // Cierre del archivo de log
    logFile->println("Finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}

void ControlManager::init()
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->println("Iniciado");

    // Iniciamos el estado de las variables
    sensors = new sensors_t;

    // Iniciamos los set points
    setPoints = new set_point_t;

    // Controlado del variador
    frequencyDriver = new FrequencyDriver(this);

    // Controlador del sistema
    pidController = new PIDController(this);

    // Timer de PID
    pidTimer = new QTimer(this);

    connect(pidTimer, &QTimer::timeout, this, [this](){
        if (setPoints->active)
        {
            uint8_t freq = pidController->getFreq(setPoints->od, sensors->pileta.od, sensors->pileta.temp);

            frequencyDriver->start();
            frequencyDriver->setFreq(freq);
        }

        else
        {
            frequencyDriver->stop();
        }
    });

    pidTimer->start(500);

    // Mensaje de finalizacion de carga
    logFile->println("Cargado");
}

void ControlManager::getParameterValue(Sensor sensor)
{
    switch (sensor)
    {
        case Sensor::SENSOR_LV_FOSO:
            emit sendParameterValue(Sensor::SENSOR_LV_FOSO, sensors->pileta.lvFoso);

            break;

        case Sensor::SENSOR_LV_LODO:
            emit sendParameterValue(Sensor::SENSOR_LV_LODO, sensors->pileta.lvLodo);

            break;

        case Sensor::SENSOR_TEMP:
            emit sendParameterValue(Sensor::SENSOR_TEMP, sensors->pileta.temp);

            break;

        case Sensor::SENSOR_OD:
            emit sendParameterValue(Sensor::SENSOR_OD, sensors->pileta.od);

            break;

        case Sensor::SENSOR_PH_ANOX:
            emit sendParameterValue(Sensor::SENSOR_PH_ANOX, sensors->pileta.phAnox);

            break;

        case Sensor::SENSOR_PH_AIREACION:
            emit sendParameterValue(Sensor::SENSOR_PH_AIREACION, sensors->pileta.phAireacion);

            break;

        case Sensor::SENSOR_MOTOR_CURRENT:
            emit sendParameterValue(Sensor::SENSOR_MOTOR_CURRENT, sensors->motor.current);

            break;

        case Sensor::SENSOR_MOTOR_VOLTAJE:
            emit sendParameterValue(Sensor::SENSOR_MOTOR_VOLTAJE, sensors->motor.voltaje);

            break;

        case Sensor::SENSOR_MOTOR_TEMP:
            emit sendParameterValue(Sensor::SENSOR_MOTOR_TEMP, sensors->motor.temperature);

            break;

        case Sensor::SENSOR_MOTOR_VELOCITY:
            emit sendParameterValue(Sensor::SENSOR_MOTOR_VELOCITY, sensors->motor.velocity);

            break;

        case Sensor::SET_POINT_OD:
            emit sendParameterValue(Sensor::SET_POINT_OD, setPoints->od);

            break;
    }
}

void ControlManager::setLvFoso(float lv)
{
    sensors->pileta.lvFoso = lv;
}

void ControlManager::setLvLodo(float lv)
{
    sensors->pileta.lvLodo = lv;
}

void ControlManager::setTemp(float temp)
{
    sensors->pileta.temp = temp;
}

void ControlManager::setOD(float od)
{
    sensors->pileta.od = od;
}

void ControlManager::setPhAnox(float ph)
{
    sensors->pileta.phAnox = ph;
}

void ControlManager::setPhAireacion(float ph)
{
    sensors->pileta.phAireacion = ph;
}

void ControlManager::setInitSystem()
{
    setPoints->active = true;
}

void ControlManager::setStopSystem()
{
    setPoints->active = false;
}

void ControlManager::setSetPointOD(float setPointOD)
{
    setPoints->od = setPointOD;
}
