#include "socketthread.h"
#include "utils.h"

#include <algorithm>

SocketThread::SocketThread(int ID, QObject *parent) : QThread(parent) , socketDescriptor(ID)
{

}

void SocketThread::setDatabase(Database *database)
{
    this->database = database;
}

void SocketThread::run()
{
    this->tcpSocket = new QTcpSocket();
    if (!tcpSocket->setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket->error());
        return;
    }
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()), Qt::QueuedConnection);
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()), Qt::QueuedConnection);
    qDebug() << socketDescriptor << "Client connected ";

    exec();
}

void SocketThread::slotReadyRead()
{
    this->sortData(this->tcpSocket->readAll());
}

void SocketThread::slotDisconnected()
{
    qDebug() << socketDescriptor << "Disconnected";
    tcpSocket->deleteLater();
    exit(0);
}

void SocketThread::sortData(QByteArray data)
{
    std::string trame = data.toStdString();
    std::string temp;
    std::vector<std::string> sortedData;

    qDebug() << QString::fromStdString(trame);

    bool cut = true;
    for(char& c : trame) {
        if(c == '&'){
            cut = !cut;
        }
        else{
            temp += c;
        }
        if(cut == true){
            sortedData.push_back(temp);
            temp = "";
        }
    }

    qDebug() << "dataSorted";

    for(std::string v : sortedData){
        qDebug() << QString::fromStdString(v);
    }

    if(sortedData.at(0) == "562"){
        this->response(this->database->loginUser(QString::fromStdString(sortedData.at(1)), QString::fromStdString(sortedData.at(2))), sortedData.at(0));
        this->username = QString::fromStdString(sortedData.at(1));
    }
    else if(sortedData.at(0) == "561"){
        this->response(this->database->registerUser(QString::fromStdString(sortedData.at(1)), QString::fromStdString(sortedData.at(2)), QString::fromStdString(sortedData.at(3))), sortedData.at(0));
    }
    else if(sortedData.at(0) == "563"){
        //message
        this->response(this->database->insertMessage(QString::fromStdString(sortedData.at(1)), QString::fromStdString(sortedData.at(2)), QString::fromStdString(sortedData.at(3))), sortedData.at(0));
        //envoyer le message au network pour qu'il l'envoie au bon socket ou faire un thread qui va toujours request dans la DB
    }
}

void SocketThread::response(bool isSuccessful, std::string response)
{
    qDebug() << isSuccessful;
    if(!isSuccessful){
        std::reverse(response.begin(),response.end());
    }
    this->tcpSocket->write(QByteArray::fromStdString(response));
}





