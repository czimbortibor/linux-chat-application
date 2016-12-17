#include "LoginDialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent), ui(new Ui::LoginDialog) {
    ui->setupUi(this);
	this->setFixedSize(this->size());
	connect(ui->btnOk, &QPushButton::clicked, this, &LoginDialog::on_btnOk_clicked);
	connect(ui->btnCancel, &QPushButton::clicked, this, &LoginDialog::on_btnCancel_clicked);
}

LoginDialog::~LoginDialog() {
    delete ui;
}

void LoginDialog::on_btnOk_clicked() {
	// TODO: check input
	username = ui->editUsername->text();
	password = ui->editPassword->text();
	serverAddr = ui->editServerAddr->text();
	port = ui->editPort->text();

	emit signIn(username, password, serverAddr, port);
	this->close();
}

void LoginDialog::on_btnCancel_clicked() {
	this->close();
}

QString LoginDialog::getUsername() {
	return username;
}

QString LoginDialog::getPassword() {
	return password;
}

QString LoginDialog::getServerAddr() {
	return serverAddr;
}

QString LoginDialog::getPort() {
	return port;
}
