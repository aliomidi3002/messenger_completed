#include "login.h"
#include "ui_login.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "ID.h"
#include <QtNetwork>
#include "chatpage.h"


QString response_code(QString Server_Response);

QString login_to_server(QString user,QString pass) {
    QString url1= "http://api.barafardayebehtar.ml:8080/login?username=";
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
    QString searchString = "\"token\": \"";
    QString targetString = "\"}";

    // Find the positions of the search and target substrings
    size_t searchPos = response.toStdString().find(searchString.toStdString());
    size_t targetPos = response.toStdString().find(targetString.toStdString());

    // Extract the desired portion of the original string
    if(response_code_login_server=="200" && searchPos != std::string::npos){

        std::string token = response.toStdString().substr(searchPos + searchString.length(), targetPos - (searchPos + searchString.length()));
        //response = QString::fromStdString(token);
        QString the_token_that_we_return= QString::fromStdString(token);
        return the_token_that_we_return ;
    }
    else if(response_code_login_server=="200"){
        return "300";
    }

    else {
        return response_code_login_server;
    }
}
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    connect(ui->showPassword, SIGNAL(clicked()), this, SLOT(on_pushButton_2_clicked()));
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_2_clicked()
{
    if (ui->lineEdit_p->echoMode() == QLineEdit::Password)
    {
        ui->lineEdit_p->setEchoMode(QLineEdit::Normal);
        ui->showPassword->setStyleSheet("background-color: rgb(0, 0, 127);");
    }
    else
    {
        ui->lineEdit_p->setEchoMode(QLineEdit::Password);
        ui->showPassword->setStyleSheet("background-color: rgb(0, 85, 127);");
    }
}


void Login::on_pushButton_b_clicked()
{
    close(); // Close the Login dialog
    QWidget* mainWindow = parentWidget();
    if (mainWindow) {
        mainWindow->show(); // Show the main window
    }
}

void Login::on_pushButton_clicked()
{
    QString Username = ui->lineEdit_2->text();
    QString Password = ui->lineEdit_p->text();

    if(Username.isEmpty()){
        ui->label_4->setText("Enter Your Username");
        return;
    }

    else if(Password.isEmpty()){
        ui->label_4->setText("Enter Your Password");
        return;
    }

    QString respons_login = login_to_server(Username , Password);
    userID currentUser = userID(Username , Password);




    if(respons_login == "404"){
        ui->label_4->setText("Connection lost! :(");
        return;
    }

    else if(respons_login == "401"){
        ui->label_4->setText("Informations Is Incorrect");
        return;
    }


    else if(respons_login == "300"){
        ui->label_4->setText("You already loged in");
        return;
    }

    else if(respons_login == "204"){
        ui->label_4->setText("204?!!!");
        return;
    }
    else if(respons_login != "200"){
        currentUser.setToken(respons_login);
        hide();
        ChatPage = new Chatpage(this, currentUser);
        ChatPage->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window | Qt::FramelessWindowHint);
        ChatPage->setAttribute(Qt::WA_DeleteOnClose); // Optional: Delete the window when closed
        ChatPage->show();
    }

}

//hide();
//Chatpage* chatPage = new Chatpage(this, currentUser);
//chatPage->show();
