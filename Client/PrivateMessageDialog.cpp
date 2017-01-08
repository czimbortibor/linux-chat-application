#include "PrivateMessageDialog.hpp"
#include "ui_PrivateMessageDialog.h"

PrivateMessageDialog::PrivateMessageDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PrivateMessageDialog)
{
	ui->setupUi(this);
}

PrivateMessageDialog::~PrivateMessageDialog()
{
	delete ui;
}
