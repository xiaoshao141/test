/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *dzId;
    QLineEdit *level;
    QLineEdit *flow_rate;
    QLineEdit *flow;
    QLineEdit *gatage;
    QLineEdit *voltage;
    QPushButton *btnAddDZ;
    QLabel *label_7;
    QLineEdit *dzName;
    QLabel *label_8;
    QLineEdit *min_flow;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(591, 537);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dialog->sizePolicy().hasHeightForWidth());
        Dialog->setSizePolicy(sizePolicy);
        label = new QLabel(Dialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(170, 70, 61, 21));
        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(170, 120, 72, 21));
        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(170, 170, 72, 21));
        label_4 = new QLabel(Dialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(170, 220, 72, 21));
        label_5 = new QLabel(Dialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(170, 270, 72, 21));
        label_6 = new QLabel(Dialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(170, 320, 72, 21));
        dzId = new QLineEdit(Dialog);
        dzId->setObjectName(QStringLiteral("dzId"));
        dzId->setGeometry(QRect(270, 70, 151, 21));
        level = new QLineEdit(Dialog);
        level->setObjectName(QStringLiteral("level"));
        level->setGeometry(QRect(270, 120, 151, 21));
        flow_rate = new QLineEdit(Dialog);
        flow_rate->setObjectName(QStringLiteral("flow_rate"));
        flow_rate->setGeometry(QRect(270, 170, 151, 21));
        flow = new QLineEdit(Dialog);
        flow->setObjectName(QStringLiteral("flow"));
        flow->setGeometry(QRect(270, 220, 151, 21));
        gatage = new QLineEdit(Dialog);
        gatage->setObjectName(QStringLiteral("gatage"));
        gatage->setGeometry(QRect(270, 270, 151, 21));
        voltage = new QLineEdit(Dialog);
        voltage->setObjectName(QStringLiteral("voltage"));
        voltage->setGeometry(QRect(270, 320, 151, 21));
        btnAddDZ = new QPushButton(Dialog);
        btnAddDZ->setObjectName(QStringLiteral("btnAddDZ"));
        btnAddDZ->setGeometry(QRect(210, 450, 161, 61));
        label_7 = new QLabel(Dialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(170, 20, 61, 21));
        dzName = new QLineEdit(Dialog);
        dzName->setObjectName(QStringLiteral("dzName"));
        dzName->setGeometry(QRect(270, 20, 151, 21));
        label_8 = new QLabel(Dialog);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(170, 370, 72, 21));
        min_flow = new QLineEdit(Dialog);
        min_flow->setObjectName(QStringLiteral("min_flow"));
        min_flow->setGeometry(QRect(270, 370, 151, 21));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        label->setText(QApplication::translate("Dialog", "\347\224\265\347\253\231\344\273\243\347\240\201", 0));
        label_2->setText(QApplication::translate("Dialog", "\345\256\236\346\227\266\346\260\264\344\275\215", 0));
        label_3->setText(QApplication::translate("Dialog", "\345\256\236\346\227\266\346\265\201\351\200\237", 0));
        label_4->setText(QApplication::translate("Dialog", "\345\256\236\346\227\266\346\265\201\351\207\217", 0));
        label_5->setText(QApplication::translate("Dialog", "\351\227\270\351\227\250\345\274\200\345\272\246", 0));
        label_6->setText(QApplication::translate("Dialog", "\345\256\236\346\227\266\347\224\265\345\216\213", 0));
        dzId->setText(QApplication::translate("Dialog", "4330222000", 0));
        level->setText(QApplication::translate("Dialog", "3.205", 0));
        flow_rate->setText(QApplication::translate("Dialog", "0.181", 0));
        flow->setText(QApplication::translate("Dialog", "4.872", 0));
        gatage->setText(QApplication::translate("Dialog", "0", 0));
        voltage->setText(QApplication::translate("Dialog", "12.73", 0));
        btnAddDZ->setText(QApplication::translate("Dialog", "\346\267\273\345\212\240\347\224\265\347\253\231", 0));
        label_7->setText(QApplication::translate("Dialog", "\347\224\265\347\253\231\345\220\215\347\247\260", 0));
        dzName->setText(QApplication::translate("Dialog", "\347\250\213\347\246\276\346\272\252\347\224\265\347\253\231", 0));
        label_8->setText(QApplication::translate("Dialog", "\346\234\200\345\260\217\344\270\213\346\263\204\346\265\201\351\207\217", 0));
        min_flow->setText(QApplication::translate("Dialog", "0.05", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
