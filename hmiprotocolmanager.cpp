#include "hmiprotocolmanager.h"

HMIProtocolPackage::HMIProtocolPackage(QObject *parent) : QObject(parent)
{

}

HMIProtocolPackage::~HMIProtocolPackage()
{

}

HMIProtocolManager::HMIProtocolManager(QObject *parent) : QObject(parent)
{

}

HMIProtocolManager::~HMIProtocolManager()
{
    delete hmiProtocolPackage;
}

void HMIProtocolManager::init()
{
    hmiProtocolPackage = new HMIProtocolPackage(this);
}

void HMIProtocolManager::readProtocol(const QByteArray data)
{
    // La posicion de lectura sera a partir de los nuevos datos
    int readIndex = hmiProtocolPackage->packageReadData.length();

    // Se añaden los datos nuevos a los anteriores, en el caso de que el paquete venga por partes
    hmiProtocolPackage->packageReadData.append(data);

    // Hasta que no se terminen de analizar todos los datos en cola
    while (readIndex < hmiProtocolPackage->packageReadData.length())
    {
        // Maquina de estados
        switch (hmiProtocolPackage->packageReadState)
        {
            // Primer byte de cabecera
            case 0:
                if (hmiProtocolPackage->packageReadData.at(readIndex) == 'S')
                {
                    hmiProtocolPackage->packageReadState = 1;
                }

                break;

            // Segundo byte de cabecera
            case 1:
                if (hmiProtocolPackage->packageReadData.at(readIndex) == 'C')
                {
                    hmiProtocolPackage->packageReadState = 2;
                }

                else
                {
                    hmiProtocolPackage->packageReadState = 0;
                }

                break;

            // Tercer byte de cabecera
            case 2:
                if (hmiProtocolPackage->packageReadData.at(readIndex) == 'P')
                {
                    hmiProtocolPackage->packageReadState = 3;
                }

                else
                {
                    hmiProtocolPackage->packageReadState = 0;
                }

                break;

            // Cuarto byte de cabecera
            case 3:
                if (hmiProtocolPackage->packageReadData.at(readIndex) == 'A')
                {
                    hmiProtocolPackage->packageReadState = 4;
                }

                else
                {
                    hmiProtocolPackage->packageReadState = 0;
                }

                break;
        }

        // Se aumenta el indice de lectura
        readIndex++;
    }
}
