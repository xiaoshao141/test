#ifndef ADDEQUIPMENT_H
#define ADDEQUIPMENT_H

#include <QDialog>

namespace Ui {
class addequipment;
}

class addequipment : public QDialog
{
    Q_OBJECT

public:
    explicit addequipment(QWidget *parent = 0);
    ~addequipment();

    int addr;
    bool isOk;
    bool plisOk;

    void set_fouce();
private slots:

    void on_YesButton_clicked();

    void on_CancelButton_clicked();

    void on_AddButton_clicked();

private:
    Ui::addequipment *ui;
    int i;

};

#endif // ADDEQUIPMENT_H
