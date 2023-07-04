#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include "chatpage.h"

namespace Ui {
class signUp;
}

class signUp : public QDialog
{
    Q_OBJECT

public:
    explicit signUp(QWidget *parent = nullptr);
    ~signUp();

private slots:

    void on_showPassword_clicked();
    void on_confirm_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_clicked();

private:
    Ui::signUp *ui;
    Chatpage * ChatPage;
};

#endif // SIGNUP_H
