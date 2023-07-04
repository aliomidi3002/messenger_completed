#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QDialog>
#include "ID.h"
#include <QTextEdit>
#include "QVBoxLayout"
#include <QKeyEvent>
#include <QListWidget>

namespace Ui {
class Chatpage;
}

class Chatpage : public QDialog
{
    Q_OBJECT

public:
    explicit Chatpage(QWidget *parent = nullptr, const userID& currentUser = userID());
    ~Chatpage();

    void showUsers();


private slots:
    void on_pushButton_2_clicked();
    void on_toolButton_5_clicked();
    void on_toolButton_3_clicked();
    void on_toolButton_4_clicked();
    void show_users_chat(QString user);
    void on_pushButton_5_clicked();
    void on_pushButton_clicked();
    void on_listWidget_2_itemClicked(QListWidgetItem *item);
    void on_listWidget_4_itemClicked(QListWidgetItem *item);
    void show_groups_chats(QString name);
    void show_channel_chats(QString name);
    void on_pushButton_7_clicked();
    void on_listWidget_3_itemClicked(QListWidgetItem *item);
    void on_pushButton_6_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::Chatpage *ui;
    userID mCurrentUser;
    QVBoxLayout* layout;
};

#endif // CHATPAGE_H
