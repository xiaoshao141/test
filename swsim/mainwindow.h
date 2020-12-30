#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QThread>

#include "swcomm.h"
#include "swdata.h"
#include "dialog.h"

extern swcomm* g_pSwComm[512];

extern swdata* g_pSwData;

namespace Ui {
class MainWindow;
}

class DevCommThread: public QThread
{
public:
    DevCommThread();
    void run();
    void init();

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateDZTable();
private:
    Ui::MainWindow *ui;

    DevCommThread* m_pDevCommThread;

private slots:
    void btnRefreshDZ();

    void btnAddDZ();

    void btnDelDZ();

    void btnRunCtrl();

private:
    bool m_RunCtrlFlag;

    Dialog* m_AddDzDialog;
};

#endif // MAINWINDOW_H
