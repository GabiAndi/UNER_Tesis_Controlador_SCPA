#include "applicationstate.h"

ApplicationState::ApplicationState(QObject *parent)
    : QObject{parent}
{

}

ApplicationState::~ApplicationState()
{

}

void ApplicationState::closeApplication()
{
    emit closedApplication();
}
