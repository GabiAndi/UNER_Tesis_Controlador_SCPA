/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 21/07/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Clase encargada de proporcionar la interfaz de texto      */
/* para la interacción con el usuario.                       */
/*************************************************************/

#ifndef TUIMANAGER_H
#define TUIMANAGER_H

#include <QObject>
#include <QTextStream>

#include "logfile.h"

class TUIManager : public QObject
{
        Q_OBJECT

    public:
        explicit TUIManager(QObject *parent = nullptr);
        ~TUIManager();

        void loop();

    private:
        // Log
        LogFile *logFile = nullptr;

        bool exitFlag = false;

        int getCommand(const QString &cmd);

        void commandClose();

        typedef enum
        {
            unknown = -1,
            exit = 0,
            help = 1,
        }commands_e;
};

#endif // TUIMANAGER_H
