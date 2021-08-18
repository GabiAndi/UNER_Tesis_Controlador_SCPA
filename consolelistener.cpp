#include "consolelistener.h"

ConsoleListener::ConsoleListener(QObject *parent) : QObject(parent)
{
    // Nuevo hilo
    threadNotifier = new QThread(this);

    // Socket notifier que esta acargo de la entrada por consola
    socketNotifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Type::Read);

    socketNotifier->moveToThread(threadNotifier);

    connect(socketNotifier, &QSocketNotifier::activated, this, &ConsoleListener::getLine);

    threadNotifier->start();
}

ConsoleListener::~ConsoleListener()
{
    // Se detiene el hilo de lectura de la consola
    threadNotifier->quit();
    threadNotifier->wait();

    // Se elimina la memoria
    delete socketNotifier;
    delete threadNotifier;
}

void ConsoleListener::getLine()
{
    std::string line;

    std::getline(std::cin, line);

    emit newLine(QString::fromStdString(line));
}
