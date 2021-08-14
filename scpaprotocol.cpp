#include "scpaprotocol.h"

SCPAProtocol::SCPAProtocol(QObject *parent) : QThread(parent)
{

}

SCPAProtocol::~SCPAProtocol()
{

}

void SCPAProtocol::setExitPending()
{
    exitPending = true;
}

bool SCPAProtocol::getExitPending()
{
    return exitPending;
}

void SCPAProtocol::readProtocol(const QByteArray dataToRead)
{
    // Si el buffer no esta lleno se añaden los paquetes
    if ((pendingDataToRead.length() + dataToRead.length()) <= (PACKAGE_MAX_LENGTH * 1024))
    {
        pendingDataToRead.append(dataToRead);

        // Si el hilo de analisis de los datos pendientes no se esta ejecutando se ejecuta
        if (!isRunning())
        {
            start();
        }
    }
}

uint8_t SCPAProtocol::checksum(QByteArray &data)
{
    uint8_t dataXor = 0;

    for (uint16_t i = 0 ; i < data.length() ; i++)
    {
        dataXor ^= data.at(i);
    }

    return dataXor;
}

void SCPAProtocol::run()
{
    // Mientras haya datos por leer
    while ((!pendingDataToRead.isEmpty()) && (!exitPending))
    {
        pendingDataToRead.at(0);
        pendingDataToRead.remove(0, 1);
    }
}
