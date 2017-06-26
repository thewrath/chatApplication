#include <QCoreApplication>
#include <QDebug>
#include "networkinterface.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NetworkInterface networkInterface;
    Database database;
    networkInterface.startServer(&database);
    return a.exec();
}
