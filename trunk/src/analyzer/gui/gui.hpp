/********************************************************************************
** Form generated from reading UI file 'analyzeray2676.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ANALYZERAY2676_H
#define ANALYZERAY2676_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
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
    QLabel *readStreamHexDumpTitleLabel;
    QLabel *readStreamIdTitleLabel;
    QLabel *readStreamIdLabel;
    QLabel *readStreamSizeTitleLabel;
    QLabel *readStreamSizeLabel;
    QTextEdit *readStreamHexDumpEdit;
    QScrollArea *readStreamFieldsFrame;
    QWidget *scrollAreaWidgetContents;
    QPushButton *readStreamParseNumericFieldButton;
    QPushButton *readStreamParseStringFieldButton;
    QWidget *senderTab;
    QSlider *writeStreamFieldsCountSlider;
    QLabel *writeStreamFieldsCountTitleLabel;
    QLabel *writeStreamFieldsCountLabel;
    QScrollArea *writeStreamFieldsFrame;
    QWidget *scrollAreaWidgetContents_2;
    QPushButton *sendWriteStreamFieldsButton;
    QPushButton *getWriteStreamHexDumpButton;
    QPushButton *clearWriteStreamFieldsButton;
    QTextEdit *writeStreamHexDumpEdit;
    QLineEdit *writeStreamIdEdit;
    QLabel *writeStreamIdTitleLabel;
    QPushButton *clearWriteStreamHexDumpButton;
    QPushButton *sendWriteStreamHexDumpButton;
    QTreeView *usersView;
    QPushButton *removeStreamButton;
    QPushButton *disconnectUserButton;

    void setupUi(QMainWindow *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName(QStringLiteral("Gui"));
        Gui->resize(868, 901);
        centralwidget = new QWidget(Gui);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(260, 0, 621, 911));
        receiverTab = new QWidget();
        receiverTab->setObjectName(QStringLiteral("receiverTab"));
        readStreamHexDumpTitleLabel = new QLabel(receiverTab);
        readStreamHexDumpTitleLabel->setObjectName(QStringLiteral("readStreamHexDumpTitleLabel"));
        readStreamHexDumpTitleLabel->setGeometry(QRect(10, 10, 121, 16));
        readStreamIdTitleLabel = new QLabel(receiverTab);
        readStreamIdTitleLabel->setObjectName(QStringLiteral("readStreamIdTitleLabel"));
        readStreamIdTitleLabel->setGeometry(QRect(10, 160, 67, 17));
        readStreamIdLabel = new QLabel(receiverTab);
        readStreamIdLabel->setObjectName(QStringLiteral("readStreamIdLabel"));
        readStreamIdLabel->setGeometry(QRect(90, 160, 67, 17));
        readStreamSizeTitleLabel = new QLabel(receiverTab);
        readStreamSizeTitleLabel->setObjectName(QStringLiteral("readStreamSizeTitleLabel"));
        readStreamSizeTitleLabel->setGeometry(QRect(210, 160, 91, 16));
        readStreamSizeLabel = new QLabel(receiverTab);
        readStreamSizeLabel->setObjectName(QStringLiteral("readStreamSizeLabel"));
        readStreamSizeLabel->setGeometry(QRect(300, 160, 67, 17));
        readStreamHexDumpEdit = new QTextEdit(receiverTab);
        readStreamHexDumpEdit->setObjectName(QStringLiteral("readStreamHexDumpEdit"));
        readStreamHexDumpEdit->setGeometry(QRect(10, 30, 591, 121));
        readStreamHexDumpEdit->setUndoRedoEnabled(false);
        readStreamHexDumpEdit->setReadOnly(true);
        readStreamFieldsFrame = new QScrollArea(receiverTab);
        readStreamFieldsFrame->setObjectName(QStringLiteral("readStreamFieldsFrame"));
        readStreamFieldsFrame->setGeometry(QRect(10, 190, 591, 641));
        readStreamFieldsFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        readStreamFieldsFrame->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 589, 639));
        readStreamFieldsFrame->setWidget(scrollAreaWidgetContents);
        readStreamParseNumericFieldButton = new QPushButton(receiverTab);
        readStreamParseNumericFieldButton->setObjectName(QStringLiteral("readStreamParseNumericFieldButton"));
        readStreamParseNumericFieldButton->setGeometry(QRect(470, 160, 131, 25));
        readStreamParseStringFieldButton = new QPushButton(receiverTab);
        readStreamParseStringFieldButton->setObjectName(QStringLiteral("readStreamParseStringFieldButton"));
        readStreamParseStringFieldButton->setGeometry(QRect(350, 160, 111, 25));
        tabWidget->addTab(receiverTab, QString());
        senderTab = new QWidget();
        senderTab->setObjectName(QStringLiteral("senderTab"));
        writeStreamFieldsCountSlider = new QSlider(senderTab);
        writeStreamFieldsCountSlider->setObjectName(QStringLiteral("writeStreamFieldsCountSlider"));
        writeStreamFieldsCountSlider->setGeometry(QRect(414, 10, 181, 21));
        writeStreamFieldsCountSlider->setMaximum(24);
        writeStreamFieldsCountSlider->setPageStep(1);
        writeStreamFieldsCountSlider->setOrientation(Qt::Horizontal);
        writeStreamFieldsCountTitleLabel = new QLabel(senderTab);
        writeStreamFieldsCountTitleLabel->setObjectName(QStringLiteral("writeStreamFieldsCountTitleLabel"));
        writeStreamFieldsCountTitleLabel->setGeometry(QRect(250, 10, 131, 16));
        writeStreamFieldsCountLabel = new QLabel(senderTab);
        writeStreamFieldsCountLabel->setObjectName(QStringLiteral("writeStreamFieldsCountLabel"));
        writeStreamFieldsCountLabel->setGeometry(QRect(380, 10, 41, 16));
        writeStreamFieldsFrame = new QScrollArea(senderTab);
        writeStreamFieldsFrame->setObjectName(QStringLiteral("writeStreamFieldsFrame"));
        writeStreamFieldsFrame->setGeometry(QRect(10, 170, 591, 701));
        writeStreamFieldsFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        writeStreamFieldsFrame->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 589, 699));
        sendWriteStreamFieldsButton = new QPushButton(scrollAreaWidgetContents_2);
        sendWriteStreamFieldsButton->setObjectName(QStringLiteral("sendWriteStreamFieldsButton"));
        sendWriteStreamFieldsButton->setGeometry(QRect(470, 677, 99, 20));
        getWriteStreamHexDumpButton = new QPushButton(scrollAreaWidgetContents_2);
        getWriteStreamHexDumpButton->setObjectName(QStringLiteral("getWriteStreamHexDumpButton"));
        getWriteStreamHexDumpButton->setGeometry(QRect(360, 677, 99, 20));
        clearWriteStreamFieldsButton = new QPushButton(scrollAreaWidgetContents_2);
        clearWriteStreamFieldsButton->setObjectName(QStringLiteral("clearWriteStreamFieldsButton"));
        clearWriteStreamFieldsButton->setGeometry(QRect(250, 677, 99, 20));
        writeStreamFieldsFrame->setWidget(scrollAreaWidgetContents_2);
        writeStreamHexDumpEdit = new QTextEdit(senderTab);
        writeStreamHexDumpEdit->setObjectName(QStringLiteral("writeStreamHexDumpEdit"));
        writeStreamHexDumpEdit->setGeometry(QRect(10, 40, 591, 102));
        writeStreamHexDumpEdit->setUndoRedoEnabled(false);
        writeStreamHexDumpEdit->setReadOnly(false);
        writeStreamIdEdit = new QLineEdit(senderTab);
        writeStreamIdEdit->setObjectName(QStringLiteral("writeStreamIdEdit"));
        writeStreamIdEdit->setGeometry(QRect(93, 7, 113, 23));
        writeStreamIdTitleLabel = new QLabel(senderTab);
        writeStreamIdTitleLabel->setObjectName(QStringLiteral("writeStreamIdTitleLabel"));
        writeStreamIdTitleLabel->setGeometry(QRect(15, 10, 67, 17));
        clearWriteStreamHexDumpButton = new QPushButton(senderTab);
        clearWriteStreamHexDumpButton->setObjectName(QStringLiteral("clearWriteStreamHexDumpButton"));
        clearWriteStreamHexDumpButton->setGeometry(QRect(390, 146, 99, 20));
        sendWriteStreamHexDumpButton = new QPushButton(senderTab);
        sendWriteStreamHexDumpButton->setObjectName(QStringLiteral("sendWriteStreamHexDumpButton"));
        sendWriteStreamHexDumpButton->setGeometry(QRect(500, 146, 99, 20));
        tabWidget->addTab(senderTab, QString());
        usersView = new QTreeView(centralwidget);
        usersView->setObjectName(QStringLiteral("usersView"));
        usersView->setGeometry(QRect(-5, 0, 266, 791));
        usersView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        removeStreamButton = new QPushButton(centralwidget);
        removeStreamButton->setObjectName(QStringLiteral("removeStreamButton"));
        removeStreamButton->setGeometry(QRect(50, 830, 121, 27));
        disconnectUserButton = new QPushButton(centralwidget);
        disconnectUserButton->setObjectName(QStringLiteral("disconnectUserButton"));
        disconnectUserButton->setGeometry(QRect(50, 800, 121, 27));
        Gui->setCentralWidget(centralwidget);

        retranslateUi(Gui);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Gui);
    } // setupUi

    void retranslateUi(QMainWindow *Gui)
    {
        Gui->setWindowTitle(QApplication::translate("Gui", "MainWindow", 0));
        readStreamHexDumpTitleLabel->setText(QApplication::translate("Gui", "Hex dump", 0));
        readStreamIdTitleLabel->setText(QApplication::translate("Gui", "Stream id:", 0));
        readStreamIdLabel->setText(QApplication::translate("Gui", "0", 0));
        readStreamSizeTitleLabel->setText(QApplication::translate("Gui", "Stream size:", 0));
        readStreamSizeLabel->setText(QApplication::translate("Gui", "0", 0));
        readStreamHexDumpEdit->setHtml(QApplication::translate("Gui", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0));
        readStreamParseNumericFieldButton->setText(QApplication::translate("Gui", "Parse numeric", 0));
        readStreamParseStringFieldButton->setText(QApplication::translate("Gui", "Parse string", 0));
        tabWidget->setTabText(tabWidget->indexOf(receiverTab), QApplication::translate("Gui", "Receiver", 0));
        writeStreamFieldsCountTitleLabel->setText(QApplication::translate("Gui", "Number of fields:", 0));
        writeStreamFieldsCountLabel->setText(QApplication::translate("Gui", "0", 0));
        sendWriteStreamFieldsButton->setText(QApplication::translate("Gui", "Send", 0));
        getWriteStreamHexDumpButton->setText(QApplication::translate("Gui", "Hex dump", 0));
        clearWriteStreamFieldsButton->setText(QApplication::translate("Gui", "Clear", 0));
        writeStreamHexDumpEdit->setHtml(QApplication::translate("Gui", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0));
        writeStreamIdEdit->setText(QApplication::translate("Gui", "0", 0));
        writeStreamIdTitleLabel->setText(QApplication::translate("Gui", "Stream id:", 0));
        clearWriteStreamHexDumpButton->setText(QApplication::translate("Gui", "Clear", 0));
        sendWriteStreamHexDumpButton->setText(QApplication::translate("Gui", "Send", 0));
        tabWidget->setTabText(tabWidget->indexOf(senderTab), QApplication::translate("Gui", "Sender", 0));
        removeStreamButton->setText(QApplication::translate("Gui", "Remove stream", 0));
        disconnectUserButton->setText(QApplication::translate("Gui", "Disconnect user", 0));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ANALYZERAY2676_H
