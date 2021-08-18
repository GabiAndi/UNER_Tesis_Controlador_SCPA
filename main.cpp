/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 17/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Codigo principal del programa.                            */
/*************************************************************/

#include <QCoreApplication>

#include "scpamanager.h"

int main(int argc, char *argv[])
{
    // Aplicacion
    QCoreApplication a(argc, argv);

    // Controlador de aplicacion
    SCPAManager *scpaManager = new SCPAManager();

    QObject::connect(scpaManager, &SCPAManager::destroyed, &a, &QCoreApplication::quit);

    scpaManager->init();

    // Bucle de eventos
    return a.exec();
}
