#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// open login page
void MainWindow::on_pushButton_clicked()
{
    hide();
    login = new Login(this);
    login->show();
}

// open signUp page
void MainWindow::on_pushButton_2_clicked()
{
    hide();
    SignUp = new signUp(this);
    SignUp->show();

}

