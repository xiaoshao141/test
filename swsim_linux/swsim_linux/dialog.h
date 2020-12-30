#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void genCRC7(quint8* pSrc, uint len);

private:
    Ui::Dialog *ui;

public slots:
    void btnAddDZInfo();
};

#endif // DIALOG_H
