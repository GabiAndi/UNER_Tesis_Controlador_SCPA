#include "tuimanager.h"

TUIManager::TUIManager(QObject *parent)
    : QObject{parent}
{

}

TUIManager::~TUIManager()
{
    // Detenemos el controlador de consola
    consoleThread->quit();
    consoleThread->wait();

    delete consoleListener;
    delete consoleThread;

    // Cierre del archivo de log
    logFile->println("Finalizado");

    // Se borran los recursos utilizados
    delete logFile;
}

void TUIManager::init()
{
    // Archivo de log
    logFile = new LogFile(this);

    logFile->println("Iniciado");

    // Iniciamos la pantalla
    // Iniciamos ncurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    // Iniciamos la captura de la consola
    consoleThread = new QThread(this);
    consoleListener = new ConsoleListener();

    consoleListener->moveToThread(consoleThread);

    connect(consoleThread, &QThread::started, consoleListener, &ConsoleListener::init);

    connect(consoleListener, &ConsoleListener::newKey, this, &TUIManager::consoleNewKey);

    consoleThread->start();

    // Mostrar mensajes por consola
    /*consoleWelcome();
    consoleWait();*/

    // Mensaje de inicio
    logFile->println("Cargado");
}

void TUIManager::hmiServerStatus(hmi_server_status_t status)
{
    /**consoleOutput << "IP servidor: " << status.serverIP << Qt::endl;
    *consoleOutput << "IP cliente: " << status.clientIP << Qt::endl;
    *consoleOutput << "Puerto usado: " << status.port << Qt::endl;

    consoleWait();*/
}

void TUIManager::consoleNewKey(const int key)
{
    clear();

    printw ("La tecla tiene codigo: ");
            attron (A_BOLD);
            printw ("%d\n", key);
            attroff (A_BOLD);

    refresh();

    // Comando de cierre con la tecla ctl + q
    if (key == 17)
    {
        emit closedApplication();
    }

    /*// Comando para limpiar la pantalla
    else if (cmd == "clear")
    {
        consoleClear();
        consoleWelcome();
        consoleWait();
    }

    // Comando para consultar el estado del servidor HMI
    else if (cmd == "hmi status")
    {
        emit getHmiServerStatus();
    }

    // Comando no conocido
    else
    {
        *consoleOutput << "Comando no valido para: " << line << Qt::endl;
        consoleWait();
    }*/
}

void TUIManager::consoleWelcome()
{
    // Dialogo de bienvenida
    /**printw("----------------------------------------");

    consoleOutput <<  << Qt::endl;
    *consoleOutput << "Controlador del sistema de aireacion." << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Tesis de grado para recibir el" << Qt::endl;
    *consoleOutput << "titulo de Ingeniero en Mecatronica." << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Gianluca Lovatto y Gabriel Aguirre" << Qt::endl;
    *consoleOutput << "Facultad de Ciencias de la" << Qt::endl;
    *consoleOutput << "Alimentacion de la Universidad" << Qt::endl;
    *consoleOutput << "Nacional de Entre Rios." << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;
    *consoleOutput << "Qt: " << QT_VERSION_STR << Qt::endl;
    *consoleOutput << "GLibc: " << gnu_get_libc_version() << Qt::endl;
    *consoleOutput << "----------------------------------------" << Qt::endl;*/
}

void TUIManager::consoleClear()
{
    // Se limpia la pantalla
    /*QProcess::execute("clear", QStringList());*/
}

void TUIManager::consoleWait()
{
    // Mensaje de espera
    /**consoleOutput << "Ingrese comando: ";
    consoleOutput->flush();*/
}

void TUIManager::closeApplication()
{
    endwin();

    emit closedApplication();
}
