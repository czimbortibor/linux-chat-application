#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	LoginDialog* dialogPtr = new LoginDialog(this);
	loginDialog = QSharedPointer<LoginDialog>(dialogPtr);
	loginDialog->show();

	connect(loginDialog.data(), &LoginDialog::signIn, this, &MainWindow::onSignIn); // data(): extracts the raw pointer
	connect(ui->btnSendMessage, &QPushButton::clicked, this, &MainWindow::onSendMessage);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSignIn(QString username, QString password, QString serverAddr, QString port) {
	this->username = username;
	this->password = password;
	this->serverAddr = serverAddr;
	this->port = port;

	initClient();
}

void MainWindow::initClient() {
	Client* clientPtr = new Client(serverAddr, port);
	client = QSharedPointer<Client>(clientPtr);

	connect(this, &MainWindow::connectClient, client.data(), &Client::onLoginRequest);

	// TODO: error handling
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
	//connect(client->tcpSocket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error), this, &MainWindow::onDisplayError);

	connect(client.data(), &Client::receivedPackage, this, &MainWindow::onReceivedMessage);
	connect(this->ui->btnLogout, &QPushButton::clicked, client.data(), &Client::onLogoutRequest);

	QString request = "login_request";
	client->setRequest(request);
    emit connectClient();
	this->show();
}

void MainWindow::onDisplayError(QAbstractSocket::SocketError socketError) {
	QString title = "Connection error!";
	switch (socketError) {
		case QAbstractSocket::HostNotFoundError:
			QMessageBox::information(this, title,
									 tr("The host was not found. Please check the "
										"host name and port settings."));
			break;
		case QAbstractSocket::ConnectionRefusedError:
			QMessageBox::information(this, title,
									 tr("The connection was refused by the host. "));
			break;
		default:
			QMessageBox::information(this, title, tr("Unknown error occured."));
		}
}

void MainWindow::onReceivedMessage(QString message) {
    ui->editRecvMessage->append(message);
	qDebug() << "received message";
}

void MainWindow::onSendMessage() {
	qDebug() << "sending message to the server...";
	QString message = ui->editSendMessage->toPlainText();
	client->sendPackage(message);
}
