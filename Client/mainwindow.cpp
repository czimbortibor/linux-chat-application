#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->btnConnect, &QPushButton::clicked, this, &MainWindow::initClient);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initClient() {
    QString serverAddr = ui->editServerAddr->text();
    QString portNr = ui->editPort->text();
    client = new Client(serverAddr, portNr);

    connect(this, &MainWindow::connectClient, client, &Client::onConnectToHost);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    //TODO
    //connect(client->tcpSocket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error), ui->lblError, &MainWindow::displayError);
    connect(client, &Client::receivedMessage, this, &MainWindow::onReceivedMessage);

    emit connectClient();
}

void MainWindow::onDisplayError(QAbstractSocket::SocketError socketError) {
    //ui->lblError->setText(errorMsg);
}

void MainWindow::onReceivedMessage(QString message) {
    qDebug() << "receiving message...";
    ui->editRecvMessage->append(message);
}
