/*************************************************************/
/* AUTOR: GabiAndi                                           */
/* FECHA: 14/08/2021                                         */
/*                                                           */
/* DESCRIPCION:                                              */
/* Provee una forma de escribir logs en un archivo           */
/* personalizada.                                            */
/*************************************************************/

#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QDate>
#include <QTime>

class LogFile : public QObject
{
        Q_OBJECT

    public:
        explicit LogFile(QObject *parent = nullptr, const QString &fileName = "DefaultLog.txt");
        ~LogFile();

        QString getTimeLog();

        bool print(const QString &data);
        bool println(const QString &data);

    private:
        // Log
        QFile *logFile = nullptr;

        const QString logSubdir = "logs";
};

#endif // LOGFILE_H
