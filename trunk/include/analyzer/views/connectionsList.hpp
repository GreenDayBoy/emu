#pragma once

#include <analyzer/gui/gui.hpp>
#include <analyzer/standardItemWrapper.hpp>

Q_DECLARE_METATYPE(std::string)

namespace eMU
{
namespace analyzer
{

class Controller;

namespace views
{

class ConnectionsList: public QObject
{ Q_OBJECT
public:
    ConnectionsList(Ui::Gui &gui, QStandardItem *connectionsViewRootItem);

    void emitConnectionAcceptedSignal(const std::string &connectionId);
    void emitPayloadReceivedSignal(const std::string &connectionId, const std::string &payloadId);
    void emitConnectionClosedSignal(const std::string &connectionId);

    std::string getConnectionId(const QModelIndex &index) const;
    std::string getSelectedConnectionId() const;
    std::string getSelectedReadPayloadId() const;

    void initialize(Controller *controller);

signals:
    void onConnectionAccept(const std::string &connectionId);
    void onPayloadReceive(const std::string &connectionId, const std::string &payloadIds);
    void onConnectionClose(const std::string &connectionId);

private slots:
    void insertConnectionId(const std::string &connectionId);
    void insertReadPayloadId(const std::string &connectionId, const std::string &payloadId);
    void removeConnectionId(const std::string &connectionId);
    void disconnect();

private:
    Ui::Gui &gui_;
    Controller *controller_;
    StandardItemWrapper connectionsViewItem_;
};

}
}
}
