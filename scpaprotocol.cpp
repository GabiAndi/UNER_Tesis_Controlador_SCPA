#include "scpaprotocol.h"

SCPAProtocol::SCPAProtocol(QObject *parent) : QThread(parent)
{

}

SCPAProtocol::~SCPAProtocol()
{

}

void SCPAProtocol::abortReadProtocol()
{
    abort = true;
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
    while ((!pendingDataToRead.isEmpty()) && (!abort))
    {
        qDebug() << pendingDataToRead.at(0);
        pendingDataToRead.remove(0, 1);
        msleep(1000);
    }
}
