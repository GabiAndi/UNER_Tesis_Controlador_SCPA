#include "tuithread.h"

TUIThread::TUIThread(QObject *parent) : QThread(parent)
{

}

TUIThread::~TUIThread()
{

}

void TUIThread::run()
{
    // Se inicia el TUI
    TUIManager *tuiManager = new TUIManager();

    // Captura de texto
    tuiManager->loop();

    // Se envia la señal de que se cerrara la aplicacion
    emit closing();

    // Bucle para las llamadas
    exec();

    // Se elimina
    delete tuiManager;
}
