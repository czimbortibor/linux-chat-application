#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

#include "LoginDialog.h"
#include "Client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
	Ui::MainWindow* ui;
	QString username;
	QString password;
	QString serverAddr;
	QString port;

	QSharedPointer<LoginDialog> loginDialog;
	QSharedPointer<Client> client;

	void initClient();

signals:
    void connectClient();

private slots:
	void onSignIn(QString username, QString password, QString serverAddr, QString port);
    void onDisplayError(QAbstractSocket::SocketError socketError);
    void onReceivedMessage(QString message);
};


#endif // MAINWINDOW_H
