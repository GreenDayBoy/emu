#include <analyzer/views/sender.hpp>
#include <analyzer/controller.hpp>

#include <core/common/logging.hpp>
#include <boost/lexical_cast.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

Sender::Sender(Ui::Gui &gui, ConnectionsList &connectionsListView):
    gui_(gui),
    connectionsListView_(connectionsListView) {}

void Sender::initialize(Controller *controller)
{
    payloadBuilder_.setParent(gui_.scrollAreaWidgetContents_2);
    controller_ = controller;

    connect(gui_.writePayloadFieldsCountSlider, SIGNAL(valueChanged(int)), this, SLOT(resizeFieldsCount(int)));
    connect(gui_.sendFieldsButton, SIGNAL(clicked()), this, SLOT(sendFields()));
    connect(gui_.sendDumpButton, SIGNAL(clicked()), this, SLOT(sendDump()));
    connect(gui_.dumpButton, SIGNAL(clicked()), this, SLOT(generateDump()));
    connect(gui_.clearWritePayloadButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(gui_.calculateSizeButton, SIGNAL(clicked()), this, SLOT(calculateSize()));
}

void Sender::sendDump()
{
    const std::string &connectionId = connectionsListView_.getSelectedConnectionId();

    if(!connectionId.empty())
    {
        controller_->send(connectionId, gui_.writePayloadDumpEdit->toPlainText().toStdString());
    }
}

void Sender::sendFields()
{
    const std::string &connectionId = connectionsListView_.getSelectedConnectionId();

    if(!connectionId.empty())
    {
        const std::string &dump = payloadBuilder_.getDump();
        controller_->send(connectionId, dump);
    }
}

void Sender::resizeFieldsCount(int numberOfFields)
{
    gui_.writePayloadFieldsCountLabel->setText(QString::fromStdString(boost::lexical_cast<std::string>(numberOfFields)));
    payloadBuilder_.resize(numberOfFields);
}

void Sender::calculateSize()
{
    gui_.writePayloadSizeLabel->setText(QString::fromStdString(boost::lexical_cast<std::string>(payloadBuilder_.getFieldsSize())));
}

void Sender::generateDump()
{
    gui_.writePayloadDumpEdit->clear();
    gui_.writePayloadDumpEdit->insertPlainText(QString::fromStdString(payloadBuilder_.getDump()));
}

void Sender::clear()
{
    gui_.writePayloadDumpEdit->clear();
    gui_.writePayloadFieldsCountSlider->setValue(0);
    gui_.writePayloadFieldsCountLabel->setText("0");
    gui_.writePayloadSizeLabel->setText("0");
    payloadBuilder_.clear();
}

}
}
}
