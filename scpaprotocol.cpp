#include "scpaprotocol.h"

SCPAProtocol::SCPAProtocol(QObject *parent) : QThread(parent)
{
    // Se instancia el timer de timeout
    packageTimeOut = new QTimer(this);

    connect(packageTimeOut, &QTimer::timeout, this, &SCPAProtocol::packageTimeOutTrigger);
}

SCPAProtocol::~SCPAProtocol()
{
    disconnect(packageTimeOut, &QTimer::timeout, this, &SCPAProtocol::packageTimeOutTrigger);
}

void SCPAProtocol::readProtocol(const QByteArray dataToRead)
{
    // Si el buffer no esta lleno se añaden los paquetes
    if ((pendingDataToRead.length()) <= (PACKAGE_MAX_LENGTH * 1024))
    {
        pendingDataToRead.append(dataToRead);
    }

    // Si el hilo de analisis de los datos pendientes no se esta ejecutando se ejecuta
    if (!isRunning())
    {
        start();
    }
}

uint8_t SCPAProtocol::checksum(QByteArray *data)
{
    uint8_t dataXor = 0;

    for (uint16_t i = 0 ; i < data->length() ; i++)
    {
        dataXor ^= data->at(i);
    }

    return dataXor;
}

void SCPAProtocol::run()
{
    // Mientras haya datos por leer
    while (!pendingDataToRead.isEmpty())
    {
        switch (packageState)
        {
            // +
            case 0:
                if (pendingDataToRead.at(packageReadIndex++) == '+')
                {
                    packageState++;

                    packageTimeOut->start(PACKAGE_MAX_TIMEOUT_MS);
                }

                else
                {
                    packageReset();
                }

                break;

            // S
            case 1:
                if (pendingDataToRead.at(packageReadIndex++) == 'S')
                {
                    packageState++;
                }

                else
                {
                    packageReset();
                }

                break;

            // C
            case 2:
                if (pendingDataToRead.at(packageReadIndex++) == 'C')
                {
                    packageState++;
                }

                else
                {
                    packageReset();
                }

                break;

            // P
            case 3:
                if (pendingDataToRead.at(packageReadIndex++) == 'P')
                {
                    packageState++;
                }

                else
                {
                    packageReset();
                }

                break;

            // A
            case 4:
                if (pendingDataToRead.at(packageReadIndex++) == 'A')
                {
                    packageState++;
                }

                else
                {
                    packageReset();
                }

                break;

            // :
            case 5:
                if (pendingDataToRead.at(packageReadIndex++) == ':')
                {
                    packageState++;
                }

                else
                {
                    packageReset();
                }

                break;

            // INFO
            case 6:
                packageState++;

                pendingPackageToRead.info = pendingDataToRead.at(packageReadIndex);

                packageReadIndex++;

                break;

            // LEN1
            case 7:
                packageState++;

                // Paquete en formato LITTLEENDIAN
                if (packageInfo & INFO::F)
                {
                    packagePayloadLength = (uint16_t)(pendingDataToRead.at(packageReadIndex) << 7);
                }

                // Paquete en formato BIGENDIAN
                else
                {
                    packagePayloadLength = (uint16_t)(pendingDataToRead.at(packageReadIndex));
                }

                packageReadIndex++;

                break;

            // LEN2
            case 8:
                packageState++;

                // Paquete en formato LITTLEENDIAN
                if (packageInfo & INFO::F)
                {
                    packagePayloadLength |= (uint16_t)(pendingDataToRead.at(packageReadIndex));
                }

                // Paquete en formato BIGENDIAN
                else
                {
                    packagePayloadLength |= (uint16_t)(pendingDataToRead.at(packageReadIndex) << 7);
                }

                packageReadIndex++;

                break;

            // CMD
            case 9:
                packageState++;

                pendingPackageToRead.cmd = pendingDataToRead.at(packageReadIndex);

                packageReadIndex++;

                break;

            // Payload
            case 10:
                if (packagePayloadLength > 0)
                {
                    pendingPackageToRead.payload.append(pendingDataToRead.at(packageReadIndex));
                    packagePayloadLength--;
                }

                else
                {
                    packageState++;
                }

                packageReadIndex++;

                break;

            // Checksum
            case 11:
                // Si el paquete viene con checksum y es igual
                if ((pendingDataToRead.at(packageReadIndex) == checksum(&pendingPackageToRead.payload)) ||
                    !(pendingPackageToRead.info & INFO::C))
                {
                    packageState++;
                }

                else
                {
                    packageReset();

                    qInfo() << "El checksum del paquete no coincide";
                }

                break;

            // Analisis del paquete
            case 12:
                packageReset();

                break;

            default:
                qCritical() << "Esto no debería estar ocurriendo";

                break;
        }
    }
}

void SCPAProtocol::packageReset()
{
    packageTimeOut->stop();

    pendingDataToRead.remove(0, packageReadIndex + 1);

    pendingPackageToRead.cmd = 0x00;
    pendingPackageToRead.payload.clear();

    packageState = 0;
    packageReadIndex = 0;
    packageInfo = 0;
    packagePayloadLength = 0;
}

void SCPAProtocol::packageRead(const scpaPackage_t paquete)
{

}

void SCPAProtocol::packageTimeOutTrigger()
{
    packageReset();

    qWarning() << "TimeOut paquete";
}
