#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include "chatpage.h"
#include "ID.h"
#include <QDebug>

QString get_Desktop_Path();

bool is_it_loged_in( QString& username, QString& password, QString& token)
{
    QString path = get_Desktop_Path();
    path.append("user_info/login_info.txt");
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false; // Failed to open the file
    }

    QTextStream in(&file);
    QString line;
    while (!in.atEnd()) {
        line = in.readLine();
        if (line.startsWith("Username: ")) {
            username = line.mid(10); // Extract the username value
        } else if (line.startsWith("Password: ")) {
            password = line.mid(10); // Extract the password value
        } else if (line.startsWith("Token: ")) {
            token = line.mid(7); // Extract the token value
        }
    }

    file.close();
    return true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QString username;
    QString password;
    QString token;

    bool logedIn = is_it_loged_in(username , password , token);

    qDebug() << "this is token:" << token;

    userID currentUser = userID(username , password);
    currentUser.setToken(token);

    if(logedIn){
        Chatpage *chat = new Chatpage(this, currentUser);
        chat->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Window | Qt::FramelessWindowHint);
        chat->setAttribute(Qt::WA_DeleteOnClose); // Optional: Delete the window when closed
        chat->show();
    }
    else{
        ui->setupUi(this);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// open login page
void MainWindow::on_pushButton_clicked()
{
    QString username;
    QString password;
    QString token;

    bool logedIn = is_it_loged_in(username , password , token);
    if(logedIn){
        return;
    }
    hide();
    login = new Login(this);
    login->show();
}

// open signUp page
void MainWindow::on_pushButton_2_clicked()
{
    QString username;
    QString password;
    QString token;

    bool logedIn = is_it_loged_in(username , password , token);
    if(logedIn){
        return;
    }
    hide();
    SignUp = new signUp(this);
    SignUp->show();

}
