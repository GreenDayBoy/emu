/********************************************************************************
** Form generated from reading UI file 'analyzerxJ9899.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ANALYZERXJ9899_H
#define ANALYZERXJ9899_H

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
    QLabel *streamHexDumpTitleLabel;
    QLabel *streamIdTitleLabel;
    QLabel *streamIdLabel;
    QLabel *streamSizeTitleLabel;
    QLabel *streamSizeLabel;
    QTextEdit *streamTextEdit;
    QLabel *streamParserTitleLabel;
    QScrollArea *parserFrame;
    QWidget *scrollAreaWidgetContents;
    QLabel *numberOfFieldsLabel;
    QSlider *numberOfFieldsSlider;
    QLabel *numberOfFieldsTitleLabel;
    QWidget *senderTab;
    QTreeView *usersView;
    QPushButton *removeStreamButton;
    QPushButton *disconnectUserButton;

    void setupUi(QMainWindow *Gui)
    {
        if (Gui->objectName().isEmpty())
            Gui->setObjectName(QStringLiteral("Gui"));
        Gui->resize(868, 870);
        centralwidget = new QWidget(Gui);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(260, 0, 621, 871));
        receiverTab = new QWidget();
        receiverTab->setObjectName(QStringLiteral("receiverTab"));
        streamHexDumpTitleLabel = new QLabel(receiverTab);
        streamHexDumpTitleLabel->setObjectName(QStringLiteral("streamHexDumpTitleLabel"));
        streamHexDumpTitleLabel->setGeometry(QRect(10, 10, 121, 16));
        streamIdTitleLabel = new QLabel(receiverTab);
        streamIdTitleLabel->setObjectName(QStringLiteral("streamIdTitleLabel"));
        streamIdTitleLabel->setGeometry(QRect(10, 100, 67, 17));
        streamIdLabel = new QLabel(receiverTab);
        streamIdLabel->setObjectName(QStringLiteral("streamIdLabel"));
        streamIdLabel->setGeometry(QRect(90, 100, 67, 17));
        streamSizeTitleLabel = new QLabel(receiverTab);
        streamSizeTitleLabel->setObjectName(QStringLiteral("streamSizeTitleLabel"));
        streamSizeTitleLabel->setGeometry(QRect(210, 100, 91, 16));
        streamSizeLabel = new QLabel(receiverTab);
        streamSizeLabel->setObjectName(QStringLiteral("streamSizeLabel"));
        streamSizeLabel->setGeometry(QRect(300, 100, 67, 17));
        streamTextEdit = new QTextEdit(receiverTab);
        streamTextEdit->setObjectName(QStringLiteral("streamTextEdit"));
        streamTextEdit->setGeometry(QRect(10, 30, 591, 61));
        streamTextEdit->setUndoRedoEnabled(false);
        streamTextEdit->setReadOnly(true);
        streamParserTitleLabel = new QLabel(receiverTab);
        streamParserTitleLabel->setObjectName(QStringLiteral("streamParserTitleLabel"));
        streamParserTitleLabel->setGeometry(QRect(20, 140, 101, 16));
        parserFrame = new QScrollArea(receiverTab);
        parserFrame->setObjectName(QStringLiteral("parserFrame"));
        parserFrame->setGeometry(QRect(10, 130, 591, 691));
        parserFrame->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        parserFrame->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 576, 689));
        numberOfFieldsLabel = new QLabel(scrollAreaWidgetContents);
        numberOfFieldsLabel->setObjectName(QStringLiteral("numberOfFieldsLabel"));
        numberOfFieldsLabel->setGeometry(QRect(170, 10, 41, 16));
        numberOfFieldsSlider = new QSlider(scrollAreaWidgetContents);
        numberOfFieldsSlider->setObjectName(QStringLiteral("numberOfFieldsSlider"));
        numberOfFieldsSlider->setGeometry(QRect(220, 10, 160, 21));
        numberOfFieldsSlider->setOrientation(Qt::Horizontal);
        numberOfFieldsTitleLabel = new QLabel(scrollAreaWidgetContents);
        numberOfFieldsTitleLabel->setObjectName(QStringLiteral("numberOfFieldsTitleLabel"));
        numberOfFieldsTitleLabel->setGeometry(QRect(40, 10, 131, 16));
        parserFrame->setWidget(scrollAreaWidgetContents);
        tabWidget->addTab(receiverTab, QString());
        senderTab = new QWidget();
        senderTab->setObjectName(QStringLiteral("senderTab"));
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
        streamHexDumpTitleLabel->setText(QApplication::translate("Gui", "Stream Hex dump", 0));
        streamIdTitleLabel->setText(QApplication::translate("Gui", "Stream id:", 0));
        streamIdLabel->setText(QString());
        streamSizeTitleLabel->setText(QApplication::translate("Gui", "Stream size:", 0));
        streamSizeLabel->setText(QString());
        streamTextEdit->setHtml(QApplication::translate("Gui", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0));
        streamParserTitleLabel->setText(QApplication::translate("Gui", "Stream parser", 0));
        numberOfFieldsLabel->setText(QApplication::translate("Gui", "0", 0));
        numberOfFieldsTitleLabel->setText(QApplication::translate("Gui", "Number of fields:", 0));
        tabWidget->setTabText(tabWidget->indexOf(receiverTab), QApplication::translate("Gui", "Receiver", 0));
        tabWidget->setTabText(tabWidget->indexOf(senderTab), QApplication::translate("Gui", "Sender", 0));
        removeStreamButton->setText(QApplication::translate("Gui", "Remove stream", 0));
        disconnectUserButton->setText(QApplication::translate("Gui", "Disconnect user", 0));
    } // retranslateUi

};

namespace Ui {
    class Gui: public Ui_Gui {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ANALYZERXJ9899_H
