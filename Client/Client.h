#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSharedPointer>

#include <QIODevice>
#include <QTcpSocket>
#include <QDataStream>


class Client : public QObject {
    Q_OBJECT
friend class MainWindow;

public:
	explicit Client(QObject* parent = 0);
	Client(QString serverAddr = "127.0.0.1", QString portNr = "10013");

	void sendMessage(QString message);

private:
	QString serverAddr;
    QString portNr;

	QSharedPointer<QTcpSocket> tcpSocket;
	QSharedPointer<QDataStream> dataStream;

signals:
    void readyRead();
    void error();
    void receivedMessage(QString message);

public slots:
    void onConnectToHost();
    void onReadMsg();
};

#endif // CLIENT_H
