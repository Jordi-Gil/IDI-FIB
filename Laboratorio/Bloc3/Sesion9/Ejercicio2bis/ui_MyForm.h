/********************************************************************************
** Form generated from reading UI file 'MyForm.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYFORM_H
#define UI_MYFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "MyGLWidget.h"
#include "MyLabel.h"
#include "MyPushButton.h"

QT_BEGIN_NAMESPACE

class Ui_MyForm
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_9;
    MyGLWidget *widget;
    MyPushButton *pushButton_3;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *radioButton;
    QSlider *horizontalSlider_2;
    QRadioButton *radioButton_2;
    QSlider *horizontalSlider;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QComboBox *comboBox;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label;
    QDial *dial_5;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_2;
    QDial *dial_2;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_6;
    QDoubleSpinBox *doubleSpinBox;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_7;
    MyLabel *label_5;
    QSpinBox *spinBox_5;
    QSpinBox *spinBox_4;
    QSpinBox *spinBox_3;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton;

    void setupUi(QWidget *MyForm)
    {
        if (MyForm->objectName().isEmpty())
            MyForm->setObjectName(QStringLiteral("MyForm"));
        MyForm->resize(981, 1260);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MyForm->sizePolicy().hasHeightForWidth());
        MyForm->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(MyForm);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        widget = new MyGLWidget(MyForm);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        widget->setMaximumSize(QSize(1000, 1000));

        verticalLayout_9->addWidget(widget);

        pushButton_3 = new MyPushButton(MyForm);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        sizePolicy.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy);

        verticalLayout_9->addWidget(pushButton_3, 0, Qt::AlignBottom);


        horizontalLayout->addLayout(verticalLayout_9);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(MyForm);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        groupBox->setCheckable(false);
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setAutoExclusive(true);

        verticalLayout_2->addWidget(radioButton);

        horizontalSlider_2 = new QSlider(groupBox);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Ignored, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(horizontalSlider_2->sizePolicy().hasHeightForWidth());
        horizontalSlider_2->setSizePolicy(sizePolicy3);
        horizontalSlider_2->setMinimum(0);
        horizontalSlider_2->setMaximum(85);
        horizontalSlider_2->setValue(0);
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(horizontalSlider_2);

        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setChecked(false);
        radioButton_2->setAutoExclusive(true);

        verticalLayout_2->addWidget(radioButton_2);

        horizontalSlider = new QSlider(groupBox);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        sizePolicy3.setHeightForWidth(horizontalSlider->sizePolicy().hasHeightForWidth());
        horizontalSlider->setSizePolicy(sizePolicy3);
        horizontalSlider->setMaximum(100);
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(horizontalSlider);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(MyForm);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setEnabled(true);
        sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy2);
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        comboBox = new QComboBox(groupBox_2);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);
        comboBox->setMaximumSize(QSize(100, 20));

        verticalLayout_3->addWidget(comboBox);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(MyForm);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setEnabled(true);
        sizePolicy2.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy2);
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMaximumSize(QSize(50, 50));

        horizontalLayout_7->addWidget(label);

        dial_5 = new QDial(groupBox_3);
        dial_5->setObjectName(QStringLiteral("dial_5"));
        sizePolicy.setHeightForWidth(dial_5->sizePolicy().hasHeightForWidth());
        dial_5->setSizePolicy(sizePolicy);
        dial_5->setMaximumSize(QSize(50, 50));
        dial_5->setMaximum(360);

        horizontalLayout_7->addWidget(dial_5);


        verticalLayout_5->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMaximumSize(QSize(50, 50));

        horizontalLayout_8->addWidget(label_2);

        dial_2 = new QDial(groupBox_3);
        dial_2->setObjectName(QStringLiteral("dial_2"));
        sizePolicy.setHeightForWidth(dial_2->sizePolicy().hasHeightForWidth());
        dial_2->setSizePolicy(sizePolicy);
        dial_2->setMaximumSize(QSize(50, 50));
        dial_2->setMaximum(360);

        horizontalLayout_8->addWidget(dial_2);


        verticalLayout_5->addLayout(horizontalLayout_8);


        verticalLayout->addWidget(groupBox_3);

        groupBox_5 = new QGroupBox(MyForm);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        verticalLayout_6 = new QVBoxLayout(groupBox_5);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        doubleSpinBox = new QDoubleSpinBox(groupBox_5);
        doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
        doubleSpinBox->setMinimum(-99.99);

        verticalLayout_6->addWidget(doubleSpinBox);


        verticalLayout->addWidget(groupBox_5);

        groupBox_6 = new QGroupBox(MyForm);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        verticalLayout_7 = new QVBoxLayout(groupBox_6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        label_5 = new MyLabel(groupBox_6);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_7->addWidget(label_5);

        spinBox_5 = new QSpinBox(groupBox_6);
        spinBox_5->setObjectName(QStringLiteral("spinBox_5"));
        spinBox_5->setMaximum(255);

        verticalLayout_7->addWidget(spinBox_5);

        spinBox_4 = new QSpinBox(groupBox_6);
        spinBox_4->setObjectName(QStringLiteral("spinBox_4"));
        spinBox_4->setMaximum(255);

        verticalLayout_7->addWidget(spinBox_4);

        spinBox_3 = new QSpinBox(groupBox_6);
        spinBox_3->setObjectName(QStringLiteral("spinBox_3"));
        spinBox_3->setMaximum(255);

        verticalLayout_7->addWidget(spinBox_3);

        pushButton_2 = new QPushButton(groupBox_6);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        verticalLayout_7->addWidget(pushButton_2);


        verticalLayout->addWidget(groupBox_6);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        pushButton = new QPushButton(MyForm);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(pushButton, 0, Qt::AlignHCenter);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(MyForm);
        QObject::connect(pushButton, SIGNAL(clicked()), MyForm, SLOT(close()));
        QObject::connect(radioButton, SIGNAL(clicked(bool)), widget, SLOT(Perspective(bool)));
        QObject::connect(radioButton_2, SIGNAL(clicked(bool)), widget, SLOT(Ortogonal(bool)));
        QObject::connect(comboBox, SIGNAL(currentTextChanged(QString)), widget, SLOT(CarregaModel(QString)));
        QObject::connect(radioButton, SIGNAL(toggled(bool)), horizontalSlider_2, SLOT(setVisible(bool)));
        QObject::connect(radioButton_2, SIGNAL(toggled(bool)), horizontalSlider, SLOT(setVisible(bool)));
        QObject::connect(horizontalSlider, SIGNAL(sliderMoved(int)), widget, SLOT(ZoomOrtogonal(int)));
        QObject::connect(horizontalSlider_2, SIGNAL(sliderMoved(int)), widget, SLOT(ZoomPerspectiva(int)));
        QObject::connect(dial_2, SIGNAL(sliderMoved(int)), widget, SLOT(updatePsi(int)));
        QObject::connect(widget, SIGNAL(updatepsi(int)), dial_2, SLOT(setValue(int)));
        QObject::connect(doubleSpinBox, SIGNAL(valueChanged(double)), widget, SLOT(setEscalat(double)));
        QObject::connect(widget, SIGNAL(updateEscalat(double)), doubleSpinBox, SLOT(setValue(double)));
        QObject::connect(pushButton_2, SIGNAL(clicked()), label_5, SLOT(send()));
        QObject::connect(label_5, SIGNAL(setred(int)), widget, SLOT(setRed(int)));
        QObject::connect(label_5, SIGNAL(setgreen(int)), widget, SLOT(setGreen(int)));
        QObject::connect(label_5, SIGNAL(setblue(int)), widget, SLOT(setBlue(int)));
        QObject::connect(spinBox_3, SIGNAL(valueChanged(int)), label_5, SLOT(updateblue(int)));
        QObject::connect(spinBox_4, SIGNAL(valueChanged(int)), label_5, SLOT(updategreen(int)));
        QObject::connect(spinBox_5, SIGNAL(valueChanged(int)), label_5, SLOT(updatered(int)));
        QObject::connect(pushButton_3, SIGNAL(clicked()), pushButton_3, SLOT(myClicked()));
        QObject::connect(pushButton_3, SIGNAL(setFile(QString)), widget, SLOT(setSong(QString)));
        QObject::connect(widget, SIGNAL(updateslort(int)), horizontalSlider, SLOT(setValue(int)));
        QObject::connect(widget, SIGNAL(updateslfov(int)), horizontalSlider_2, SLOT(setValue(int)));
        QObject::connect(widget, SIGNAL(updateOrt()), radioButton_2, SLOT(click()));
        QObject::connect(widget, SIGNAL(updatePers()), radioButton, SLOT(click()));
        QObject::connect(dial_5, SIGNAL(sliderMoved(int)), widget, SLOT(updateTheta(int)));
        QObject::connect(widget, SIGNAL(updatetheta(int)), dial_5, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(MyForm);
    } // setupUi

    void retranslateUi(QWidget *MyForm)
    {
        MyForm->setWindowTitle(QApplication::translate("MyForm", "Form", 0));
        pushButton_3->setText(QApplication::translate("MyForm", "Examinar", 0));
        groupBox->setTitle(QApplication::translate("MyForm", "\303\223ptica", 0));
        radioButton->setText(QApplication::translate("MyForm", "Perspec&tive", 0));
        radioButton_2->setText(QApplication::translate("MyForm", "Ortogona&l", 0));
        groupBox_2->setTitle(QApplication::translate("MyForm", "Objectes", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("MyForm", "Patricio", 0)
         << QApplication::translate("MyForm", "Homer", 0)
         << QApplication::translate("MyForm", "Legoman", 0)
         << QApplication::translate("MyForm", "Legoman Assegut", 0)
         << QApplication::translate("MyForm", "Shaun Hastings", 0)
         << QApplication::translate("MyForm", "Vaca", 0)
         << QApplication::translate("MyForm", "Delfin", 0)
         << QApplication::translate("MyForm", "F16", 0)
         << QApplication::translate("MyForm", "Porsche", 0)
        );
        groupBox_3->setTitle(QApplication::translate("MyForm", "C\303\241mera", 0));
        label->setText(QApplication::translate("MyForm", "Theta", 0));
        label_2->setText(QApplication::translate("MyForm", "Psi", 0));
        groupBox_5->setTitle(QApplication::translate("MyForm", "Escalat", 0));
        groupBox_6->setTitle(QApplication::translate("MyForm", "Color", 0));
        label_5->setText(QString());
        pushButton_2->setText(QApplication::translate("MyForm", "Pinta", 0));
        pushButton->setText(QApplication::translate("MyForm", "&Sortir", 0));
    } // retranslateUi

};

namespace Ui {
    class MyForm: public Ui_MyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYFORM_H
