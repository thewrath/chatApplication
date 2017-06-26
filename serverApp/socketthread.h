#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QByteArray>
#include <QString>

#include <string.h>
#include <vector>
#include "database.h"

class SocketThread : public QThread
{
    Q_OBJECT
public:
    SocketThread(int ID);
    void setDatabase(Database *database);
    void run() override;
    QString getUsername(){ return this->username; }
    void setUsername(QString username){ this->username = username; }
private slots:
    void slotReadyRead();
    void slotDisconnected();
    void slotMessageFromNetwork(QString sender, QString receiver);
signals:
    void error(QTcpSocket::SocketError socketError);
    void signalNewMessageInDatabase(QString sender, QString receiver);
private:
    QTcpSocket *tcpSocket;
    int socketDescriptor;
    Database *database;
    QString username = NULL;
    bool connected = false;

    void sortData(QByteArray data);
    void response(bool isSuccessful, std::string response);
    void sendMessage(Message message);
};

#endif // SOCKETTHREAD_H
