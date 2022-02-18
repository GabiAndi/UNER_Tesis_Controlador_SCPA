#include "controlmanager.h"

ControlManager::ControlManager(QObject *parent)
    : QObject{parent}
{

}

ControlManager::~ControlManager()
{
    delete sensors;
    delete set_points;

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
    set_points = new set_point_t;

    // Mensaje de finalizacion de carga
    logFile->println("Cargado");
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

void ControlManager::setMotorCurrent(float current)
{
    sensors->motor.current = current;
}

void ControlManager::setMotorVoltaje(float voltaje)
{
    sensors->motor.voltaje = voltaje;
}

void ControlManager::setMotorTemp(float temp)
{
    sensors->motor.temperature = temp;
}

void ControlManager::setMotorVelocity(float velocity)
{
    sensors->motor.velocity = velocity;
}
