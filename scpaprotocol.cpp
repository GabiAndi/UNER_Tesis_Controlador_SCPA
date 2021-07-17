#include "scpaprotocol.h"

SCPAProtocol::SCPAProtocol(QObject *parent) : QThread(parent)
{
    // Se instancia el timer
    packageTimeOut = new QTimer(this);

    connect(packageTimeOut, &QTimer::timeout, this, &SCPAProtocol::packageTimeOutTrigger);
}

SCPAProtocol::~SCPAProtocol()
{
    disconnect(packageTimeOut, &QTimer::timeout, this, &SCPAProtocol::packageTimeOutTrigger);
}

void SCPAProtocol::readProtocol(const QByteArray dataToRead)
{
    pendingDataToRead.append(dataToRead);

    if (!isRunning())
    {
        start();
    }
}

void SCPAProtocol::run()
{
    // Verifica si el paquete se recibio completamente
    uint16_t lengthPackage = 0;

    // Mientras haya datos disponibles por leer
    while (!pendingDataToRead.isEmpty())
    {
        qInfo() << pendingDataToRead.at(0);

        // Analisis del paquete
        switch (packageState)
        {
            // +
            case 0:
                if (pendingDataToRead.at(0) == '+')
                {
                    packageState++;
                }

                break;

            // S
            case 1:
                if (pendingDataToRead.at(0) == 'S')
                {
                    packageState++;
                }

                else
                {
                    packageState = 0;
                }

                break;

            // C
            case 2:
                if (pendingDataToRead.at(0) == 'C')
                {
                    packageState++;
                }

                else
                {
                    packageState = 0;
                }

                break;

            // P
            case 3:
                if (pendingDataToRead.at(0) == 'P')
                {
                    packageState++;
                }

                else
                {
                    packageState = 0;
                }

                break;

            // A
            case 4:
                if (pendingDataToRead.at(0) == 'A')
                {
                    packageState++;
                }

                else
                {
                    packageState = 0;
                }

                break;

            // :
            case 5:
                if (pendingDataToRead.at(0) == ':')
                {
                    packageState++;
                }

                else
                {
                    packageState = 0;
                }

                break;

            // INFO
            case 6:
                packageState++;

                pendingPackageToRead.append(pendingDataToRead.at(0));

                break;

            // LEN1
            case 7:
                packageState++;

                pendingPackageToRead.append(pendingDataToRead.at(0));

                break;

            // LEN2
            case 8:
                packageState++;

                pendingPackageToRead.append(pendingDataToRead.at(0));

                if ((pendingPackageToRead.at(0) & (1 << 7)) == 0)
                {
                    lengthPackage = ((uint8_t)(pendingDataToRead.at(1))) | ((uint8_t)(pendingDataToRead.at(2)) << 8);
                }

                else
                {
                    lengthPackage = ((uint8_t)(pendingDataToRead.at(1)) << 8) | ((uint8_t)(pendingDataToRead.at(2)));
                }

                break;

            // CMD
            case 9:
                packageState++;

                pendingPackageToRead.append(pendingDataToRead.at(0));

                break;

            // PAYL
            case 10:
                packageState++;


                break;
        }

        // Una vez leido el dato se elimina de los bytes pendientes por leer
        pendingDataToRead.remove(0, 1);
    }

    qInfo() << "Paquete listo para analizar";
    qInfo() << pendingPackageToRead;
}

void SCPAProtocol::packageTimeOutTrigger()
{

}
