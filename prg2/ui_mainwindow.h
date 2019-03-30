/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *graphics_view;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *towns_checkbox;
    QCheckBox *vassals_checkbox;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QToolButton *zoom_plus;
    QToolButton *zoom_minus;
    QHBoxLayout *horizontalLayout_4;
    QToolButton *zoom_1;
    QToolButton *zoom_fit;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_3;
    QDoubleSpinBox *fontscale;
    QPlainTextEdit *output;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QLabel *cmd_info_text;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *cmd_select;
    QLabel *label_2;
    QComboBox *number_select;
    QPushButton *file_button;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout_3;
    QPushButton *stop_button;
    QPushButton *execute_button;
    QPushButton *clear_input_button;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(747, 563);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        graphics_view = new QGraphicsView(layoutWidget);
        graphics_view->setObjectName(QStringLiteral("graphics_view"));
        graphics_view->setInteractive(true);
        graphics_view->setDragMode(QGraphicsView::NoDrag);

        horizontalLayout_2->addWidget(graphics_view);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        towns_checkbox = new QCheckBox(layoutWidget);
        towns_checkbox->setObjectName(QStringLiteral("towns_checkbox"));
        towns_checkbox->setChecked(true);

        verticalLayout_2->addWidget(towns_checkbox);

        vassals_checkbox = new QCheckBox(layoutWidget);
        vassals_checkbox->setObjectName(QStringLiteral("vassals_checkbox"));
        vassals_checkbox->setChecked(true);

        verticalLayout_2->addWidget(vassals_checkbox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        zoom_plus = new QToolButton(layoutWidget);
        zoom_plus->setObjectName(QStringLiteral("zoom_plus"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(zoom_plus->sizePolicy().hasHeightForWidth());
        zoom_plus->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(zoom_plus);

        zoom_minus = new QToolButton(layoutWidget);
        zoom_minus->setObjectName(QStringLiteral("zoom_minus"));
        sizePolicy.setHeightForWidth(zoom_minus->sizePolicy().hasHeightForWidth());
        zoom_minus->setSizePolicy(sizePolicy);

        horizontalLayout_3->addWidget(zoom_minus);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        zoom_1 = new QToolButton(layoutWidget);
        zoom_1->setObjectName(QStringLiteral("zoom_1"));
        sizePolicy.setHeightForWidth(zoom_1->sizePolicy().hasHeightForWidth());
        zoom_1->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(zoom_1);

        zoom_fit = new QToolButton(layoutWidget);
        zoom_fit->setObjectName(QStringLiteral("zoom_fit"));
        sizePolicy.setHeightForWidth(zoom_fit->sizePolicy().hasHeightForWidth());
        zoom_fit->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(zoom_fit);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_2->addWidget(label_3);

        fontscale = new QDoubleSpinBox(layoutWidget);
        fontscale->setObjectName(QStringLiteral("fontscale"));
        fontscale->setMinimum(0.01);
        fontscale->setSingleStep(0.1);
        fontscale->setValue(1);

        verticalLayout_2->addWidget(fontscale);


        horizontalLayout_2->addLayout(verticalLayout_2);

        splitter->addWidget(layoutWidget);
        output = new QPlainTextEdit(splitter);
        output->setObjectName(QStringLiteral("output"));
        output->setUndoRedoEnabled(false);
        output->setLineWrapMode(QPlainTextEdit::NoWrap);
        splitter->addWidget(output);

        verticalLayout->addWidget(splitter);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        cmd_info_text = new QLabel(centralWidget);
        cmd_info_text->setObjectName(QStringLiteral("cmd_info_text"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cmd_info_text->sizePolicy().hasHeightForWidth());
        cmd_info_text->setSizePolicy(sizePolicy1);

        verticalLayout_4->addWidget(cmd_info_text);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        verticalLayout_4->addWidget(lineEdit);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy2);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label);

        cmd_select = new QComboBox(centralWidget);
        cmd_select->setObjectName(QStringLiteral("cmd_select"));

        horizontalLayout->addWidget(cmd_select);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label_2);

        number_select = new QComboBox(centralWidget);
        number_select->setObjectName(QStringLiteral("number_select"));

        horizontalLayout->addWidget(number_select);

        file_button = new QPushButton(centralWidget);
        file_button->setObjectName(QStringLiteral("file_button"));

        horizontalLayout->addWidget(file_button);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_4->addLayout(horizontalLayout);


        horizontalLayout_5->addLayout(verticalLayout_4);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        stop_button = new QPushButton(centralWidget);
        stop_button->setObjectName(QStringLiteral("stop_button"));
        stop_button->setEnabled(false);

        verticalLayout_3->addWidget(stop_button);

        execute_button = new QPushButton(centralWidget);
        execute_button->setObjectName(QStringLiteral("execute_button"));

        verticalLayout_3->addWidget(execute_button);

        clear_input_button = new QPushButton(centralWidget);
        clear_input_button->setObjectName(QStringLiteral("clear_input_button"));

        verticalLayout_3->addWidget(clear_input_button);


        horizontalLayout_5->addLayout(verticalLayout_3);


        verticalLayout->addLayout(horizontalLayout_5);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        towns_checkbox->setText(QApplication::translate("MainWindow", "Towns", 0));
        vassals_checkbox->setText(QApplication::translate("MainWindow", "Vassals", 0));
        zoom_plus->setText(QApplication::translate("MainWindow", "+", 0));
        zoom_minus->setText(QApplication::translate("MainWindow", "-", 0));
        zoom_1->setText(QApplication::translate("MainWindow", "1:1", 0));
        zoom_fit->setText(QApplication::translate("MainWindow", "Fit", 0));
        label_3->setText(QApplication::translate("MainWindow", "Fontscale", 0));
        cmd_info_text->setText(QString());
        label->setText(QApplication::translate("MainWindow", "Command:", 0));
        label_2->setText(QApplication::translate("MainWindow", "Number:", 0));
        file_button->setText(QApplication::translate("MainWindow", "File...", 0));
        stop_button->setText(QApplication::translate("MainWindow", "Stop test", 0));
        execute_button->setText(QApplication::translate("MainWindow", "Execute", 0));
        clear_input_button->setText(QApplication::translate("MainWindow", "Clear input", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
