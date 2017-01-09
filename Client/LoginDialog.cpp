#include "LoginDialog.h"
#include "ui_LoginDialog.h"

#include <QDebug>

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
	this->setFixedSize(this->size());
	connect(ui->btnOk, &QPushButton::clicked, this, &LoginDialog::onOKClicked);
	connect(ui->btnCancel, &QPushButton::clicked, this, &LoginDialog::onCancelClicked);
}

LoginDialog::~LoginDialog() {
    delete ui;
}

void LoginDialog::onOKClicked() {
	// TODO: check input
	username = ui->editUsername->text();
	password = ui->editPassword->text();
	serverAddr = ui->editServerAddr->text();
	port = ui->editPort->text();

	emit signIn(username, password, serverAddr, port);
	this->close();
}

void LoginDialog::onCancelClicked() {
	this->close();
}
