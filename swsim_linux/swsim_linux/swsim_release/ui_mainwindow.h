/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTableWidget *tblDZInfo;
    QPushButton *btnAddDZ;
    QPushButton *btnDelDZ;
    QPushButton *btnRunCtrl;
    QPushButton *btnRefresh;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1091, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        tblDZInfo = new QTableWidget(centralwidget);
        if (tblDZInfo->columnCount() < 8)
            tblDZInfo->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tblDZInfo->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tblDZInfo->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tblDZInfo->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tblDZInfo->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tblDZInfo->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tblDZInfo->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tblDZInfo->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tblDZInfo->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tblDZInfo->setObjectName(QStringLiteral("tblDZInfo"));
        tblDZInfo->setGeometry(QRect(5, 1, 861, 551));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tblDZInfo->sizePolicy().hasHeightForWidth());
        tblDZInfo->setSizePolicy(sizePolicy);
        btnAddDZ = new QPushButton(centralwidget);
        btnAddDZ->setObjectName(QStringLiteral("btnAddDZ"));
        btnAddDZ->setGeometry(QRect(910, 90, 131, 61));
        btnDelDZ = new QPushButton(centralwidget);
        btnDelDZ->setObjectName(QStringLiteral("btnDelDZ"));
        btnDelDZ->setGeometry(QRect(910, 170, 131, 61));
        btnRunCtrl = new QPushButton(centralwidget);
        btnRunCtrl->setObjectName(QStringLiteral("btnRunCtrl"));
        btnRunCtrl->setGeometry(QRect(940, 470, 71, 71));
        btnRunCtrl->setStyleSheet(QStringLiteral("border-image: url(:/pics/start.jpg);"));
        btnRefresh = new QPushButton(centralwidget);
        btnRefresh->setObjectName(QStringLiteral("btnRefresh"));
        btnRefresh->setGeometry(QRect(910, 10, 131, 61));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1091, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        QTableWidgetItem *___qtablewidgetitem = tblDZInfo->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("MainWindow", "\347\224\265\347\253\231\345\220\215\347\247\260", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tblDZInfo->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("MainWindow", "\347\224\265\347\253\231\344\273\243\347\240\201", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tblDZInfo->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("MainWindow", "\346\265\201\351\207\217\346\225\260\346\215\256", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tblDZInfo->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("MainWindow", "\346\260\264\344\275\215\346\225\260\346\215\256", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tblDZInfo->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("MainWindow", "\346\265\201\351\200\237\346\225\260\346\215\256", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tblDZInfo->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("MainWindow", "\351\227\270\351\227\250\345\274\200\345\272\246", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tblDZInfo->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("MainWindow", "\346\234\200\345\260\217\344\270\213\346\263\204\346\265\201\351\207\217", 0));
        QTableWidgetItem *___qtablewidgetitem7 = tblDZInfo->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("MainWindow", "\347\224\265\346\261\240\347\224\265\345\216\213", 0));
        btnAddDZ->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\347\224\265\347\253\231", 0));
        btnDelDZ->setText(QApplication::translate("MainWindow", "\345\210\240\351\231\244\347\224\265\347\253\231", 0));
        btnRunCtrl->setText(QString());
        btnRefresh->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260\347\224\265\347\253\231", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
