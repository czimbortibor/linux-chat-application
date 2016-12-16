#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    Client* client;

signals:
    void connectClient();

private slots:
    void initClient();
    void onDisplayError(QAbstractSocket::SocketError socketError);
    void onReceivedMessage(QString message);
};


#endif // MAINWINDOW_H
