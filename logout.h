#ifndef LOGOUT_H
#define LOGOUT_H

#include <QDialog>

namespace Ui {
class LogOut;
}

class LogOut : public QDialog
{
    Q_OBJECT

public:
    explicit LogOut(QWidget *parent = nullptr);
    ~LogOut();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::LogOut *ui;
};

#endif // LOGOUT_H
