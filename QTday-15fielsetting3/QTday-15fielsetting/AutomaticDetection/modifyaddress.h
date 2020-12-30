#ifndef MODIFYADDRESS_H
#define MODIFYADDRESS_H

#include <QDialog>

namespace Ui {
class ModifyAddress;
}

class ModifyAddress : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyAddress(QWidget *parent = 0);
    ~ModifyAddress();
    int addr;
    bool isOk;

private slots:
    void on_YesButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::ModifyAddress *ui;
};

#endif // MODIFYADDRESS_H
