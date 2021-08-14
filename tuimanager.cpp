#include "tuimanager.h"

TUIManager::TUIManager(QObject *parent) : QObject(parent)
{
    // Log
    logFile = new LogFile(this, "TUIManager.txt");

    logFile->println("Iniciando");
}

TUIManager::~TUIManager()
{
    logFile->println("Saliendo");
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
        outputStream << "Ingrese comando del SCPA: ";
        outputStream.flush();

        // Lectura de datos
        cmd = inputStream.readLine();

        outputStream << Qt::endl;

        // Se analiza el comando escrito
        switch (getCommand(cmd))
        {
            case exit:
                logFile->println("Se recibio comando de cierre");

                commandClose();

                break;

            case unknown:
                outputStream << "Comando inválido: " << cmd << Qt::endl;

                break;
        }
    }

    // Eventos pre cierre
    outputStream << "Cerrando SCPA" << Qt::endl;
}

int TUIManager::getCommand(const QString &cmd)
{
    // Comando de cierre de programa
    if (cmd == "exit")
    {
        return exit;
    }

    return unknown;
}

void TUIManager::commandClose()
{
    // Se marca la aplicacion para el cierre
    exitFlag = true;
}
