#include "client.h"

Client::Client(QObject *parent) : QObject(parent) {
}

Client::Client(QString hostAddr, QString portNr) {
    tcpSocket = new QTcpSocket();
    dataStream = new QDataStream();

    dataStream->setDevice(tcpSocket);

    this->hostAddr = hostAddr;
    this->portNr = portNr;

    connect(tcpSocket, &QIODevice::readyRead, this, &Client::onReadMsg);
}

void Client::onConnectToHost() {
    tcpSocket->abort();
    qDebug() << "connecting to " << hostAddr << ":" << portNr;

    /** connect to the main server */
    tcpSocket->connectToHost(hostAddr, portNr.toInt());
}

void Client::onReadMsg() {
    qDebug() << "starting the transaction...";

    /** read the message in parts */
    /*dataStream->startTransaction();
    QString dataBlock;
    *dataStream >> dataBlock;

    qDebug() << "reading the message...";
    /**tries to close the transaction */
    /*if (!dataStream->commitTransaction()) {
        return;
    }
    qDebug() << "closing transaction...";*/

    /** reads the whole message. NOT GOOD ENOUGH */
    tcpSocket->waitForBytesWritten();
    QByteArray res = tcpSocket->readAll();
    QString message = QString::fromLatin1(res);

    emit receivedMessage(message);
}
