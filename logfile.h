/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 17/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Esta clase proporciona una forma de escribir archivos     */
/* de logs sobre distintos eventos sucedidos en la           */
/* aplicacion.                                               */
/*************************************************************/

#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QDate>
#include <QTime>
#include <QDebug>

class LogFile : public QObject
{
        Q_OBJECT

    public:
        explicit LogFile(QObject *parent = nullptr);
        ~LogFile();

        bool create(const QString &fileName);
        void close();

        QString getTimeLog();

        bool print(const QString &logData);
        bool println(const QString &logData);

    private:
        QFile file;

        const QString logSubdir = "logs";
};

#endif // LOGFILE_H
