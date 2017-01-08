#ifndef PRIVATEMESSAGEDIALOG_HPP
#define PRIVATEMESSAGEDIALOG_HPP

#include <QDialog>

namespace Ui {
class PrivateMessageDialog;
}

class PrivateMessageDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PrivateMessageDialog(QWidget *parent = 0);
	~PrivateMessageDialog();

private:
	Ui::PrivateMessageDialog *ui;
};

#endif // PRIVATEMESSAGEDIALOG_HPP
