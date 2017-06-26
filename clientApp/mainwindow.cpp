#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setupUi();
    this->linkUiToSystem();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->incorrectLabel->setVisible(false);
    ui->noDataLabel->setVisible(false);

    ui->passwordEditLogin->setEchoMode(QLineEdit::Password);
    ui->passwordEditLogin->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    ui->passwordEditRegister->setEchoMode(QLineEdit::Password);
    ui->passwordEditRegister->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
}

void MainWindow::linkUiToSystem()
{
    this->networkInterface.setMessageView(this->ui->chatView);
    //login form
    connect(this->ui->usernameEditLogin,SIGNAL(textChanged(QString)), &this->networkInterface, SLOT(slotSetUserName(QString)));
    connect(this->ui->passwordEditLogin,SIGNAL(textChanged(QString)), &this->networkInterface, SLOT(slotSetUserPassword(QString)));
    connect(this->ui->loginButton, SIGNAL(pressed()), &this->networkInterface, SLOT(slotLoginUser()));
    connect(&this->networkInterface, SIGNAL(signalNoData(bool)), this->ui->noDataLabel, SLOT(setVisible(bool)));
    connect(&this->networkInterface, SIGNAL(signalIncorrectData(bool)), this->ui->incorrectLabel, SLOT(setVisible(bool)));
    //register form
    connect(this->ui->usernameEditRegister,SIGNAL(textChanged(QString)), &this->networkInterface, SLOT(slotSetUserName(QString)));
    connect(this->ui->passwordEditRegister,SIGNAL(textChanged(QString)), &this->networkInterface, SLOT(slotSetUserPassword(QString)));
    connect(this->ui->registerButton, SIGNAL(pressed()), &this->networkInterface, SLOT(slotRegisterUser()));

    connect(this->ui->emailEditRegister,SIGNAL(textChanged(QString)), &this->networkInterface, SLOT(slotSetUserEmail(QString)));
    connect(&this->networkInterface, SIGNAL(signalSwitchPage(int)),this->ui->stackedWidget, SLOT(setCurrentIndex(int)));

    //send message

    connect(this->ui->chatEdit, SIGNAL(textChanged(QString)), &this->networkInterface, SLOT(slotSetMessage(QString)));
    connect(this->ui->sendButton, SIGNAL(pressed()), &this->networkInterface, SLOT(slotSendMessage()));

    //add friend
    connect(this->ui->addFriendButton, SIGNAL(pressed()), &this->networkInterface, SLOT(slotGoToFriendPage()));
    connect(this->ui->searchEdit, SIGNAL(textChanged(QString)), &this->networkInterface, SLOT(slotSetAddFriendName(QString)));
    connect(this->ui->addButton, SIGNAL(pressed()), &this->networkInterface, SLOT(slotAddFriend()));
    connect(&this->networkInterface, SIGNAL(signalUpdateFriendList()), this, SLOT(slotSetFriendList()));
    //set friend list
    this->slotSetFriendList();
    connect(this->ui->friendList, SIGNAL(currentIndexChanged(QString)), &this->networkInterface, SLOT(slotSetReceiverName(QString)));
}

void MainWindow::slotSetFriendList()
{
    this->ui->friendList->clear();
    this->ui->friendList->addItems(this->networkInterface.getFriends());
}

