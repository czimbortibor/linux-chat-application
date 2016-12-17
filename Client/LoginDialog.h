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

	QString getUsername();
	QString getPassword();
	QString getServerAddr();
	QString getPort();

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
