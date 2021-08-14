#include "logfile.h"

LogFile::LogFile(QObject *parent, const QString &fileName) : QObject(parent)
{
    // Archivo de Log
    if (!QDir(logSubdir).exists())
    {
        QDir().mkdir(logSubdir);
    }

    logFile = new QFile(logSubdir + "/" + fileName, this);

    // Solo añade al final del archivo
    //logFile->open(QIODevice::OpenModeFlag::ReadWrite | QIODevice::OpenModeFlag::Append);

    // Borra el contenido anterior
    logFile->open(QIODevice::OpenModeFlag::ReadWrite);
}

LogFile::~LogFile()
{
    if (logFile->isOpen())
    {
        logFile->close();
    }
}

QString LogFile::getTimeLog()
{
    return QDate::currentDate().toString("yyyy-MM-dd") +
            "-" +
            QTime::currentTime().toString("hh-mm-ss-zzz");
}

bool LogFile::print(const QString &data)
{
    logFile->write(QString(getTimeLog() + ": " + data).toLatin1());

    return logFile->flush();
}

bool LogFile::println(const QString &data)
{
    logFile->write(QString(getTimeLog() + ": " + data + "\r\n").toLatin1());

    return logFile->flush();
}
