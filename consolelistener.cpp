#include "consolelistener.h"

ConsoleListener::ConsoleListener(QObject *parent)
    : QObject{parent}
{

}

ConsoleListener::~ConsoleListener()
{
    // Se elimina la memoria
    delete socketNotifier;
}

void ConsoleListener::init()
{
    // Socket notifier que esta acargo de la entrada por consola
    socketNotifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Type::Read);

    connect(socketNotifier, &QSocketNotifier::activated, this, &ConsoleListener::getKeyboard);
}

void ConsoleListener::getKeyboard()
{
    int key = getch();

    emit newKey(key);
}
