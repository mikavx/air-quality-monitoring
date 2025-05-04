/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCharts/QChartView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QChartView *chartView;
    QHBoxLayout *horizontalLayout;
    QPushButton *loadDataButton;
    QPushButton *saveDataButton;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QTextEdit *measurementDataTextEdit;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_2;
    QListWidget *sensorsListWidget;
    QVBoxLayout *verticalLayout_3;
    QPushButton *analyzeButton;
    QTextEdit *analysisResultsTextEdit;
    QVBoxLayout *verticalLayout;
    QPushButton *fetchStationsButton;
    QLineEdit *cityFilterLineEdit;
    QListWidget *listWidget;
    QLabel *selectedStationLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName("mainWindow");
        mainWindow->resize(1056, 390);
        mainWindow->setStyleSheet(QString::fromUtf8("/* --- Styl dla G\305\202\303\263wnego Okna (T\305\202o) --- */\n"
"QMainWindow {\n"
"    background-color: #5e3c75; \n"
"}\n"
"\n"
"/* --- Styl dla Centralnego Widgetu --- */\n"
"QWidget#centralwidget {\n"
"    background-color: #e6d8f3;  \n"
"}\n"
"\n"
"/* --- Styl dla Przycisk\303\263w --- */\n"
"QPushButton {\n"
"    background-color: #a86dbf;  \n"
"    color: #ffffff;\n"
"    border: 2px solid #50325d; \n"
"    border-radius: 8px;\n"
"    padding: 5px 10px;\n"
"    font-weight: bold;\n"
"    font-family: \"Verdana\", Arial, sans-serif;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #6e4e93; \n"
"    border-color: #3f2b57;    \n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #50325d; \n"
"    border-style: inset;\n"
"    border-color: #3a2247;\n"
"}\n"
"\n"
"QPushButton:disabled {\n"
"    background-color: #a6b4e2; \n"
"    color: #e7d8f3;\n"
"    border-color: #b0c4de;\n"
"}\n"
"\n"
"/* --- Styl dla Etykiet Tekstowych --- */\n"
"QLabel {\n"
"    color: #50325d; \n"
"    font-we"
                        "ight: bold;\n"
"    font-size: 10pt;\n"
"    padding: 2px;\n"
"	font-family: \"Verdana\", Arial, sans-serif;\n"
"    qproperty-alignment: 'AlignCenter';\n"
"}\n"
"\n"
"/* --- Styl dla Konkretnych Etykiet --- */\n"
"QLabel#analysisResultsLabel {\n"
"    color: #50325d;\n"
"    font-weight: normal;\n"
"    font-size: 9pt;\n"
"    border: 1px solid #8966b1;\n"
"    background-color: #f2f8fd;\n"
"    padding: 5px;\n"
"}\n"
"\n"
"/* --- Styl dla Pola Wprowadzania (Filtra) --- */\n"
"QLineEdit {\n"
"    border: 1px solid #8966b1;\n"
"    border-radius: 5px;\n"
"    padding: 3px 5px; \n"
"    background-color: #f4f9fd;\n"
"    font-family: \"Verdana\", Arial, sans-serif;\n"
"    font-size: 10pt;\n"
"    color: #50325d;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QLineEdit:focus {\n"
"    border-color: #50325d;\n"
"}\n"
"\n"
"/* --- Styl dla List --- */\n"
"QListWidget {\n"
"    border: 1px solid #8966b1;\n"
"    background-color: #ffffff;\n"
"    font-size: 10pt;\n"
"    color: #50325d;\n"
"	font-family: \"Verdana\", A"
                        "rial, sans-serif;\n"
"}\n"
"\n"
"/* --- Styl dla P\303\263l Tekstowych (Wyniki, Dane) --- */\n"
"QTextEdit {\n"
"    border: 1px solid #8966b1;\n"
"    background-color: #ffffff;\n"
"    font-family: \"Verdana\", Arial, sans-serif;\n"
"    font-size: 9pt;\n"
"    color: #50325d;\n"
"}\n"
"\n"
"/* --- Styl dla Wykresu (Usuni\304\231cie Ramki) --- */\n"
"QChartView {\n"
"    border: none; \n"
"    background-color: #ffffff;\n"
"}\n"
"\n"
"/* --- Styl dla Paska Statusu (Opcjonalnie) --- */\n"
"QStatusBar {\n"
"    background-color: #50325d;\n"
"    color: #f4f4f4;\n"
"    font-family: \"Verdana\", Arial, sans-serif;\n"
"}\n"
""));
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        chartView = new QChartView(centralwidget);
        chartView->setObjectName("chartView");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(chartView->sizePolicy().hasHeightForWidth());
        chartView->setSizePolicy(sizePolicy);

        gridLayout->addWidget(chartView, 2, 0, 1, 4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        loadDataButton = new QPushButton(centralwidget);
        loadDataButton->setObjectName("loadDataButton");

        horizontalLayout->addWidget(loadDataButton);

        saveDataButton = new QPushButton(centralwidget);
        saveDataButton->setObjectName("saveDataButton");

        horizontalLayout->addWidget(saveDataButton);


        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 2);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName("verticalLayout_4");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        verticalLayout_4->addWidget(label);

        measurementDataTextEdit = new QTextEdit(centralwidget);
        measurementDataTextEdit->setObjectName("measurementDataTextEdit");
        measurementDataTextEdit->setReadOnly(true);

        verticalLayout_4->addWidget(measurementDataTextEdit);


        gridLayout->addLayout(verticalLayout_4, 1, 2, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);

        sensorsListWidget = new QListWidget(centralwidget);
        sensorsListWidget->setObjectName("sensorsListWidget");

        verticalLayout_2->addWidget(sensorsListWidget);


        gridLayout->addLayout(verticalLayout_2, 1, 1, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        analyzeButton = new QPushButton(centralwidget);
        analyzeButton->setObjectName("analyzeButton");

        verticalLayout_3->addWidget(analyzeButton);

        analysisResultsTextEdit = new QTextEdit(centralwidget);
        analysisResultsTextEdit->setObjectName("analysisResultsTextEdit");
        analysisResultsTextEdit->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        analysisResultsTextEdit->setReadOnly(true);

        verticalLayout_3->addWidget(analysisResultsTextEdit);


        gridLayout->addLayout(verticalLayout_3, 0, 3, 2, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        fetchStationsButton = new QPushButton(centralwidget);
        fetchStationsButton->setObjectName("fetchStationsButton");

        verticalLayout->addWidget(fetchStationsButton);

        cityFilterLineEdit = new QLineEdit(centralwidget);
        cityFilterLineEdit->setObjectName("cityFilterLineEdit");

        verticalLayout->addWidget(cityFilterLineEdit);

        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");

        verticalLayout->addWidget(listWidget);

        selectedStationLabel = new QLabel(centralwidget);
        selectedStationLabel->setObjectName("selectedStationLabel");

        verticalLayout->addWidget(selectedStationLabel);


        gridLayout->addLayout(verticalLayout, 0, 0, 2, 1);

        mainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1056, 17));
        mainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(mainWindow);
        statusbar->setObjectName("statusbar");
        mainWindow->setStatusBar(statusbar);

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QCoreApplication::translate("mainWindow", "MainWindow", nullptr));
        loadDataButton->setText(QCoreApplication::translate("mainWindow", "Wczytaj dane", nullptr));
        saveDataButton->setText(QCoreApplication::translate("mainWindow", "Zapisz dane", nullptr));
        label->setText(QCoreApplication::translate("mainWindow", "Dane pomiarowe", nullptr));
        label_2->setText(QCoreApplication::translate("mainWindow", "Dost\304\231pne sensory", nullptr));
        analyzeButton->setText(QCoreApplication::translate("mainWindow", "Analizuj dane", nullptr));
        fetchStationsButton->setText(QCoreApplication::translate("mainWindow", "Pobierz stacje", nullptr));
        cityFilterLineEdit->setPlaceholderText(QCoreApplication::translate("mainWindow", "Wpisz miejscowo\305\233\304\207...", nullptr));
        selectedStationLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
