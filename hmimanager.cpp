#include "hmimanager.h"

HMIManager::HMIManager(QObject *parent) : QObject(parent)
{
    // Log
    logFile = new LogFile(this, "HMIManager.txt");

    logFile->println("Iniciando servidor HMI");

    // Inicializacion del servidor
    hmiServer = new QTcpServer(this);

    // Inicialización de la lista de clientes
    hmiClients = new QList<HMIClient *>();
}

HMIManager::~HMIManager()
{
    logFile->println("Servidor HMI cerrado");

    delete hmiClients;
}

void HMIManager::start()
{
    // Conectamos las señales del servidor tcp
    connect(hmiServer, &QTcpServer::newConnection, this, &HMIManager::newConnection);
    connect(hmiServer, &QTcpServer::acceptError, this, &HMIManager::newConnectionError);

    if (hmiServer->listen(QHostAddress::AnyIPv4, 33600))
    {
        logFile->println("Servidor HMI iniciado");
    }

    else
    {
        logFile->println("Error al iniciar el servidor HMI");
    }
}

void HMIManager::closeAplication()
{
    // Marcamos la bandera de cierre
    closingAplication = true;

    // Si hay clientes conectados, enviamos la señal para cerrar todas las conexiones
    if (hmiClients->length() > 0)
    {
        closeAllClients();
    }

    // Si no hay clientes cierro el hilo
    else
    {
        exit(0);
    }
}

void HMIManager::closeAllClients()
{
    // Se cierra la conexión de todos los cliente
    for (int i = 0 ; i < hmiClients->length() ; i++)
    {
        hmiClients->at(i)->disconnectClient();
    }
}

int HMIManager::getID()
{
    bool hmiAceptNewConnection = true;

    for (int newId = 0 ; newId < hmiClientsMax ; newId++)
    {
        hmiAceptNewConnection = true;

        for (int i = 0 ; i < hmiClients->length() ; i++)
        {
            if (hmiClients->at(i)->getId() == newId)
            {
                hmiAceptNewConnection = false;

                break;
            }
        }

        if (hmiAceptNewConnection)
        {
            return newId;
        }
    }

    return -1;
}

void HMIManager::newConnection()
{
    logFile->println("Nueva conexion entrante");

    int newId = getID();

    if (newId != -1)
    {
        logFile->println(QString::asprintf("Conexion aceptada con id: %d", newId));

        HMIClient *newClient = new HMIClient(this, hmiServer->nextPendingConnection(), newId);

        hmiClients->append(newClient);

        connect(newClient, &HMIClient::hmiClientClosed, this, &HMIManager::hmiClientDisconnected);
    }

    else
    {
        logFile->println("Conexion rechasada");
    }
}

void HMIManager::newConnectionError(const QAbstractSocket::SocketError socketError)
{
    logFile->println(QString::asprintf("Error en la conexion entrante: %d", socketError));
}

void HMIManager::hmiClientDisconnected(HMIClient *hmiClient)
{
    // Se cierra la conexión del cliente eliminandolo de la lista de clientes activos
    logFile->println(QString::asprintf("Conexion cerrada del cliente id: %d", hmiClient->getId()));

    hmiClients->removeOne(hmiClient);

    disconnect(hmiClient, &HMIClient::hmiClientClosed, this, &HMIManager::hmiClientDisconnected);

    // Si la aplicacion fue marcada para cerrarse y no hay mas clientes se termina el proceso
    if ((closingAplication) && (hmiClients->length() == 0))
    {
        exit(0);
    }
}
