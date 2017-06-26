#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include "utils.h"

class Database
{
public:
    Database();
    bool registerUser(QString username, QString password, QString email);
    bool loginUser(QString username, QString password);
    bool checkEmailAndUsername(QString username, QString email);
    bool insertMessage(QString sender, QString receiver, QString content);
    Message giveMessageToReceiver(QString senderName, QString receiverName);
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
