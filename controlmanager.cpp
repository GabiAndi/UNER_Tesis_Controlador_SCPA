#include "controlmanager.h"

ControlManager::ControlManager(QObject *parent)
    : QObject{parent}
{

}

ControlManager::~ControlManager()
{
    systemState->active = false;
    pidTimer->stop();
    frequencyDriver->stop();

    delete sensors;
    delete systemState;

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
    systemState = new system_state_t;

    // Controlado del variador
    frequencyDriver = new FrequencyDriver(this);

    // Controlador del sistema
    pidController = new PIDController(this);

    // Timer de PID
    pidTimer = new QTimer(this);

    connect(pidTimer, &QTimer::timeout, this, &ControlManager::syncPID);

    pidTimer->start(500);

    // Mensaje de finalizacion de carga
    logFile->println("Cargado");
}

void ControlManager::getSensorValue(Sensor sensor)
{
    switch (sensor)
    {
        case Sensor::SENSOR_LV_FOSO:
            emit sendSensorValue(Sensor::SENSOR_LV_FOSO, sensors->pileta.lvFoso);

            break;

        case Sensor::SENSOR_LV_LODO:
            emit sendSensorValue(Sensor::SENSOR_LV_LODO, sensors->pileta.lvLodo);

            break;

        case Sensor::SENSOR_TEMP:
            emit sendSensorValue(Sensor::SENSOR_TEMP, sensors->pileta.temp);

            break;

        case Sensor::SENSOR_OD:
            emit sendSensorValue(Sensor::SENSOR_OD, sensors->pileta.od);

            break;

        case Sensor::SENSOR_PH_ANOX:
            emit sendSensorValue(Sensor::SENSOR_PH_ANOX, sensors->pileta.phAnox);

            break;

        case Sensor::SENSOR_PH_AIREACION:
            emit sendSensorValue(Sensor::SENSOR_PH_AIREACION, sensors->pileta.phAireacion);

            break;

        case Sensor::SENSOR_MOTOR_CURRENT:
            emit sendSensorValue(Sensor::SENSOR_MOTOR_CURRENT, sensors->motor.current);

            break;

        case Sensor::SENSOR_MOTOR_VOLTAJE:
            emit sendSensorValue(Sensor::SENSOR_MOTOR_VOLTAJE, sensors->motor.voltaje);

            break;

        case Sensor::SENSOR_MOTOR_TEMP:
            emit sendSensorValue(Sensor::SENSOR_MOTOR_TEMP, sensors->motor.temperature);

            break;

        case Sensor::SENSOR_MOTOR_VELOCITY:
            emit sendSensorValue(Sensor::SENSOR_MOTOR_VELOCITY, sensors->motor.velocity);

            break;
    }
}

void ControlManager::setSensorValue(Sensor sensor, float value)
{
    switch (sensor)
    {
        case Sensor::SENSOR_LV_FOSO:
            sensors->pileta.lvFoso = value;

            break;

        case Sensor::SENSOR_LV_LODO:
            sensors->pileta.lvLodo = value;

            break;

        case Sensor::SENSOR_TEMP:
            sensors->pileta.temp = value;

            break;

        case Sensor::SENSOR_OD:
            sensors->pileta.od = value;

            break;

        case Sensor::SENSOR_PH_ANOX:
            sensors->pileta.phAnox = value;

            break;

        case Sensor::SENSOR_PH_AIREACION:
            sensors->pileta.phAireacion = value;

            break;

        case Sensor::SENSOR_MOTOR_CURRENT:
            sensors->motor.current = value;

            break;

        case Sensor::SENSOR_MOTOR_VOLTAJE:
            sensors->motor.voltaje = value;

            break;

        case Sensor::SENSOR_MOTOR_TEMP:
            sensors->motor.temperature = value;

            break;

        case Sensor::SENSOR_MOTOR_VELOCITY:
            sensors->motor.velocity = value;

            break;
    }
}

void ControlManager::getSystemState(SystemState state)
{
    switch (state)
    {
        case SystemState::CONTROL_SYSTEM:
            emit sendSystemState(state, systemState->active);

            break;

        case SystemState::SETPOINT_OD:
            emit sendSystemState(state, systemState->od);

            break;
    }
}

void ControlManager::setSystemState(SystemState state, float value)
{
    switch (state)
    {
        case SystemState::CONTROL_SYSTEM:
            systemState->active = value;

            break;

        case SystemState::SETPOINT_OD:
            systemState->od = value;

            break;
    }
}

void ControlManager::getMetricSensorValue(Sensor sensor)
{
    switch (sensor)
    {
        case Sensor::SENSOR_LV_FOSO:
            emit sendMetricSensorValue(Sensor::SENSOR_LV_FOSO, sensors->pileta.lvFoso);

            break;

        case Sensor::SENSOR_LV_LODO:
            emit sendMetricSensorValue(Sensor::SENSOR_LV_LODO, sensors->pileta.lvLodo);

            break;

        case Sensor::SENSOR_TEMP:
            emit sendMetricSensorValue(Sensor::SENSOR_TEMP, sensors->pileta.temp);

            break;

        case Sensor::SENSOR_OD:
            emit sendMetricSensorValue(Sensor::SENSOR_OD, sensors->pileta.od);

            break;

        case Sensor::SENSOR_PH_ANOX:
            emit sendMetricSensorValue(Sensor::SENSOR_PH_ANOX, sensors->pileta.phAnox);

            break;

        case Sensor::SENSOR_PH_AIREACION:
            emit sendMetricSensorValue(Sensor::SENSOR_PH_AIREACION, sensors->pileta.phAireacion);

            break;

        case Sensor::SENSOR_MOTOR_CURRENT:
            emit sendMetricSensorValue(Sensor::SENSOR_MOTOR_CURRENT, sensors->motor.current);

            break;

        case Sensor::SENSOR_MOTOR_VOLTAJE:
            emit sendMetricSensorValue(Sensor::SENSOR_MOTOR_VOLTAJE, sensors->motor.voltaje);

            break;

        case Sensor::SENSOR_MOTOR_TEMP:
            emit sendMetricSensorValue(Sensor::SENSOR_MOTOR_TEMP, sensors->motor.temperature);

            break;

        case Sensor::SENSOR_MOTOR_VELOCITY:
            emit sendMetricSensorValue(Sensor::SENSOR_MOTOR_VELOCITY, sensors->motor.velocity);

            break;
    }
}

void ControlManager::getMetricSystemState(SystemState state)
{
    switch (state)
    {
        case SystemState::CONTROL_SYSTEM:
            emit sendMetricSystemState(state, systemState->active);

            break;

        case SystemState::SETPOINT_OD:
            emit sendMetricSystemState(state, systemState->od);

            break;
    }
}

void ControlManager::syncPID()
{
    if (systemState->active)
    {
        uint8_t freq = pidController->getFreq(systemState->od, sensors->pileta.od, sensors->pileta.temp);

        frequencyDriver->start();
        frequencyDriver->setFreq(freq);

        sensors->motor.velocity = freq;
        sensors->motor.voltaje = 220;
        sensors->motor.current = 70;
    }

    else
    {
        frequencyDriver->stop();

        sensors->motor.velocity = 0;
        sensors->motor.voltaje = 0;
        sensors->motor.current = 0;
    }
}
