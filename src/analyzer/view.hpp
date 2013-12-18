#pragma once

#include <analyzer/gui/gui.hpp>
#include <analyzer/views/readPayload.hpp>
#include <analyzer/views/writePayload.hpp>
#include <analyzer/standardItemWrapper.hpp>
#include <QtGui/QStandardItemModel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>

namespace eMU
{
namespace analyzer
{

class Controller;

class View: public QMainWindow
{ Q_OBJECT
public:
    View();

    void displayConnection(const std::string &connectionId);
    void removeConnectionFromDisplay(const std::string &connectionId);
    void displayConnectionReadPayloads(const std::string &connectionId, const std::vector<std::string> &payloadIds);
    void setController(Controller *controller);
    void display();
    void displayReadPayload(const std::string &readPayloadDump);

public slots:
    void displayReadPayload(const QModelIndex &index);
    void disconnect();
    void resizeWritePayloadFieldsCount(int numberOfFields);
    void parseNumericField();
    void parseStringField();
    void sendDump();
    void sendFields();
    void generateDump();
    void clearReadPayloadView();
    void clearWritePayloadView();
    void calculatehWritePayloadSize();

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
