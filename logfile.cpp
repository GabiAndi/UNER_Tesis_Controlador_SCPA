#include "logfile.h"

LogFile::LogFile(QObject *parent, const QString &fileName) : QObject(parent)
{
    // Archivo de Log
    if (!QDir(logSubdir).exists())
    {
        QDir().mkdir(logSubdir);
    }

    logFile = new QFile(logSubdir + "/" + fileName, this);

    // Si el archivo existe se elimina
    if (logFile->exists())
    {
        logFile->remove();
    }

    // Se abre el archivo para añadir contenido al final
    logFile->open(QIODevice::OpenModeFlag::ReadWrite | QIODevice::OpenModeFlag::Append);
}

LogFile::~LogFile()
{
    // Si el archivo quedo abierto, antes de eliminarse se cierra
    if (logFile->isOpen())
    {
        logFile->flush();
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
