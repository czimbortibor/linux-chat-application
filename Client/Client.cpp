#include "Client.h"

Client::Client(QObject* parent) : QObject(parent) {
}

Client::Client(QString serverAddr, QString portNr) {
	QTcpSocket* tcpSocketPtr = new QTcpSocket();
	tcpSocket = QSharedPointer<QTcpSocket>(tcpSocketPtr);
	QDataStream* dataStreamPtr = new QDataStream();
	dataStream = QSharedPointer<QDataStream>(dataStreamPtr);

	dataStream->setDevice(tcpSocket.data());

	this->serverAddr = serverAddr;
    this->portNr = portNr;

	connect(tcpSocket.data(), &QIODevice::readyRead, this, &Client::onReadMsg);
}

void Client::onConnectToHost() {
    tcpSocket->abort();
	qDebug() << "connecting to " << serverAddr << ":" << portNr;

    /** connect to the main server */
	tcpSocket->connectToHost(serverAddr, portNr.toInt());
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

	/** reads the whole message. MAY NOT BE GOOD ENOUGH */
    tcpSocket->waitForBytesWritten();
    QByteArray res = tcpSocket->readAll();
    QString message = QString::fromLatin1(res);

    emit receivedMessage(message);
}

void Client::sendMessage(QString message) {
	/** encode the message into a byte array */
	QByteArray messageBlock;
	QDataStream writeOut(&messageBlock, QIODevice::WriteOnly);
	writeOut << message;

	tcpSocket->write(messageBlock);
	qDebug() << "message sent to the server.";
}
