/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 07/07/2021                                         */
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

    // Intancia la clase controladora principal
    SCPAManager *scpaManager = new SCPAManager();

    scpaManager->start();

    // Bucle infinito
    int exit = a.exec();

    // Operaciones de cierre de la aplicacion
    delete scpaManager;

    // Devuelve la causa del cierre del programa
    return exit;
}
