#ifndef SWITCHANGELS_H
#define SWITCHANGELS_H

#include <QDialog>
#include <QTimer>



namespace Ui {
class switchangels;
}

class switchangels : public QDialog
{
    Q_OBJECT

public:
    explicit switchangels(QWidget *parent = 0);
    ~switchangels();

private slots:
    void btnHeManual2_clicked();

    void btnFenManual2_clicked();

//    void btnStopManual2_clicked();

//    void btnDHeManual2_clicked();

//    void btnDFenManual2_clicked();

//    void btnDStopManual2_clicked();

    void m_timerSlot();

private:
    void init();
    void update_wdState();


private:
    Ui::switchangels *ui;

    QTimer* m_timer=new QTimer(this);

};

#endif // SWITCHANGELS_H
