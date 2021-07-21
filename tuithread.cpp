#include "tuithread.h"

TUIThread::TUIThread(QObject *parent) : QThread(parent)
{

}

TUIThread::~TUIThread()
{

}

void TUIThread::run()
{
    // Entrada y salida de consola
    QTextStream inputStream(stdin, QIODevice::OpenModeFlag::ReadOnly);
    QTextStream outputStream(stdout, QIODevice::OpenModeFlag::WriteOnly);

    // Comando
    QString cmd;

    // Bucle de captura
    bool exitFlag = false;

    // Bucle de TUI
    while (!exitFlag)
    {
        outputStream << "SCPA: ";
        outputStream.flush();

        cmd = inputStream.readLine();

        outputStream << Qt::endl;

        if (cmd == "exit")
        {
            exitFlag = true;
        }
    }
}
