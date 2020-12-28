#ifndef SWITCHPORT_H
#define SWITCHPORT_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class switchPort;
}

class switchPort : public QDialog
{
    Q_OBJECT

public:
    explicit switchPort(QWidget *parent = 0);
    ~switchPort();

private slots:
    void btnHeManual1_clicked();

    void btnFenManual1_clicked();

//    void btnStopManual1_clicked();

//    void btnDHeManual1_clicked();

//    void btnDFenManual1_clicked();

//    void btnDStopManual1_clicked();

    void m_timerSlot();

private:
    void init();
    void update_ztState();

private:
    Ui::switchPort *ui;

    QTimer* m_timer=new QTimer(this);
};

#endif // SWITCHPORT_H
