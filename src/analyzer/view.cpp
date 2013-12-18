#include <analyzer/view.hpp>
#include <analyzer/controller.hpp>

#include <functional>
#include <glog/logging.h>
#include <boost/lexical_cast.hpp>
#include <iomanip>

namespace eMU
{
namespace analyzer
{

View::View():
    connectionsViewItemModel_(new QStandardItemModel()),
    connectionsViewItem_(connectionsViewItemModel_->invisibleRootItem())
{
    gui_.setupUi(this);
    gui_.retranslateUi(this);
    gui_.connectionsView->setModel(connectionsViewItemModel_);

    readPayloadView_.setParent(gui_.scrollAreaWidgetContents);
    writePayloadView_.setParent(gui_.scrollAreaWidgetContents_2);
}

void View::displayConnection(const std::string &connectionId)
{
    connectionsViewItem_.insert(connectionId);
}

void View::removeConnectionFromDisplay(const std::string &connectionId)
{
    connectionsViewItem_.remove(connectionId);
}

void View::displayConnectionReadPayloads(const std::string &connectionId, const std::vector<std::string> &payloadIds)
{
    try
    {
        StandardItemWrapper readPayloadsViewItem = connectionsViewItem_.find(connectionId);
        readPayloadsViewItem.clear();

        for(const auto& payloadId: payloadIds)
        {
            readPayloadsViewItem.insert(payloadId);
        }
    }
    catch(const StandardItemWrapper::ItemNotFoundException&)
    {
        LOG(ERROR) << "Could not find connectionId: " << connectionId;
    }
}

void View::setController(Controller *controller)
{
    controller_ = controller;
}

void View::displayReadPayload(const QModelIndex &index)
{
    if(index.isValid() && index.parent().isValid())
    {
        StandardItemWrapper connectionId = connectionsViewItem_.find(gui_.connectionsView->currentIndex().parent());
        StandardItemWrapper payloadId = connectionId.find(gui_.connectionsView->currentIndex());

        const std::string &dump = controller_->getReadPayloadDump(connectionId.getText(), payloadId.getText());

        readPayloadView_.clear();
        gui_.readPayloadDumpEdit->clear();
        gui_.readPayloadDumpEdit->insertPlainText(QString::fromStdString(dump));

        gui_.tabWidget->setCurrentIndex(0);
    }
    else
    {
        LOG(ERROR) << "Could not get dump for selected payload!";
    }
}

void View::display()
{
    connect(gui_.connectionsView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(displayReadPayload(const QModelIndex&)));
    connect(gui_.disconnectButton, SIGNAL(clicked()), this, SLOT(disconnect()));

    connect(gui_.parseNumericButton, SIGNAL(clicked()), this, SLOT(parseNumericField()));
    connect(gui_.parseStringButton, SIGNAL(clicked()), this, SLOT(parseStringField()));
    connect(gui_.parseWideStringButton, SIGNAL(clicked()), this, SLOT(parseWideStringField()));
    connect(gui_.clearReadPayloadFieldsButton, SIGNAL(clicked()), this, SLOT(clearReadPayloadView()));

    connect(gui_.writePayloadFieldsCountSlider, SIGNAL(valueChanged(int)), this, SLOT(resizeWritePayloadFieldsCount(int)));
    connect(gui_.sendFieldsButton, SIGNAL(clicked()), this, SLOT(sendFields()));
    connect(gui_.sendDumpButton, SIGNAL(clicked()), this, SLOT(sendDump()));
    connect(gui_.dumpButton, SIGNAL(clicked()), this, SLOT(generateDump()));
    connect(gui_.clearWritePayloadButton, SIGNAL(clicked()), this, SLOT(clearWritePayloadView()));
    connect(gui_.calculateSizeButton, SIGNAL(clicked()), this, SLOT(calculatehWritePayloadSize()));

    this->show();
}

void View::disconnect()
{
    const std::string &connectionId = getSelectedConnectionId();

    if(!connectionId.empty())
    {
        controller_->disconnect(connectionId);
    }
}

void View::resizeWritePayloadFieldsCount(int numberOfFields)
{
    gui_.writePayloadFieldsCountLabel->setText(QString::fromStdString(boost::lexical_cast<std::string>(numberOfFields)));
    writePayloadView_.resize(numberOfFields);
}

void View::parseNumericField()
{
    QString selectedString = gui_.readPayloadDumpEdit->textCursor().selectedText();

    if(selectedString.length() > 0)
    {
        readPayloadView_.insertNumericField(selectedString.toStdString());
    }
}

void View::parseStringField()
{
    QString selectedString = gui_.readPayloadDumpEdit->textCursor().selectedText();

    if(selectedString.length() > 0)
    {
        readPayloadView_.insertStringField(selectedString.toStdString());
    }
}

void View::parseWideStringField()
{
    QString selectedString = gui_.readPayloadDumpEdit->textCursor().selectedText();

    if(selectedString.length() > 0)
    {
        readPayloadView_.insertWideStringField(selectedString.toStdString());
    }
}

void View::sendFields()
{
    const std::string &connectionId = this->getSelectedConnectionId();

    if(!connectionId.empty())
    {
        const std::string &dump = writePayloadView_.getDump();
        controller_->send(connectionId, dump);
    }
}

void View::generateDump()
{
    gui_.writePayloadDumpEdit->clear();
    gui_.writePayloadDumpEdit->insertPlainText(QString::fromStdString(writePayloadView_.getDump()));
}

void View::clearWritePayloadView()
{
    gui_.writePayloadDumpEdit->clear();
    gui_.writePayloadFieldsCountSlider->setValue(0);
    gui_.writePayloadFieldsCountLabel->setText("0");
    writePayloadView_.clear();
}

void View::clearReadPayloadView()
{
    readPayloadView_.clear();
}

void View::sendDump()
{
    const std::string &connectionId = this->getSelectedConnectionId();

    if(!connectionId.empty())
    {
        controller_->send(connectionId, gui_.writePayloadDumpEdit->toPlainText().toStdString());
    }
}

std::string View::getSelectedConnectionId() const
{
    try
    {
        StandardItemWrapper connectionId = connectionsViewItem_.find(gui_.connectionsView->currentIndex());
        return connectionId.getText();
    }
    catch(const StandardItemWrapper::ItemNotFoundException&)
    {
        LOG(ERROR) << "Selected object is not a connection!";
    }

    return "";
}

void View::calculatehWritePayloadSize()
{
    gui_.writePayloadSizeLabel->setText(QString::fromStdString(boost::lexical_cast<std::string>(writePayloadView_.getFieldsSize())));
}

}
}
