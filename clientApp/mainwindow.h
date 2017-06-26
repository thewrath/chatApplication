#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkinterface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void slotSetFriendList();
private:
    Ui::MainWindow *ui;
    NetworkInterface networkInterface;

    void setupUi();
    void linkUiToSystem();
};

#endif // MAINWINDOW_H
