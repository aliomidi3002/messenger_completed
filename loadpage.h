#ifndef LOADPAGE_H
#define LOADPAGE_H

#include <QDialog>

namespace Ui {
class loadPage;
}

class loadPage : public QDialog
{
    Q_OBJECT

public:
    explicit loadPage(QWidget *parent = nullptr);
    ~loadPage();

private:
    Ui::loadPage *ui;
};

#endif // LOADPAGE_H
