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
    SocketThread(int ID, QObject *parent);
    void setDatabase(Database *database);
    void run() override;
    QString getUsername(){ return this->username; }
    void setUsername(QString username){ this->username = username; }
private slots:
    void slotReadyRead();
    void slotDisconnected();
signals:
    void error(QTcpSocket::SocketError socketError);
private:
    QTcpSocket *tcpSocket;
    int socketDescriptor;
    Database *database;
    QString username = NULL;

    void sortData(QByteArray data);
    void response(bool isSuccessful, std::string response);
    void sendMessage(Message message);
};

#endif // SOCKETTHREAD_H
