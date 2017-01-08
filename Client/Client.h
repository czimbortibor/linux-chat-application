#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSharedPointer>

#include <QIODevice>
#include <QTcpSocket>
#include <QDataStream>

#include "../util/Packaging.h"


class Client : public QObject {
    Q_OBJECT
friend class MainWindow;

public:
	explicit Client(QObject* parent = 0);
	Client(QString serverAddr = "127.0.0.1", QString portNr = "10013", QString username = "test user");

	void connectToServer();
	void sendPackage(QString package);

private:
	QString serverAddr;
    QString portNr;
	QString username;
	QSharedPointer<QTcpSocket> tcpSocket;
	QSharedPointer<QDataStream> dataStream;

	Packaging packaging;

signals:
    void readyRead();
    void error();
	void login();
	void receivedPackage(QString package);

public slots:
	void onLoginRequest();
	void onReadMsg();
	void onLogoutRequest();
	void onGlobalPackage(QString message);
};

#endif // CLIENT_H
