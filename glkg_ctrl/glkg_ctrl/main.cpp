#include "mainwindow.h"
#include <QApplication>

bool g_wdAState;
bool g_wdBState;
bool g_wdCState;
bool g_wdPortAState;
bool g_wdPortBState;
bool g_wdPortCState;
bool g_wdHelpState;
bool g_wdTotalState;

bool g_ztAState;
bool g_ztBState;
bool g_ztCState;
bool g_ztPortAState;
bool g_ztPortBState;
bool g_ztPortCState;
bool g_ztHelpState;
bool g_ztTotalState;

MainWindow* g_pMainWindow;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g_pMainWindow = new MainWindow();
    g_pMainWindow->showFullScreen();

    return a.exec();
}
