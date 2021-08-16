#include "tuithread.h"

TUIThread::TUIThread(QObject *parent) : QThread(parent)
{

}

TUIThread::~TUIThread()
{

}

void TUIThread::finishProcess()
{
    exit(0);
}

void TUIThread::run()
{
    // Se inicia el TUI
    tuiManager = new TUIManager();

    // Captura de texto
    tuiManager->loop();

    // Se envia la señal de que se cerrara la aplicacion
    emit closeAplication();

    // Bucle para las llamadas
    exec();

    // Se elimina
    delete tuiManager;

    tuiManager = nullptr;
}
