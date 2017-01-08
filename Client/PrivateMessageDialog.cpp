#include "PrivateMessageDialog.h"
#include "ui_PrivateMessageDialog.h"

PrivateMessageDialog::PrivateMessageDialog(QWidget* parent, Client* client, QString chatPartner) : QDialog(parent),
	ui(new Ui::PrivateMessageDialog), client(client) {
	this->chatPartner = chatPartner;
	ui->setupUi(this);

	connect(client, &Client::receivedPrivateMessage, this, &PrivateMessageDialog::onReceivedPrivateMessage);
	connect(ui->btnSendMessage, &QPushButton::clicked, this, &PrivateMessageDialog::onSendMessageButtonClicked);
	connect(this, &PrivateMessageDialog::sendPrivateMessage, client, &Client::onSendPrivateMessage);
}

PrivateMessageDialog::~PrivateMessageDialog() {
	delete ui;
}

void PrivateMessageDialog::onReceivedPrivateMessage(QString receiver, QString message, QString sender) {
	QString output = receiver + ": " + message;
	ui->editRecvMessage->append(output);
}

void PrivateMessageDialog::onSendMessageButtonClicked() {
	QString message = ui->editSendMessage->toPlainText();
	QString receiver = chatPartner;
	QString sender = client->getUsername();

	emit sendPrivateMessage(receiver, message, sender);
	ui->editSendMessage->clear();
	QString output = sender + ": " + message;
	ui->editRecvMessage->append(output);
}
