#include "newchat.h"
#include "ui_newchat.h"
QString sendmessageuser_chat_to_server(QString token,QString dst , QString body);
NewChat::NewChat(QWidget *parent, const QString& token)
    : QDialog(parent), ui(new Ui::NewChat), m_token(token)
{
    ui->setupUi(this);
}

NewChat::~NewChat()
{
    delete ui;
}

void NewChat::on_pushButton_clicked()
{
    hide();
}

void NewChat::on_pushButton_2_clicked()
{

    hide();
    return;
}

