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

class TUIManager : public QObject
{
        Q_OBJECT

    public:
        explicit TUIManager(QObject *parent = nullptr);
        ~TUIManager();

        void loop();

    private:
        bool exitFlag = false;

        void commandClose();
};

#endif // TUIMANAGER_H
