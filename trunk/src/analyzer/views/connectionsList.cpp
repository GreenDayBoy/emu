#include <analyzer/views/connectionsList.hpp>
#include <analyzer/controller.hpp>

#include <glog/logging.h>

namespace eMU
{
namespace analyzer
{
namespace views
{

ConnectionsList::ConnectionsList(Ui::Gui &gui, QStandardItem *connectionsViewRootItem):
    gui_(gui),
    connectionsViewItem_(connectionsViewRootItem)
{
    qRegisterMetaType<std::string>("std::string");
}

void ConnectionsList::emitConnectionAcceptedSignal(const std::string &connectionId)
{
    emit onConnectionAccept(connectionId);
}

void ConnectionsList::emitPayloadReceivedSignal(const std::string &connectionId, const std::string &payloadId)
{
    emit onPayloadReceive(connectionId, payloadId);
}

void ConnectionsList::emitConnectionClosedSignal(const std::string &connectionId)
{
    emit onConnectionClose(connectionId);
}

std::string ConnectionsList::getConnectionId(const QModelIndex &index) const
{
    if(index.isValid() && index.parent().isValid())
    {
        StandardItemWrapper connectionItem = connectionsViewItem_.find(gui_.connectionsView->currentIndex().parent());
        return connectionItem.getText();
    }
    else
    {
        LOG(ERROR) << "selected object has not valid parent!";
    }

    return "";
}

std::string ConnectionsList::getSelectedConnectionId() const
{
    try
    {
        StandardItemWrapper connectionItem = connectionsViewItem_.find(gui_.connectionsView->currentIndex());
        return connectionItem.getText();
    }
    catch(const StandardItemWrapper::ItemNotFoundException&)
    {
        LOG(ERROR) << "Selected object is not a connection!";
    }

    return "";
}

std::string ConnectionsList::getSelectedReadPayloadId() const
{
    const QModelIndex &index = gui_.connectionsView->currentIndex();

    if(index.isValid() && index.parent().isValid())
    {
        StandardItemWrapper connectionItem = connectionsViewItem_.find(index.parent());
        StandardItemWrapper payloadItem = connectionItem.find(index);

        return payloadItem.getText();
    }
    else
    {
        LOG(ERROR) << "selected object is not a payload!";
    }

    return "";
}

void ConnectionsList::initialize(Controller *controller)
{
    controller_ = controller;

    connect(this, SIGNAL(onConnectionAccept(const std::string&)), this, SLOT(insertConnectionId(const std::string&)));
    connect(this, SIGNAL(onPayloadReceive(const std::string&, const std::string&)), this, SLOT(insertReadPayloadId(const std::string&, const std::string&)));
    connect(this, SIGNAL(onConnectionClose(const std::string&)), this, SLOT(removeConnectionId(const std::string&)));
    connect(gui_.disconnectButton, SIGNAL(clicked()), this, SLOT(disconnect()));
}

void ConnectionsList::insertConnectionId(const std::string &connectionId)
{
    connectionsViewItem_.insert(connectionId);
}

void ConnectionsList::insertReadPayloadId(const std::string &connectionId, const std::string &payloadId)
{
    try
    {
        StandardItemWrapper readPayloadsViewItem = connectionsViewItem_.find(connectionId);
        readPayloadsViewItem.insert(payloadId);
    }
    catch(const StandardItemWrapper::ItemNotFoundException&)
    {
        LOG(ERROR) << "Could not find connectionId: " << connectionId;
    }
}

void ConnectionsList::removeConnectionId(const std::string &connectionId)
{
     connectionsViewItem_.remove(connectionId);
}

void ConnectionsList::disconnect()
{
    const std::string &connectionId = this->getSelectedConnectionId();

    if(!connectionId.empty())
    {
        controller_->disconnect(connectionId);
    }
}

}
}
}
