#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QVector>

#include "LoginDialog.h"
#include "Client.h"
#include "PrivateMessageDialog.h"


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

	/** list of all the active private message dialogs */
	QVector<QString> privateDialogs;

	void initClient();
	void createPrivateDialog(QString username);

private slots:
	void onSignIn(QString username, QString password, QString serverAddr, QString port);
    void onDisplayError(QAbstractSocket::SocketError socketError);
    void onReceivedMessage(QString message);
	void onSendMessage();
	void onReceivedUsersList(QString usersList);
	void onPrivateMessage(QListWidgetItem* item);
	void onUserSelected();
	void onClosePrivateDialog(QString username);
	void onReceivedPrivateMessage(QString receiver, QString message, QString sender);
};


#endif // MAINWINDOW_H
