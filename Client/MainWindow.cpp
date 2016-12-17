#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	LoginDialog* dialogPtr = new LoginDialog(this);
	loginDialog = QSharedPointer<LoginDialog>(dialogPtr);
	loginDialog->show();
	connect(loginDialog.data(), &LoginDialog::signIn, this, &MainWindow::onSignIn); // data(): extracts the raw pointer
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

	connect(this, &MainWindow::connectClient, client.data(), &Client::onConnectToHost);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
	// TODO: socket error handling
    //connect(client->tcpSocket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error), ui->lblError, &MainWindow::displayError);
	connect(client.data(), &Client::receivedMessage, this, &MainWindow::onReceivedMessage);

    emit connectClient();
	this->show();
}

void MainWindow::onDisplayError(QAbstractSocket::SocketError socketError) {
    //ui->lblError->setText(errorMsg);
}

void MainWindow::onReceivedMessage(QString message) {
    qDebug() << "receiving message...";
	message.insert(0, "Logged in on: ");
    ui->editRecvMessage->append(message);
}
