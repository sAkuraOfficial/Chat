#pragma once
#include <qdatetime.h>
#include <qstring.h>
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

struct message_info
{
    QString message;
    QString sender;
    QString receiver;
    QDateTime time; // 年月日时分秒
};
;

struct user_info
{
    QString username;
    int user_id;
    bool isOnline;
};

struct friend_info : public user_info
{
    friend_status_code::code status;
    message_info last_message;
};