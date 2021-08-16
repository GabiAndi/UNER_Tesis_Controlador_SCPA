#include "tuimanager.h"

TUIManager::TUIManager(QObject *parent) : QObject(parent)
{
    // Log
    logFile = new LogFile(this, "TUIManager.txt");

    logFile->println("Iniciando consola");
}

TUIManager::~TUIManager()
{
    logFile->println("Consola finalizada");
}

void TUIManager::loop()
{
    // Entrada y salida de consola
    QTextStream inputStream(stdin, QIODevice::OpenModeFlag::ReadOnly);
    QTextStream outputStream(stdout, QIODevice::OpenModeFlag::WriteOnly);

    // Comando
    QString cmd;

    logFile->println("Capturando consola");

    // Bucle de TUI
    while (!closeAplicationFlag)
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
                commandClose();

                break;

            case unknown:
                outputStream << "Comando invalido: " << cmd << Qt::endl;

                break;
        }
    }

    // Eventos pre cierre
    logFile->println("Cerrando consola");

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
    closeAplicationFlag = true;
}
