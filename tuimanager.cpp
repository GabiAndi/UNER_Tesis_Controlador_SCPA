#include "tuimanager.h"

TUIManager::TUIManager(QObject *parent) : QObject(parent)
{

}

TUIManager::~TUIManager()
{

}

void TUIManager::loop()
{
    // Entrada y salida de consola
    QTextStream inputStream(stdin, QIODevice::OpenModeFlag::ReadOnly);
    QTextStream outputStream(stdout, QIODevice::OpenModeFlag::WriteOnly);

    // Comando
    QString cmd;

    // Bucle de TUI
    while (!exitFlag)
    {
        // Cabecera
        outputStream << "SCPA: ";
        outputStream.flush();

        // Lectura de datos
        cmd = inputStream.readLine();

        outputStream << Qt::endl;

        // Comando de cierre de programa
        if (cmd == "exit")
        {
            commandClose();
        }
    }

    // Eventos pre cierre
    outputStream << "Cerrando SCPA" << Qt::endl;
}

void TUIManager::commandClose()
{
    // Se marca la aplicacion para el cierre
    exitFlag = true;
}
