#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QIODevice>
#include <QTcpSocket>
#include <QDataStream>


class Client : public QObject {
    Q_OBJECT
friend class MainWindow;

public:
    explicit Client(QObject *parent = 0);
    Client(QString hostAddr, QString portNr);

private:
    QString hostAddr;
    QString portNr;

    QTcpSocket* tcpSocket;
    QDataStream* dataStream;

signals:
    void readyRead();
    void error();
    void receivedMessage(QString message);

public slots:
    void onConnectToHost();
    void onReadMsg();
};

#endif // CLIENT_H
