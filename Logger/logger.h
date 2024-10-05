#pragma once
#include <QFile>
#include <QMutex>
#include <QObject>
#include <QString>

// 采用单例模式
class Logger : public QObject
{
    Q_OBJECT

  public:
    enum LogLevel
    {
        INFO,
        WARNING,
        ERROR
    };
    void log(const QString &msg, LogLevel level = INFO);
    static Logger &getInstance();
    static void outputLog(const QString &msg, LogLevel level = INFO);
  private:
    QMutex m_mutex;
    QString formatLogString(const QString &msg, LogLevel level);
    Logger(QObject *parent = nullptr);
    ~Logger();
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

  signals:
    void newLog_Formated(const QString &msg);        // 新的日志,格式化后的
    void newLog(const QString &msg, LogLevel level); // 新的日志,未格式化
  private slots:
};
