#pragma once

#include <analyzer/gui/gui.hpp>
#include <analyzer/views/readPayload.hpp>
#include <analyzer/views/writePayload.hpp>
#include <analyzer/standardItemWrapper.hpp>
#include <QtGui/QStandardItemModel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(std::vector<std::string>)

namespace eMU
{
namespace analyzer
{

class Controller;

class View: public QMainWindow
{ Q_OBJECT
public:
    View();

    void setController(Controller *controller);
    void display();
    void displayReadPayload(const std::string &readPayloadDump);

    void emitConnectionAccepted(const std::string &connectionId);
    void emitPayloadReceived(const std::string &connectionId, const std::vector<std::string> &payloadIds);
    void emitConnectionClosed(const std::string &connectionId);

private slots:
    void displayReadPayload(const QModelIndex &index);
    void disconnect();
    void resizeWritePayloadFieldsCount(int numberOfFields);
    void parseNumericField();
    void parseStringField();
    void parseWideStringField();
    void sendDump();
    void sendFields();
    void generateDump();
    void clearReadPayloadView();
    void clearWritePayloadView();
    void calculatehWritePayloadSize();

    void displayConnection(const std::string &connectionId);
    void removeConnectionFromDisplay(const std::string &connectionId);
    void displayConnectionReadPayloads(const std::string &connectionId, const std::vector<std::string> &payloadIds);

signals:
    void connectionAccepted(const std::string &connectionId);
    void payloadReceived(const std::string &connectionId, const std::vector<std::string> &payloadIds);
    void connectionClosed(const std::string &connectionId);

private:
    std::string getSelectedConnectionId() const;

    Ui::Gui gui_;
    QStandardItemModel *connectionsViewItemModel_;
    StandardItemWrapper connectionsViewItem_;
    Controller *controller_;
    views::ReadPayload readPayloadView_;
    views::WritePayload writePayloadView_;
};

}
}
