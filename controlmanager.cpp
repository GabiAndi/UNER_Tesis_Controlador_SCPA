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
    delete scpa;

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

    if (!pidController->init())
    {
        logFile->println("No se encontro el archivo de configuracion del controlador PID");
    }

    else
    {
        logFile->println("Configuracion correcta del controlador PID");
    }

    // Sensor de OD
    scpa = new SCPA(this);

    if (!scpa->init())
    {
        logFile->println("No se encontro el archivo de configuracion del sistema");
    }

    else
    {
        logFile->println("Configuracion correcta del sistema");

        connect(scpa, &SCPA::scpaConnected, this, &ControlManager::scpaConnected);
        connect(scpa, &SCPA::scpaErrorConnected, this, &ControlManager::scpaErrorConnected);
        connect(scpa, &SCPA::scpaDisconnected, this, &ControlManager::scpaDisconnected);

        connect(scpa, &SCPA::readOD, this, [this](float od)
        {
            sensors->pileta.od = od;
        });
    }

    // Timer de PID
    pidTimer = new QTimer(this);

    connect(pidTimer, &QTimer::timeout, this, &ControlManager::syncPID);

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
            emit sendSystemState(SystemState::CONTROL_SYSTEM, systemState->active);

            break;

        case SystemState::SETPOINT_OD:
            emit sendSystemState(SystemState::SETPOINT_OD, systemState->od);

            break;

        case SystemState::PID_ERROR:
            emit sendSystemState(SystemState::PID_ERROR, pidController->getError());

            break;

        case SystemState::PID_KP:
            emit sendSystemState(SystemState::PID_KP, pidController->getKp());

            break;

        case SystemState::PID_RPM_KP:
            emit sendSystemState(SystemState::PID_RPM_KP, pidController->getRPMKp());

            break;

        case SystemState::PID_KD:
            emit sendSystemState(SystemState::PID_KD, pidController->getKd());

            break;

        case SystemState::PID_RPM_KD:
            emit sendSystemState(SystemState::PID_RPM_KD, pidController->getRPMKd());

            break;

        case SystemState::PID_KI:
            emit sendSystemState(SystemState::PID_KI, pidController->getKi());

            break;

        case SystemState::PID_RPM_KI:
            emit sendSystemState(SystemState::PID_RPM_KI, pidController->getRPMKi());

            break;
    }
}

void ControlManager::setSystemState(SystemState state, float value)
{
    switch (state)
    {
        case SystemState::CONTROL_SYSTEM:
            systemState->active = value;

            if (value)
            {
                frequencyDriver->start();
                pidTimer->start(100000);

                syncPID();
            }

            else
            {
                frequencyDriver->stop();
                pidTimer->stop();

                pidController->resetPID();

                // Actualizamos los valores
                sensors->motor.velocity = 0;
                sensors->motor.voltaje = 0;
                sensors->motor.current = 0;
            }

            break;

        case SystemState::SETPOINT_OD:
            systemState->od = value;

            break;

        case SystemState::PID_KP:
            pidController->setKp(value);

            break;

        case SystemState::PID_KD:
            pidController->setKd(value);

            break;

        case SystemState::PID_KI:
            pidController->setKi(value);

            break;

        case SystemState::PID_ERROR:
        case SystemState::PID_RPM_KP:
        case SystemState::PID_RPM_KD:
        case SystemState::PID_RPM_KI:
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
            emit sendMetricSystemState(SystemState::CONTROL_SYSTEM, systemState->active);

            break;

        case SystemState::SETPOINT_OD:
            emit sendMetricSystemState(SystemState::SETPOINT_OD, systemState->od);

            break;

        case SystemState::PID_ERROR:
            emit sendMetricSystemState(SystemState::PID_ERROR, pidController->getError());

            break;

        case SystemState::PID_KP:
            emit sendMetricSystemState(SystemState::PID_KP, pidController->getKp());

            break;

        case SystemState::PID_RPM_KP:
            emit sendMetricSystemState(SystemState::PID_RPM_KP, pidController->getRPMKp());

            break;

        case SystemState::PID_KD:
            emit sendMetricSystemState(SystemState::PID_KD, pidController->getKd());

            break;

        case SystemState::PID_RPM_KD:
            emit sendMetricSystemState(SystemState::PID_RPM_KD, pidController->getRPMKd());

            break;

        case SystemState::PID_KI:
            emit sendMetricSystemState(SystemState::PID_KI, pidController->getKi());

            break;

        case SystemState::PID_RPM_KI:
            emit sendMetricSystemState(SystemState::PID_RPM_KI, pidController->getRPMKi());

            break;
    }
}

void ControlManager::syncPID()
{
    int8_t freqCorrection = pidController->getFreq(systemState->od, sensors->pileta.od, sensors->pileta.temp);

    if (freqCorrection > 60)
        freqCorrection = 60;

    else if (freqCorrection < -60)
        freqCorrection = -60;

    sensors->motor.velocity += freqCorrection;
    sensors->motor.voltaje = 220.0 * sensors->motor.velocity / 50.0;
    sensors->motor.current = 70;

    // Establecemos el variador de frecuencia
    frequencyDriver->setFreq(sensors->motor.velocity);

    // Establecemos la velocidad
    scpa->setFreq(sensors->motor.velocity);
}

void ControlManager::scpaConnected()
{
    logFile->println("Conexion correcta con el sistema");
}

void ControlManager::scpaErrorConnected(QAbstractSocket::SocketError error)
{
    logFile->println("Error de conexion con el sistema");
}

void ControlManager::scpaDisconnected()
{
    logFile->println("Desconexion con el sistema");
}
