#include "networkinterface.h"
#include "socketthread.h"
#include<QDebug>

NetworkInterface::NetworkInterface(QObject *parent) : QTcpServer(parent)
{

}

void NetworkInterface::startServer(Database *database)
{
    this->database = database;
    if(!this->listen(QHostAddress::Any, this->port))
    {
        qDebug() << "Could not start sever ";
    }
    else
    {
        qDebug() << "Listenning to port " << this->port << "...";
    }
}

void NetworkInterface::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "Socket number :" << socketDescriptor;
    SocketThread *socketInThread = new SocketThread(socketDescriptor);
    socketInThread->setDatabase(this->database);
    connect(socketInThread, SIGNAL(finished()), socketInThread, SLOT(deleteLater()));
    connect(socketInThread, SIGNAL(signalNewMessageInDatabase(QString,QString)), this, SLOT(slotNewMessageInDatabase(QString,QString)));
    connect(this, SIGNAL(signalNewMessageInDB(QString,QString)), socketInThread, SLOT(slotMessageFromNetwork(QString,QString)));
    socketInThread->start();
}

void NetworkInterface::slotNewMessageInDatabase(QString sender, QString receiver)
{
    qDebug() << "from thread sender : " << sender << " and receiver : " << receiver;
    emit this->signalNewMessageInDB(sender, receiver);
}

