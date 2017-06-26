#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QTcpSocket>
#include <QTextBrowser>

#include "user.h"

class NetworkInterface : public QObject
{
    Q_OBJECT
public:
    NetworkInterface();
    QStringList getFriends(){ return this->friends; }
    void setMessageView(QTextBrowser *messageView){ this->messageView = messageView; }
public slots:
    //ui
    void slotSetUserName(QString name);
    void slotSetUserEmail(QString email);
    void slotSetUserPassword(QString password);
    void slotSetMessage(QString message);
    void slotSetReceiverName(QString name);
    void slotSetAddFriendName(QString name);

    void slotLoginUser();
    void slotRegisterUser();
    void slotSendMessage();
    void slotAddFriend();

    //server
    void slotConnexionReady();
    void slotReadData();

    void slotGoToFriendPage();
signals:
    void signalIncorrectData(bool visible);
    void signalNoData(bool visible);
    void signalSwitchPage(int index);
    void signalUpdateFriendList();
private:
    bool connected = false;
    User user;
    //ui
    QString userName = NULL;
    QString userEmail = NULL;
    QString userPassword = NULL;
    QString message = NULL;
    QString receiverName = NULL;
    QString addFriendName = NULL;

    QTextBrowser *messageView;


    //server
    const QString IP = "localhost";
    const int port = 4000;
    QTcpSocket socket;
    QStringList friends;
    void sendRequestToServer();
    void sendLoginRequestToServer();
    void sendRegisterRequestToServer();
    void sendMessageToServer();

    void sortData(QByteArray data);

    void insertMessageOnView(QString senderName, QString receiverName, QString content);
};

#endif // NETWORKINTERFACE_H
