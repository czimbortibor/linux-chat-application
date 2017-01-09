#ifndef PRIVATEMESSAGEDIALOG_HPP
#define PRIVATEMESSAGEDIALOG_HPP

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>

#include "Client.h"

namespace Ui {
class PrivateMessageDialog;
}

class PrivateMessageDialog : public QDialog {
	Q_OBJECT

public:
	explicit PrivateMessageDialog(QWidget* parent = 0, Client* client = 0, QString chatPartner = "");
	~PrivateMessageDialog();

	QString getUsername() { return client->getUsername(); }

private:
	Ui::PrivateMessageDialog* ui;
	QSharedPointer<Client> client;
	/** the user's name whom the private chat is ongoing */
	QString chatPartner;

signals:
	void closeDialog(QString username);
	void sendPrivateMessage(QString receiver, QString message, QString sender);
	void sendFile(QByteArray blob, int fileSize, QString receiver, QString sender);

public slots:
	void onReceivedPrivateMessage(QString receiver, QString message, QString sender);
	void onSendMessageButtonClicked();
	void onSendFile();
};

#endif // PRIVATEMESSAGEDIALOG_HPP
