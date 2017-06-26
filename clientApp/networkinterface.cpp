#include "networkinterface.h"
#include <QDebug>

NetworkInterface::NetworkInterface()
{
    //connexion au serveur
    connect(&this->socket, SIGNAL(connected()), this, SLOT(slotConnexionReady()));
    connect(&this->socket, SIGNAL(readyRead()), this, SLOT(slotReadData()));
    this->socket.connectToHost(this->IP, this->port);
}

void NetworkInterface::slotSetUserName(QString name)
{
    this->userName = name;
}

void NetworkInterface::slotSetUserEmail(QString email)
{
    this->userEmail = email;
}

void NetworkInterface::slotSetUserPassword(QString password)
{
    this->userPassword = password;

}

void NetworkInterface::slotSetMessage(QString message)
{
    this->message = message;
}

void NetworkInterface::slotSetReceiverName(QString name)
{
    this->receiverName = name;
    qDebug() << name;
}

void NetworkInterface::slotSetAddFriendName(QString name)
{
    this->addFriendName = name;
}

void NetworkInterface::slotLoginUser()
{
    qDebug() << " Login user : " << this->userName;
    qDebug() << " with password : " << this->userPassword;
    //demande de login au serveur
    if(this->userName != NULL && this->userPassword != NULL)
        this->sendLoginRequestToServer();
    else
    {
        qDebug() << "data error";
        emit this->signalNoData(true);
    }
}

void NetworkInterface::slotRegisterUser()
{
    qDebug() << "Register user : " << this->userName;
    qDebug() << " with password : " << this->userPassword;
    qDebug() << " and email : " << this->userEmail;
    //demande de register au serveur
    if(this->userName != NULL && this->userPassword != NULL && this->userEmail != NULL)
        this->sendRegisterRequestToServer();
    else
    {
        qDebug() << "data error";
        emit this->signalNoData(true);
    }

}

void NetworkInterface::slotSendMessage()
{
    if(this->userName != NULL && this->userPassword != NULL && this->message != NULL && this->receiverName != NULL)
        this->sendMessageToServer();
    else
    {
        qDebug() << "message error";
    }
}

void NetworkInterface::slotAddFriend()
{
    if(this->addFriendName != NULL && this->addFriendName !=""){
        this->friends.append(this->addFriendName);
        this->addFriendName = "";
        emit this->signalUpdateFriendList();
        emit this->signalSwitchPage(1);
    }
}

void NetworkInterface::slotConnexionReady()
{
    qDebug() << "Connected to server ";
}

void NetworkInterface::slotReadData()
{
    this->sortData(this->socket.readAll());
}

void NetworkInterface::slotGoToFriendPage()
{
    emit this->signalSwitchPage(2);
}

void NetworkInterface::sendRequestToServer()
{

}

void NetworkInterface::sendLoginRequestToServer()
{
    //trame de login
    if(!this->connected){
        this->socket.write("&");
        this->socket.write("562");
        this->socket.write("&");
        this->socket.write("&");
        this->socket.write(this->userName.toUtf8());
        this->socket.write("&");
        this->socket.write("&");
        this->socket.write(this->userPassword.toUtf8());
        this->socket.write("&");
    }

}

void NetworkInterface::sendRegisterRequestToServer()
{
    //trame de register
    if(!this->connected){
        this->socket.write("&");
        this->socket.write("561");
        this->socket.write("&");
        this->socket.write("&");
        this->socket.write(this->userName.toUtf8());
        this->socket.write("&");
        this->socket.write("&");
        this->socket.write(this->userPassword.toUtf8());
        this->socket.write("&");
        this->socket.write("&");
        this->socket.write(this->userEmail.toUtf8());
        this->socket.write("&");
    }
}

void NetworkInterface::sendMessageToServer()
{
    //trame de message
    if(this->connected){
        this->socket.write("&");
        this->socket.write("563");
        this->socket.write("&");
        this->socket.write("&");
        this->socket.write(this->userName.toUtf8());
        this->socket.write("&");
        this->socket.write("&");
        this->socket.write(this->receiverName.toUtf8());
        this->socket.write("&");
        this->socket.write("&");
        this->socket.write(this->message.toUtf8());
        this->socket.write("&");

        this->insertMessageOnView(this->userName, this->receiverName, this->message);
    }
}

void NetworkInterface::sortData(QByteArray data)
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


    //boucle for d'analyse de la trame
    //562 reponse ok pour le login
    if(sortedData.at(0) == "562" || sortedData.at(0) == "561"){
        this->connected = true;
        this->user.setName(this->userName);
        this->user.setPassword(this->userPassword);
        this->user.setEmail(this->userEmail);
        emit this->signalSwitchPage(1);
    }
    //265 reponse false du login
    else if(sortedData.at(0) == "265" || sortedData.at(0) == "165"){
        emit this->signalIncorrectData(true);
    }

    if(sortedData.at(0) == "564"){
        //un message
        this->insertMessageOnView(QString::fromStdString(sortedData.at(1)), QString::fromStdString(sortedData.at(2)), QString::fromStdString(sortedData.at(3)));
    }


}

void NetworkInterface::insertMessageOnView(QString senderName, QString receiverName, QString content)
{
    this->messageView->append("<"+senderName+"> " + content);
}
