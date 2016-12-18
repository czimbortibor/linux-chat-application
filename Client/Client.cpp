#include "Client.h"

Client::Client(QObject* parent) : QObject(parent) {}

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

	std::string package = packaging.createLoginPackage("test_user");
}

void Client::onReadMsg() {
    qDebug() << "starting the transaction...";

    /** read the message in parts */
    /*dataStream->startTransaction();
    QString dataBlock;
    *dataStream >> dataBlock;

    qDebug() << "reading the message...";
	// tries to close the transaction
	if (!dataStream->commitTransaction()) {
        return;
    }
    qDebug() << "closing transaction...";*/

	/** reads the whole message in one go. MAY NOT BE GOOD ENOUGH */
    tcpSocket->waitForBytesWritten();
    QByteArray res = tcpSocket->readAll();

	QString package = QString::fromLatin1(res);
	packaging.parsePackage(package.toStdString());
	QString sender = QString::fromStdString(packaging.getSender());
	QString message = QString::fromStdString(packaging.getMessage());
	QString output = sender + message;

	emit receivedPackage(output);
}

void Client::sendPackage(QString package) {
	/** encode the message into a byte array */
<<<<<<< HEAD
	QByteArray messageBlock = package.toUtf8();
=======
	QByteArray messagBlock = message.toUtf8();
>>>>>>> 1a123a584b2b361786849f2a6f4730d715edc42b

	tcpSocket->write(messagBlock);
	qDebug() << "message sent to the server.";
}
