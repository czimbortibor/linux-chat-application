#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	loginDialog = QSharedPointer<LoginDialog>::create(this);
	loginDialog->show();

	connect(loginDialog.data(), &LoginDialog::signIn, this, &MainWindow::onSignIn); // data(): extracts the raw pointer
	connect(ui->btnSendMessage, &QPushButton::clicked, this, &MainWindow::onSendMessage);
	// send private message via double click
	connect(ui->listUsers, &QListWidget::itemDoubleClicked, this, &MainWindow::onPrivateMessage);
	connect(client.data(), &Client::receivedPrivateMessage, this, &MainWindow::onReceivedPrivateMessage);
	// TODO: send private message with the button
	//connect(ui->btnSendPrivate, &QPushButton::clicked, this, &MainWindow::onPrivateMessage);
	// enable the "Send private message" button when an online user was selected
	connect(ui->listUsers, &QListWidget::itemPressed, this, &MainWindow::onUserSelected);
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
	QString username = this->username;
	client = QSharedPointer<Client>::create(serverAddr, port, username);

    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
	connect(client->tcpSocket.data(), static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error), this, &MainWindow::onDisplayError);

	connect(client.data(), &Client::receivedMessage, this, &MainWindow::onReceivedMessage);
	connect(client.data(), &Client::receivedUsersList, this, &MainWindow::onReceivedUsersList);
	connect(this->ui->btnLogout, &QPushButton::clicked, client.data(), &Client::onLogoutRequest);

	client->connectToServer();
	this->show();
}

void MainWindow::onDisplayError(QAbstractSocket::SocketError socketError) {
	QString title = tr("Connection error!");
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
		break;
	}
}

void MainWindow::onReceivedMessage(QString message) {
    ui->editRecvMessage->append(message);
}

void MainWindow::onReceivedUsersList(QString usersList) {
	// parse the string
	size_t pos = 0;
	std::string token;
	std::string inputStr = usersList.toStdString();
	while ((pos = inputStr.find(",")) != std::string::npos) {
		// next token
		token = inputStr.substr(0, pos);
		QString text = QString::fromStdString(token);
		// check if the list already contains the user and don't add the user itself to the list
		if ((ui->listUsers->findItems(text, Qt::MatchExactly).size()) == 0 && text != this->username) {
			ui->listUsers->addItem(text);
		}
		// erase the token + the glue
		inputStr.erase(0, pos + 1);
	}
}

void MainWindow::onSendMessage() {
	qDebug() << "sending message to the server...";
	QString message = ui->editSendMessage->toPlainText();
	client->onGlobalPackage(message);
	ui->editSendMessage->clear();
}

void MainWindow::onUserSelected() {
	ui->btnSendPrivate->setEnabled(true);
}

void MainWindow::onPrivateMessage(QListWidgetItem* item) {
	QString username = item->text();
	createPrivateDialog(username);
}

void MainWindow::onReceivedPrivateMessage(QString receiver, QString message, QString sender) {
	createPrivateDialog(sender);
}

void MainWindow::createPrivateDialog(QString username) {
	if (!privateDialogs.contains(username)) {
		PrivateMessageDialog* privateDialog = new PrivateMessageDialog(this, client.data(), username);
		// signal from the private message dialog when the user closes it so it can be removed from the list of opened dialogs
		connect(privateDialog, &PrivateMessageDialog::closeDialog, this, &MainWindow::onClosePrivateDialog);
		privateDialog->setWindowTitle("Chat with " + username);
		privateDialog->show();

		privateDialogs.push_back(username);
	}
}

void MainWindow::onClosePrivateDialog(QString username) {
	int i = 0;
	for (i = 0; i < privateDialogs.length(); ++i) {
		if (privateDialogs[i].compare(username) == 0) {
			break;
		}
	}
	privateDialogs.remove(i);
}
