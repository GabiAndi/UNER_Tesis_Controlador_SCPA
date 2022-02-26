#include "metricsmanager.h"

MetricsManager::MetricsManager(QObject *parent)
    : QObject{parent}
{

}

MetricsManager::~MetricsManager()
{
    delete database;
}
