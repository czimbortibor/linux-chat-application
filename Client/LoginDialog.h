#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT

public:
	explicit LoginDialog(QWidget* parent = 0);
    ~LoginDialog();

	QString getUsername() const { return username; }
	QString getPassword() const { return password; }
	QString getServerAddr() const { return serverAddr; }
	QString getPort() const { return port; }

private:
	Ui::LoginDialog* ui;
	QString username;
	QString password;
	QString serverAddr;
	QString port;

signals:
	void signIn(QString username, QString password, QString serverAddr, QString port);

private slots:
	void on_btnOk_clicked();
	void on_btnCancel_clicked();
};

#endif // LOGINDIALOG_H
