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

void HMIUser::sendSensorValue(Sensor sensor, float value)
{
    DataConverter converter;

    converter.f[0] = value;

    QByteArray data;

    data.append(sensor);
    data.append(converter.u8[0]);
    data.append(converter.u8[1]);
    data.append(converter.u8[2]);
    data.append(converter.u8[3]);

    hmiProtocol->write(Command::REQUEST_GET_SENSOR, data);
}

void HMIUser::sendRequestSetSensor(Sensor sensor)
{
    hmiProtocol->write(Command::REQUEST_SET_SENSOR, QByteArray().append(sensor));
}

void HMIUser::sendSystemState(hmiprotocoldata::SystemState state, float value)
{
    DataConverter converter;

    converter.f[0] = value;

    QByteArray data;

    data.append(state);
    data.append(converter.u8[0]);
    data.append(converter.u8[1]);
    data.append(converter.u8[2]);
    data.append(converter.u8[3]);

    hmiProtocol->write(Command::REQUEST_GET_SYSTEM_STATE, data);
}

void HMIUser::sendRequestSetSystemState(hmiprotocoldata::SystemState state)
{
    hmiProtocol->write(Command::REQUEST_SET_SYSTEM_STATE, QByteArray().append(state));
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
         * GET_SENSOR
         *
         * Se pide los valores de los sensores del sistema
         */
        case Command::GET_SENSOR:
        {
            emit getSensorValue((Sensor)(payload.at(0)));

            break;
        }

        /*
         * SET_SENSOR
         *
         * Se establece los valores de los sensores para la simulación
         */
        case Command::SET_SENSOR:
        {
            DataConverter converter;

            converter.u8[0] = payload.at(1);
            converter.u8[1] = payload.at(2);
            converter.u8[2] = payload.at(3);
            converter.u8[3] = payload.at(4);

            emit setSensorValue((Sensor)(payload.at(0)), converter.f[0]);

            sendRequestSetSensor((Sensor)(payload.at(0)));

            break;
        }

        /*
         * GET_SYSTEM_STATE
         *
         * Lee un estado del sistema
         */
        case Command::GET_SYSTEM_STATE:
        {
            emit getSystemState((SystemState)(payload.at(0)));

            break;
        }

        /*
         * SET_SYSTEM_STATE
         *
         * Establece un estado
         */
         case Command::SET_SYSTEM_STATE:
        {
            DataConverter converter;

            converter.u8[0] = payload.at(1);
            converter.u8[1] = payload.at(2);
            converter.u8[2] = payload.at(3);
            converter.u8[3] = payload.at(4);

            emit setSystemState((SystemState)(payload.at(0)), converter.f[0]);

            sendRequestSetSystemState((SystemState)(payload.at(0)));

            break;
        }

        default:
            break;
    }
}
