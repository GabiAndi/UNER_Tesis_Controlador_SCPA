#include "hmiuser.h"

HMIUser::HMIUser(QString userName, QString password, QTcpSocket *tcpSocket, QObject *parent)
    : HMIClient{tcpSocket, parent}, userName{userName}, password{password}
{

}

HMIUser::~HMIUser()
{

}

QString HMIUser::getUserName()
{
    return userName;
}

QString HMIUser::getPassword()
{
    return password;
}

void HMIUser::sendLoginForceRequired()
{
    hmiProtocol->write(Command::LOGIN_REQUEST, QByteArray().append(LoginRequest::LOGIN_FORCE_REQUIRED));
}

void HMIUser::sendLoginCorrect()
{
    hmiProtocol->write(Command::LOGIN_REQUEST, QByteArray().append(LoginRequest::LOGIN_CORRECT));
}

void HMIUser::sendDisconnectOtherUserLogin()
{
    hmiProtocol->write(Command::DISCONNECT_CODE, QByteArray().append(DisconnectCode::OTHER_USER_LOGIN));
}

void HMIUser::sendRequestSetParam(SimulationSensor sensor)
{
    hmiProtocol->write(Command::REQUEST_SET_PARAM, QByteArray().append(sensor));
}

void HMIUser::tcpSocketDisconnected()
{
    emit userDisconnected(this);
}

void HMIUser::newPackage(const uint8_t cmd, const QByteArray payload)
{
    // Se analiza el comando recibido
    switch ((Command)(cmd))
    {
        /*
         * ALIVE
         *
         * Si recibimos un alive lo devolvemos automaticamente
         * sin informar al client manager.
         */
        case Command::KEEP_ALIVE:
        {
            if ((uint8_t)(payload.at(0)) == KeepAliveMode::REPLY)
            {
                hmiProtocol->write(Command::KEEP_ALIVE, QByteArray().append(KeepAliveMode::REQUEST));
            }

            else if ((uint8_t)(payload.at(0)) == KeepAliveMode::REQUEST)
            {

            }

            break;
        }

        /*
         * LOGIN
         *
         * Al recibir un LOGIN extraemos el usuario y contraseña y lo enviamos
         * a las capas superiores.
         */
        case Command::FORCE_LOGIN:
        {
            bool connect = ((ForceLogin)(payload.at(0)) == ForceLogin::FORCE_CONNECT);

            emit userForceLogin(this, connect);

            break;
        }

        /*
         * SET_PARAM
         *
         * Se establece los valores de los sensores para la simulación
         */
        case Command::SET_PARAM:
        {
            DataConverter converter;

            converter.u8[0] = payload.at(1);
            converter.u8[1] = payload.at(2);
            converter.u8[2] = payload.at(3);
            converter.u8[3] = payload.at(4);

            switch ((SimulationSensor)(payload.at(0)))
            {
                case SimulationSensor::SENSOR_LV_FOSO:
                    emit setSimulationLvFoso(converter.f[0]);

                    break;

                case SimulationSensor::SENSOR_LV_LODO:
                    emit setSimulationLvLodo(converter.f[0]);

                    break;

                case SimulationSensor::SENSOR_TEMP:
                    emit setSimulationTemp(converter.f[0]);

                    break;

                case SimulationSensor::SENSOR_OD:
                    emit setSimulationOD(converter.f[0]);

                    break;

                case SimulationSensor::SENSOR_PH_ANOX:
                    emit setSimulationPhAnox(converter.f[0]);

                    break;

                case SimulationSensor::SENSOR_PH_AIREACION:
                    emit setSimulationPhAireacion(converter.f[0]);

                    break;

                case SimulationSensor::SENSOR_MOTOR_CURRENT:
                    emit setSimulationMotorCurrent(converter.f[0]);

                    break;

                case SimulationSensor::SENSOR_MOTOR_VOLTAJE:
                    emit setSimulationMotorVoltaje(converter.f[0]);

                    break;

                case SimulationSensor::SENSOR_MOTOR_TEMP:
                    emit setSimulationMotorTemp(converter.f[0]);

                    break;

                case SimulationSensor::SENSOR_MOTOR_VELOCITY:
                    emit setSimulationMotorVelocity(converter.f[0]);

                    break;
            }

            sendRequestSetParam((SimulationSensor)(payload.at(0)));

            break;
        }

        default:
            break;
    }
}
