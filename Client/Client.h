#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSharedPointer>

#include <QIODevice>
#include <QTcpSocket>
#include <QDataStream>

#include "Packaging.h"


class Client : public QObject {
    Q_OBJECT
friend class MainWindow;

public:
	explicit Client(QObject* parent = 0);
	Client(QString serverAddr = "127.0.0.1", QString portNr = "10013");

	void sendPackage(QString package);

	void setRequest(const QString& value) { request = value; }

private:
	QString serverAddr;
    QString portNr;
	QSharedPointer<QTcpSocket> tcpSocket;
	QSharedPointer<QDataStream> dataStream;

	Packaging packaging;
	QString request = "login_request";

signals:
    void readyRead();
    void error();
	void receivedPackage(QString package);

public slots:
    void onConnectToHost();
    void onReadMsg();
};

#endif // CLIENT_H
