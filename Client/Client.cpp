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
	QString receiver = QString::fromStdString(packaging.getReceiver());
	std::string request = packaging.identifyRequest(package.toStdString());
	qDebug() << "request:" << QString::fromStdString(request);
	/** string to be displayed on the message wall */
	QString output;

	qDebug() << "from server: " << res;
	if (request.compare("time_package") == 0) {
		output = "Login time: " + message;
		emit receivedMessage(output);
	}
	else if (request.compare("online_users_package") == 0) {
		emit receivedUsersList(message);
	}
	else if (request.compare("global_package") == 0) {
		output = sender + ": " + message;
		emit receivedMessage(output);
	}
	else if (request.compare("private_package") == 0) {
		// qDebug() << sender + " -> " + receiver + ": " + message;
		emit receivedPrivateMessage(sender, message, receiver);
	}
}

void Client::sendPackage(QString package) {
	qDebug() << "package: " << package;

	/** encode the message into a byte array */
	QByteArray messageBlock = package.toUtf8();;

	tcpSocket->write(messageBlock);
	qDebug() << "package sent to the server.\n";
}

void Client::onSendPrivateMessage(QString receiver, QString message, QString sender) {
	std::string package = packaging.createPivatePackage(receiver.toStdString(), message.toStdString(), sender.toStdString());
	sendPackage(QString::fromStdString(package));
}

void Client::onSendFile(QByteArray blob, int fileSize, QString receiver, QString sender) {
	// send over the file_size package
	std::string package = packaging.createFileSizePackage(fileSize);
	sendPackage(QString::fromStdString(package));

	tcpSocket->waitForBytesWritten();
	tcpSocket->flush();
	package = blob.toStdString();
	sendPackage(QString::fromStdString(package));
}
