#include "database.h"
#include <QDebug>
#include <QSqlRecord>

Database::Database()
{
    this->db = QSqlDatabase::addDatabase("QMYSQL");
    this->db.setHostName("localhost");
    this->db.setDatabaseName("chatAppDB");
    this->db.setUserName("root");
    this->db.setPassword("root");
    bool ok = this->db.open();

    qDebug() << "Database connection :" << ok;

}

bool Database::registerUser(QString username, QString password, QString email)
{
    if(!checkEmailAndUsername(username, email)){
        QSqlQuery query;
        query.prepare("INSERT INTO users (username, password, email) VALUES (:username, :password, :email);");
        query.bindValue(":username", username);
        query.bindValue(":password", password);
        query.bindValue(":email", email);
        if(query.exec()){
            return true;
        }
    }
    return false;
}

bool Database::loginUser(QString username, QString password)
{

    qDebug() << username;
    qDebug() << password;
    QSqlQuery query;
    //check user in database
    query.prepare("SELECT username FROM users WHERE username = (:username) AND password = (:password) ");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if(query.exec()){
        if(query.next()){
            return true;
        }
    }
    return false;
}

bool Database::checkEmailAndUsername(QString username, QString email)
{
    QSqlQuery query;
    //check user in database
    query.prepare("SELECT username FROM users WHERE username = (:username) AND email = (:email) ");
    query.bindValue(":username", username);
    query.bindValue(":email", email);
    if(query.exec()){
        if(query.next()){
            return true;
        }
    }
    return false;
}

bool Database::insertMessage(QString sender, QString receiver, QString content)
{
    QSqlQuery query;
    query.prepare("INSERT INTO message (sender, receiver, content) VALUES (:sender, :receiver, :content);");
    query.bindValue(":sender", sender);
    query.bindValue(":receiver", receiver);
    query.bindValue(":content", content);
    if(query.exec()){
        return true;
    }
    return false;
}

Message Database::giveMessageToReceiver(QString senderName, QString receiverName)
{
    Message message;
    message.senderName = senderName;
    message.receiverName = receiverName;

    qDebug() << senderName;
    qDebug() << receiverName;

    QSqlQuery query;
    query.prepare("SELECT content FROM message WHERE sender = (:sender) AND receiver = (:receiver)  ORDER BY id DESC LIMIT 1;");
    query.bindValue(":sender", senderName);
    query.bindValue(":receiver", receiverName);
    query.exec();
    if(query.first()){
        QString content = query.value(0).toString();
        message.content = content;
        qDebug() << content;
    }


    return message;


}
