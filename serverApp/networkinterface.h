#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <vector>
#include<QTcpServer>
#include<QTcpSocket>
#include "socketthread.h"
#include"database.h"

class NetworkInterface : public QTcpServer
{
    Q_OBJECT
public:
    explicit NetworkInterface(QObject *parent=0);
    void startServer(Database *database);
public slots:
    void slotNewMessageInDatabase(QString sender,QString receiver);
signals:
    void signalNewMessageInDB(QString sender, QString receiver);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
private:
    const int port = 4000;
    QTcpSocket *clientConnection;
    Database *database;
};

#endif // NETWORKINTERFACE_H
