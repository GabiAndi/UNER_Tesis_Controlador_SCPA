#include "hmiprotocolmanager.h"

HMIProtocolManager::HMIProtocolManager(QObject *parent) : QObject(parent)
{

}

HMIProtocolManager::~HMIProtocolManager()
{
    // Paquete
    delete hmiProtocolPackage;

    // Timer
    delete readProtocolTimer;
}

void HMIProtocolManager::hmiNewCommand(uint8_t &cmd, QByteArray &payload)
{
    // Se analiza el comando recibido
    switch (cmd)
    {
        case 0xFF:

            break;

        default:

            break;
    }
}

void HMIProtocolManager::init()
{
    hmiProtocolPackage = new HMIProtocolPackage(this);

    // Timer de TimeOut
    readProtocolTimer = new QTimer(this);

    connect(readProtocolTimer, &QTimer::timeout, this, &HMIProtocolManager::readProtocolReset);
}

void HMIProtocolManager::readProtocol(const QByteArray data)
{
    /*
     * Se añaden los datos nuevos a los anteriores, en el caso de que el paquete venga por partes.
     * Solo si el tamaño no supera al maximo permitido.
     */
    if ((hmiProtocolPackage->packageReadData.length() + data.length()) <= (readProtocolDataMaxLength * 1024))
    {
        hmiProtocolPackage->packageReadData.append(data);
    }

    // Hasta que no se terminen de analizar todos los datos en cola
    while (hmiProtocolPackage->packageReadAvailable())
    {
        // Se lee el proximo byte del buffer si esta disponible y se aumenta el indice de lectura
        uint8_t readByte = hmiProtocolPackage->packageReadNextByte();

        // Maquina de estados
        switch (hmiProtocolPackage->packageReadState)
        {
            // Primer byte de cabecera
            case 0:
                if (readByte == 'S')
                {
                    readProtocolTimer->start(readProtocolTimeOutMs);

                    hmiProtocolPackage->packageReadState = 1;
                }

                else
                {
                    readProtocolReset();
                }

                break;

            // Segundo byte de cabecera
            case 1:
                if (readByte == 'C')
                {
                    hmiProtocolPackage->packageReadState = 2;
                }

                else
                {
                    readProtocolReset();
                }

                break;

            // Tercer byte de cabecera
            case 2:
                if (readByte == 'P')
                {
                    hmiProtocolPackage->packageReadState = 3;
                }

                else
                {
                    readProtocolReset();
                }

                break;

            // Cuarto byte de cabecera
            case 3:
                if (readByte == 'A')
                {
                    hmiProtocolPackage->packageReadState = 4;
                }

                else
                {
                    readProtocolReset();
                }

                break;

            // Byte de token
            case 4:
                if (readByte == ':')
                {
                    hmiProtocolPackage->packageReadState = 5;
                }

                else
                {
                    readProtocolReset();
                }

                break;

            // Byte 1 de tamaño
            case 5:
                hmiProtocolPackage->packageReadPayloadLength = (uint16_t)(readByte);

                hmiProtocolPackage->packageReadState = 6;

                break;

            // Byte 2 de tamaño
            case 6:
                hmiProtocolPackage->packageReadPayloadLength |= ((uint16_t)(readByte) << 8);

                hmiProtocolPackage->packageReadState = 7;

                break;

            // Comando
            case 7:
                hmiProtocolPackage->packageReadCmd = readByte;

                hmiProtocolPackage->packageReadState = 8;

                break;

            // Payload
            case 8:
                // Si quedan datos del payload por leer
                if (hmiProtocolPackage->packageReadPayloadLength > 0)
                {
                    hmiProtocolPackage->packageReadPayload.append(readByte);

                    hmiProtocolPackage->packageReadPayloadLength--;
                }

                else
                {
                    // Se analiza el paquete
                    hmiNewCommand(hmiProtocolPackage->packageReadCmd, hmiProtocolPackage->packageReadPayload);

                    // Finalizo correctamento
                    if (readByte == 0xFF)
                    {

                    }

                    // Error de formato de paquete
                    else
                    {

                    }

                    readProtocolReset();
                }

                break;

            // Caso por defecto
            default:
                readProtocolReset();

                break;
        }
    }
}

void HMIProtocolManager::readProtocolReset()
{
    readProtocolTimer->stop();
    hmiProtocolPackage->packageReadReset();
}
