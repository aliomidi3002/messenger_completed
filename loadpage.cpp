#include "loadpage.h"
#include "ui_loadpage.h"

loadPage::loadPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loadPage)
{
    ui->setupUi(this);
}

loadPage::~loadPage()
{
    delete ui;
}
