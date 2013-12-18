/********************************************************************************
** Form generated from reading UI file 'analyzerwv2554.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ANALYZERWV2554_H
#define ANALYZERWV2554_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Gui
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *receiverTab;
    QTextEdit *readPayloadDumpEdit;
    QScrollArea *readPayloadFieldsFrame;
    QWidget *scrollAreaWidgetContents;
    QPushButton *parseNumericButton;
    QPushButton *parseWideStringButton;
    QPushButton *clearReadPayloadFieldsButton;
    QPushButton *parseStringButton;
    QWidget *senderTab;
    QScrollArea *writePayloadFieldsFrame;
    QWidget *scrollAreaWidgetContents_2;
    QTextEdit *writePayloadDumpEdit;
    QPushButton *sendDumpButton;
    QPushButton *sendFieldsButton;
    QPushButton *clearWritePayloadButton;
    QPushButton *dumpButton;
    QSlider *writePayloadFieldsCountSlider;
    QLabel *writePayloadFieldsCountLabel;
    QLabel *writePayloadFieldsTitleLabel;
    QLabel *writePayloadSizeTitleLabel;
    QLabel *writePayloadSizeLabel;
    QPushButton *calculateSizeButton;
    QTreeView *connectionsView;
    QPushButton *removePayloadButton;
    QPushButton *disconnectButton;
    QLabel *activeConnectionsTitleLabel;

    void setupUi(QMainWindow *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName(QStringLiteral("Gui"));
        Gui->resize(868, 898);
        centralwidget = new QWidget(Gui);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(260, 0, 621, 911));
        receiverTab = new QWidget();
        receiverTab->setObjectName(QStringLiteral("receiverTab"));
        readPayloadDumpEdit = new QTextEdit(receiverTab);
        readPayloadDumpEdit->setObjectName(QStringLiteral("readPayloadDumpEdit"));
        readPayloadDumpEdit->setGeometry(QRect(10, 2, 591, 100));
        readPayloadDumpEdit->setUndoRedoEnabled(false);
        readPayloadDumpEdit->setReadOnly(false);
        readPayloadFieldsFrame = new QScrollArea(receiverTab);
        readPayloadFieldsFrame->setObjectName(QStringLiteral("readPayloadFieldsFrame"));
        readPayloadFieldsFrame->setGeometry(QRect(10, 136, 591, 725));
        readPayloadFieldsFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        readPayloadFieldsFrame->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 589, 723));
        readPayloadFieldsFrame->setWidget(scrollAreaWidgetContents);
        parseNumericButton = new QPushButton(receiverTab);
        parseNumericButton->setObjectName(QStringLiteral("parseNumericButton"));
        parseNumericButton->setGeometry(QRect(470, 106, 131, 24));
        parseWideStringButton = new QPushButton(receiverTab);
        parseWideStringButton->setObjectName(QStringLiteral("parseWideStringButton"));
        parseWideStringButton->setGeometry(QRect(356, 106, 110, 24));
        clearReadPayloadFieldsButton = new QPushButton(receiverTab);
        clearReadPayloadFieldsButton->setObjectName(QStringLiteral("clearReadPayloadFieldsButton"));
        clearReadPayloadFieldsButton->setGeometry(QRect(180, 106, 61, 24));
        parseStringButton = new QPushButton(receiverTab);
        parseStringButton->setObjectName(QStringLiteral("parseStringButton"));
        parseStringButton->setGeometry(QRect(243, 106, 110, 24));
        tabWidget->addTab(receiverTab, QString());
        senderTab = new QWidget();
        senderTab->setObjectName(QStringLiteral("senderTab"));
        writePayloadFieldsFrame = new QScrollArea(senderTab);
        writePayloadFieldsFrame->setObjectName(QStringLiteral("writePayloadFieldsFrame"));
        writePayloadFieldsFrame->setGeometry(QRect(10, 134, 591, 706));
        writePayloadFieldsFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        writePayloadFieldsFrame->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 589, 704));
        writePayloadFieldsFrame->setWidget(scrollAreaWidgetContents_2);
        writePayloadDumpEdit = new QTextEdit(senderTab);
        writePayloadDumpEdit->setObjectName(QStringLiteral("writePayloadDumpEdit"));
        writePayloadDumpEdit->setGeometry(QRect(10, 2, 591, 100));
        writePayloadDumpEdit->setUndoRedoEnabled(false);
        writePayloadDumpEdit->setReadOnly(false);
        sendDumpButton = new QPushButton(senderTab);
        sendDumpButton->setObjectName(QStringLiteral("sendDumpButton"));
        sendDumpButton->setGeometry(QRect(502, 106, 99, 24));
        sendFieldsButton = new QPushButton(senderTab);
        sendFieldsButton->setObjectName(QStringLiteral("sendFieldsButton"));
        sendFieldsButton->setGeometry(QRect(540, 843, 61, 23));
        clearWritePayloadButton = new QPushButton(senderTab);
        clearWritePayloadButton->setObjectName(QStringLiteral("clearWritePayloadButton"));
        clearWritePayloadButton->setGeometry(QRect(9, 843, 61, 23));
        dumpButton = new QPushButton(senderTab);
        dumpButton->setObjectName(QStringLiteral("dumpButton"));
        dumpButton->setGeometry(QRect(478, 843, 58, 23));
        writePayloadFieldsCountSlider = new QSlider(senderTab);
        writePayloadFieldsCountSlider->setObjectName(QStringLiteral("writePayloadFieldsCountSlider"));
        writePayloadFieldsCountSlider->setGeometry(QRect(307, 107, 161, 23));
        writePayloadFieldsCountSlider->setMaximum(25);
        writePayloadFieldsCountSlider->setPageStep(1);
        writePayloadFieldsCountSlider->setOrientation(Qt::Horizontal);
        writePayloadFieldsCountLabel = new QLabel(senderTab);
        writePayloadFieldsCountLabel->setObjectName(QStringLiteral("writePayloadFieldsCountLabel"));
        writePayloadFieldsCountLabel->setGeometry(QRect(476, 109, 31, 16));
        writePayloadFieldsTitleLabel = new QLabel(senderTab);
        writePayloadFieldsTitleLabel->setObjectName(QStringLiteral("writePayloadFieldsTitleLabel"));
        writePayloadFieldsTitleLabel->setGeometry(QRect(258, 110, 41, 16));
        writePayloadSizeTitleLabel = new QLabel(senderTab);
        writePayloadSizeTitleLabel->setObjectName(QStringLiteral("writePayloadSizeTitleLabel"));
        writePayloadSizeTitleLabel->setGeometry(QRect(10, 110, 41, 17));
        writePayloadSizeLabel = new QLabel(senderTab);
        writePayloadSizeLabel->setObjectName(QStringLiteral("writePayloadSizeLabel"));
        writePayloadSizeLabel->setGeometry(QRect(50, 110, 67, 17));
        calculateSizeButton = new QPushButton(senderTab);
        calculateSizeButton->setObjectName(QStringLiteral("calculateSizeButton"));
        calculateSizeButton->setGeometry(QRect(96, 106, 99, 24));
        tabWidget->addTab(senderTab, QString());
        connectionsView = new QTreeView(centralwidget);
        connectionsView->setObjectName(QStringLiteral("connectionsView"));
        connectionsView->setGeometry(QRect(-5, 30, 266, 831));
        connectionsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        removePayloadButton = new QPushButton(centralwidget);
        removePayloadButton->setObjectName(QStringLiteral("removePayloadButton"));
        removePayloadButton->setGeometry(QRect(2, 867, 121, 27));
        disconnectButton = new QPushButton(centralwidget);
        disconnectButton->setObjectName(QStringLiteral("disconnectButton"));
        disconnectButton->setGeometry(QRect(132, 867, 121, 27));
        activeConnectionsTitleLabel = new QLabel(centralwidget);
        activeConnectionsTitleLabel->setObjectName(QStringLiteral("activeConnectionsTitleLabel"));
        activeConnectionsTitleLabel->setGeometry(QRect(2, 10, 131, 17));
        Gui->setCentralWidget(centralwidget);

        retranslateUi(Gui);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QMainWindow *Gui)
    {
        Gui->setWindowTitle(QApplication::translate("Gui", "MainWindow", 0));
        readPayloadDumpEdit->setHtml(QApplication::translate("Gui", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0));
        parseNumericButton->setText(QApplication::translate("Gui", "Parse numeric", 0));
        parseWideStringButton->setText(QApplication::translate("Gui", "Parse wstring", 0));
        clearReadPayloadFieldsButton->setText(QApplication::translate("Gui", "Clear", 0));
        parseStringButton->setText(QApplication::translate("Gui", "Parse string", 0));
        tabWidget->setTabText(tabWidget->indexOf(receiverTab), QApplication::translate("Gui", "Receiver", 0));
        writePayloadDumpEdit->setHtml(QApplication::translate("Gui", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0));
        sendDumpButton->setText(QApplication::translate("Gui", "Send dump", 0));
        sendFieldsButton->setText(QApplication::translate("Gui", "Send", 0));
        clearWritePayloadButton->setText(QApplication::translate("Gui", "Clear", 0));
        dumpButton->setText(QApplication::translate("Gui", "Dump", 0));
        writePayloadFieldsCountLabel->setText(QApplication::translate("Gui", "0", 0));
        writePayloadFieldsTitleLabel->setText(QApplication::translate("Gui", "Fields", 0));
        writePayloadSizeTitleLabel->setText(QApplication::translate("Gui", "Size:", 0));
        writePayloadSizeLabel->setText(QApplication::translate("Gui", "0", 0));
        calculateSizeButton->setText(QApplication::translate("Gui", "Calculate size", 0));
        tabWidget->setTabText(tabWidget->indexOf(senderTab), QApplication::translate("Gui", "Sender", 0));
        removePayloadButton->setText(QApplication::translate("Gui", "Remove payload", 0));
        disconnectButton->setText(QApplication::translate("Gui", "Disconnect", 0));
        activeConnectionsTitleLabel->setText(QApplication::translate("Gui", "Active connections", 0));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ANALYZERWV2554_H
