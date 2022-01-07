#include "hmiprotocolmanager.h"

HMIProtocolManager::HMIProtocolManager(QObject *parent)
    : QObject{parent}
{

}

HMIProtocolManager::~HMIProtocolManager()
{
    // Paquete
    delete hmi_protocol_package;

    // Timer
    delete timeOutTimer;
}

void HMIProtocolManager::newCommand(uint8_t &cmd, QByteArray &payload)
{
    // Se analiza el comando recibido
    switch (cmd)
    {
        case 0xA0:
            if (payload.length() != 1)
                break;

            if ((uint8_t)(payload.at(0)) != (uint8_t)(0xFF))
                break;

            // Codigo
            break;

        default:
            break;
    }
}

void HMIProtocolManager::init()
{
    // Paquete de datos
    hmi_protocol_package = new hmi_protocol_package_t;

    hmi_protocol_package->dataMaxLength = 1;  // Tamaño en Kb maximo del buffer de lectura
    hmi_protocol_package->timeOutMs = 100;  // Tiempo maximo en ms hasta que se descarta el paquete

    hmi_protocol_package->packageReadData.clear();

    hmi_protocol_package->packageReadIndex = 0;
    hmi_protocol_package->packageReadState = 0;

    hmi_protocol_package->packageReadPayloadLength = 0;
    hmi_protocol_package->packageReadCmd = 0x00;
    hmi_protocol_package->packageReadPayload.clear();

    // Timer de TimeOut
    timeOutTimer = new QTimer(this);

    connect(timeOutTimer, &QTimer::timeout, this, &HMIProtocolManager::readReset);
}

void HMIProtocolManager::readProtocol(const QByteArray data)
{
    /*
     * Se añaden los datos nuevos a los anteriores, en el caso de que el paquete venga por partes.
     * Solo si el tamaño no supera al maximo permitido.
     */
    if ((hmi_protocol_package->packageReadData.length() + data.length()) <=
            (hmi_protocol_package->dataMaxLength * 1024))
    {
        hmi_protocol_package->packageReadData.append(data);
    }

    // Hasta que no se terminen de analizar todos los datos en cola
    while (hmi_protocol_package->packageReadIndex < hmi_protocol_package->packageReadData.length())
    {
        // Se lee el proximo byte del buffer si esta disponible y se aumenta el indice de lectura
        uint8_t readByte = hmi_protocol_package->packageReadData.at(hmi_protocol_package->packageReadIndex++);

        // Maquina de estados
        switch (hmi_protocol_package->packageReadState)
        {
            // Primer byte de cabecera
            case 0:
                if (readByte == 'S')
                {
                    timeOutTimer->start(hmi_protocol_package->timeOutMs);

                    hmi_protocol_package->packageReadState = 1;
                }

                else
                {
                    readReset();
                }

                break;

            // Segundo byte de cabecera
            case 1:
                if (readByte == 'C')
                {
                    hmi_protocol_package->packageReadState = 2;
                }

                else
                {
                    readReset();
                }

                break;

            // Tercer byte de cabecera
            case 2:
                if (readByte == 'P')
                {
                    hmi_protocol_package->packageReadState = 3;
                }

                else
                {
                    readReset();
                }

                break;

            // Cuarto byte de cabecera
            case 3:
                if (readByte == 'A')
                {
                    hmi_protocol_package->packageReadState = 4;
                }

                else
                {
                    readReset();
                }

                break;

            // Byte de token
            case 4:
                if (readByte == ':')
                {
                    hmi_protocol_package->packageReadState = 5;
                }

                else
                {
                    readReset();
                }

                break;

            // Byte 1 de tamaño
            case 5:
                hmi_protocol_package->packageReadPayloadLength = (uint16_t)(readByte);

                hmi_protocol_package->packageReadState = 6;

                break;

            // Byte 2 de tamaño
            case 6:
                hmi_protocol_package->packageReadPayloadLength |= ((uint16_t)(readByte) << 8);

                hmi_protocol_package->packageReadState = 7;

                break;

            // Comando
            case 7:
                hmi_protocol_package->packageReadCmd = readByte;

                hmi_protocol_package->packageReadState = 8;

                break;

            // Payload
            case 8:
                // Si quedan datos del payload por leer
                if (hmi_protocol_package->packageReadPayloadLength > 0)
                {
                    hmi_protocol_package->packageReadPayload.append(readByte);

                    hmi_protocol_package->packageReadPayloadLength--;
                }

                // Si se recibio todo el paquete
                if (hmi_protocol_package->packageReadPayloadLength == 0)
                {
                    // Se analiza el paquete
                    newCommand(hmi_protocol_package->packageReadCmd, hmi_protocol_package->packageReadPayload);

                    readReset();
                }

                break;

            // Caso por defecto
            default:
                readReset();

                break;
        }
    }
}

void HMIProtocolManager::readReset()
{
    timeOutTimer->stop();

    hmi_protocol_package->packageReadData.remove(0, hmi_protocol_package->packageReadIndex);

    hmi_protocol_package->packageReadIndex = 0;
    hmi_protocol_package->packageReadState = 0;

    hmi_protocol_package->packageReadPayloadLength = 0;
    hmi_protocol_package->packageReadCmd = 0x00;
    hmi_protocol_package->packageReadPayload.clear();
}
