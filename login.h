#ifndef LOGIN_H
#define LOGIN_H
#include "chatpage.h"
#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_b_clicked();
    void on_pushButton_clicked();

private:
    Ui::Login *ui;
    Chatpage * ChatPage;

};

#endif // LOGIN_H
