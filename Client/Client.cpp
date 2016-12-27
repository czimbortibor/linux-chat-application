#include "Client.h"


Client::Client(QObject* parent) : QObject(parent) {}

Client::Client(QString serverAddr, QString portNr, QString username) {
	tcpSocket = QSharedPointer<QTcpSocket>::create();
	//QDataStream* dataStreamPtr = new QDataStream();
	//dataStream = QSharedPointer<QDataStream>(dataStreamPtr);
	//dataStream->setDevice(tcpSocket.data());

	this->serverAddr = serverAddr;
    this->portNr = portNr;
	this->username = username;

	connect(tcpSocket.data(), &QIODevice::readyRead, this, &Client::onReadMsg);
	connect(this, &Client::login, this, &Client::onLoginRequest);
}

void Client::connectToServer() {
	qDebug() << "connecting to " << serverAddr << ":" << portNr;

	/** connect to the main server */
	tcpSocket->connectToHost(serverAddr, portNr.toInt());
	if (tcpSocket->waitForConnected(10000)) {
		qDebug() << "connected!";
	} else {
		qDebug() << "could not connect!";
	}
	emit login();
}

void Client::onLoginRequest() {
	std::string package = packaging.createLoginPackage(username.toStdString());
	sendPackage(QString::fromStdString(package));
}

void Client::onLogoutRequest() {
	std::string package = packaging.createDisconnectPackage();
	sendPackage(QString::fromStdString(package));
}

void Client::onGlobalPackage(QString message) {
	std::string package = packaging.createGlobalPackage(message.toStdString(), username.toStdString());
	sendPackage(QString::fromStdString(package));
}

void Client::onReadMsg() {
    qDebug() << "starting the transaction...";

	// read the message in parts
    /*dataStream->startTransaction();
    QString dataBlock;
    *dataStream >> dataBlock;

    qDebug() << "reading the message...";
	// tries to close the transaction
	if (!dataStream->commitTransaction()) {
        return;
    }
    qDebug() << "closing transaction...";*/

	// reads the whole message in one go. MAY NOT WORK in all cases
    tcpSocket->waitForBytesWritten();
    QByteArray res = tcpSocket->readAll();
	QString package = QString::fromLatin1(res);
	packaging.parsePackage(package.toStdString());
	QString sender = QString::fromStdString(packaging.getSender());
	QString message = QString::fromStdString(packaging.getMessage());
	/** string to be displayed on the message wall */
	QString output;
	// on login the server sends back the current time
	if (sender == " ") {
		output = "Login time: " + message;
	}
	// normal message from a user
	else {
		output = sender + ": "+ message;
	}

	emit receivedPackage(output);
}

void Client::sendPackage(QString package) {
	qDebug() << "package: " << package;

	/** encode the message into a byte array */
	QByteArray messageBlock = package.toUtf8();;

	tcpSocket->write(messageBlock);
	qDebug() << "message sent to the server.";
}
