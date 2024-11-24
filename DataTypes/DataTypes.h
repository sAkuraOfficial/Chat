#pragma once
#include <QByteArray>
#include <qdatetime.h>
#include <qstring.h>
#include <qwebsocket.h>
class friend_status_code
{
  public:
    enum class code
    {
        ACCEPTED,
        PENDING,
        BLOCKED,
        UNKNOWN
    };
    static QString GetStatusCode(code status)
    {
        switch (status)
        {
        case code::ACCEPTED:
            return "ACCEPTED";
        case code::PENDING:
            return "PENDING";
        case code::BLOCKED:
            return "BLOCKED";
        case code::UNKNOWN:
            return "UNKNOWN";
        default:
            return "UNKNOWN";
        }
    }
    static code GetStatusCode(QString status)
    {
        if (status == "ACCEPTED")
            return code::ACCEPTED;
        if (status == "PENDING")
            return code::PENDING;
        if (status == "BLOCKED")
            return code::BLOCKED;
        return code::UNKNOWN;
    }
};




struct user_info
{
    QString username;
    int user_id;
    bool isOnline;
    QByteArray avatar;
};

struct message_info
{
    QString message;
    user_info sender;
    user_info receiver;
    bool receiver_is_client;
    QDateTime time; // 年月日时分秒
};

struct friend_info : public user_info
{
    friend_status_code::code status;
    message_info last_message;
};

struct client_info : public user_info
{
    QString username;
    int user_id;
    bool isOnline;
    QWebSocket *socket;
};
