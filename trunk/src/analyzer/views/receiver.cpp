#include <analyzer/views/receiver.hpp>
#include <analyzer/controller.hpp>

#include <core/common/logging.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

Receiver::Receiver(Ui::Gui &gui, ConnectionsList &connectionsListView):
    gui_(gui),
    connectionsListView_(connectionsListView) {}

void Receiver::initialize(Controller *controller)
{
    payloadParser_.setParent(gui_.scrollAreaWidgetContents);
    controller_ = controller;

    connect(gui_.parseNumericButton, SIGNAL(clicked()), this, SLOT(parseAsNumeric()));
    connect(gui_.parseStringButton, SIGNAL(clicked()), this, SLOT(parseAsString()));
    connect(gui_.parseWideStringButton, SIGNAL(clicked()), this, SLOT(parseAsWideString()));
    connect(gui_.clearReadPayloadFieldsButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(gui_.connectionsView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(loadReadPayload(const QModelIndex&)));
}

void Receiver::loadReadPayload(const QModelIndex &index)
{
    const std::string &connectionId = connectionsListView_.getConnectionId(index);
    const std::string &payloadId = connectionsListView_.getSelectedReadPayloadId();

    if(!connectionId.empty() && !payloadId.empty())
    {
        gui_.tabWidget->setCurrentIndex(0);

        payloadParser_.clear();
        gui_.readPayloadDumpEdit->clear();

        const std::string &dump = controller_->getReadPayloadDump(connectionId, payloadId);
        gui_.readPayloadDumpEdit->insertPlainText(QString::fromStdString(dump));
    }
}

void Receiver::parseAsNumeric()
{
    QString selectedString = gui_.readPayloadDumpEdit->textCursor().selectedText();

    if(selectedString.length() > 0)
    {
        payloadParser_.parseAsNumeric(selectedString.toStdString());
    }
}

void Receiver::parseAsString()
{
    QString selectedString = gui_.readPayloadDumpEdit->textCursor().selectedText();

    if(selectedString.length() > 0)
    {
        payloadParser_.parseAsString(selectedString.toStdString());
    }
}

void Receiver::parseAsWideString()
{
    QString selectedString = gui_.readPayloadDumpEdit->textCursor().selectedText();

    if(selectedString.length() > 0)
    {
        payloadParser_.parseAsWideString(selectedString.toStdString());
    }
}

void Receiver::clear()
{

    payloadParser_.clear();
}

}
}
}
