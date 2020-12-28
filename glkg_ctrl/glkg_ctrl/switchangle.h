#ifndef SWITCHANGLE_H
#define SWITCHANGLE_H

#include <QDialog>

namespace Ui {
class switchAngle;
}

class switchAngle : public QDialog
{
    Q_OBJECT

public:
    explicit switchAngle(QWidget *parent = 0);
    ~switchAngle();

private slots:
    void on_btHeManual_clicked();

    void on_btFenManual_clicked();

    void on_btStopManual_clicked();



private:
    Ui::switchAngle *ui;
};

#endif // SWITCHANGLE_H
