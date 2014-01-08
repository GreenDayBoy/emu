#pragma once

#include <analyzer/gui/gui.hpp>
#include <analyzer/views/connectionsList.hpp>
#include <analyzer/views/receiver.hpp>
#include <analyzer/views/sender.hpp>

namespace eMU
{
namespace analyzer
{

class Controller;

namespace views
{

class Main: public QMainWindow
{
public:
    Main();

    void initialize(Controller *controller);
    ConnectionsList& getConnectionsListView();

    void display();

private:
    Ui::Gui gui_;
    QStandardItemModel *connectionsViewItemModel_;
    ConnectionsList connectionsListView_;
    Receiver receiverView_;
    Sender senderView_;
};

}
}
}
