#include "logger.h"
#include <QDateTime>
Logger::Logger(QObject *parent)
    : QObject(parent)
{
}

Logger::~Logger()
{
}

void Logger::log(const QString &msg, LogLevel level)
{
    QMutexLocker locker(&m_mutex);
    QString logString = formatLogString(msg, level);
    qDebug() << logString;
    emit newLog_Formated(logString);
    emit newLog(msg, level);
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::outputLog(const QString &msg, LogLevel level)
{
    Logger::getInstance().log(msg, level);
}

QString Logger::formatLogString(const QString &msg, LogLevel level)
{
    QString strLogLevel;
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch (level)
    {
    case INFO:
        strLogLevel = "INFO";
        break;
    case WARNING:
        strLogLevel = "WARNING";
        break;
    case ERROR:
        strLogLevel = "ERROR";
        break;
    default:
        strLogLevel = "UNKNOWN";
        break;
    }
    return QString("[%1] [%2] %3")
        .arg(dateTime)
        .arg(strLogLevel)
        .arg(msg);
}