#include "logout.h"
#include "ui_logout.h"
#include <QMessageBox>
#include <QUrlQuery>
#include <QtNetwork>
#include "mainwindow.h"
QString logout(QString user,QString pass);
QString response_code(QString Server_Response);
QString logout(QString user,QString pass) {
    QString url1= "http://api.barafardayebehtar.ml:8080/logout?username=";
    QString url2= "&password=";

    url1=url1+user+url2+pass;
    QUrl url(url1);
    /*
    QUrl url2("&password=");
    url = url.resolved(user1);
    url = url.resolved(url2);
    url = url.resolved(pass1);*/
    // Create a QNetworkAccessManager object
    QNetworkAccessManager manager;

    // Create a QNetworkRequest object and set the URL
    QNetworkRequest request;
    request.setUrl(url);

    // Send the GET request
    QNetworkReply* reply = manager.get(request);

    // Wait for the request to finish
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString response;
    if (reply->error() == QNetworkReply::NoError) {
        // Reading the response data
        QByteArray responseData = reply->readAll();
        response = QString(responseData);
    } else {
        // Handle error cases
        qDebug() << "Error:" << reply->errorString();
    }

    // Clean up
    reply->deleteLater();
    QString response_code_login_server = response_code(response);
    return response_code_login_server;

}

LogOut::LogOut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogOut)
{
    ui->setupUi(this);
}

LogOut::~LogOut()
{
    delete ui;
}

void LogOut::on_pushButton_3_clicked()
{
    if (ui->lineEdit_2->echoMode() == QLineEdit::Password)
    {
        ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
        ui->pushButton_3->setStyleSheet("background-color: rgb(0, 0, 127);");
    }
    else
    {
        ui->lineEdit_2->setEchoMode(QLineEdit::Password);
        ui->pushButton_3->setStyleSheet("background-color: rgb(0, 85, 127);");
    }
}


void LogOut::on_pushButton_4_clicked()
{
    if (ui->lineEdit_3->echoMode() == QLineEdit::Password)
    {
        ui->lineEdit_3->setEchoMode(QLineEdit::Normal);
        ui->pushButton_4->setStyleSheet("background-color: rgb(0, 0, 127);");
    }
    else
    {
        ui->lineEdit_3->setEchoMode(QLineEdit::Password);
        ui->pushButton_4->setStyleSheet("background-color: rgb(0, 85, 127);");
    }
}



void LogOut::on_pushButton_2_clicked()
{
    QMessageBox::StandardButton replay = QMessageBox::question(this , "Log out " , "Are you sure you want to exit?" , QMessageBox::Yes | QMessageBox::No);

    if (replay == QMessageBox::Yes){
        hide();
        MainWindow * main;
        main = new MainWindow(this);
        main->show();
    }
    else
        return;

}

