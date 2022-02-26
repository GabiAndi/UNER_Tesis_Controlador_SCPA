/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 17/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Punto de entrada del programa.                            */
/*************************************************************/

#include <QCoreApplication>

#include "scpamanager.h"

int main(int argc, char *argv[])
{
    // Aplicacion
    QCoreApplication a(argc, argv);

    // Controlador de aplicacion
    SCPAManager *scpaManager = new SCPAManager();

    scpaManager->setObjectName("SCPAManager");

    scpaManager->init();

    // Si el objeto controlador de la aplicacion se destruye, el programa se cierra
    QObject::connect(scpaManager, &SCPAManager::destroyed, &a, &QCoreApplication::quit);

    // Bucle de eventos
    return a.exec();
}
