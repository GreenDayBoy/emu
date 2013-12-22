#pragma once

#include <analyzer/gui/gui.hpp>
#include <analyzer/views/connectionsList.hpp>
#include <analyzer/payload/parser.hpp>

namespace eMU
{
namespace analyzer
{

class Controller;

namespace views
{

class Receiver: public QObject
{ Q_OBJECT
public:
    Receiver(Ui::Gui &gui, ConnectionsList &connectionsListView);

    void initialize(Controller *controller);

private slots:
    void loadReadPayload(const QModelIndex &index);
    void parseAsNumeric();
    void parseAsString();
    void parseAsWideString();
    void clear();

private:
    Ui::Gui &gui_;
    ConnectionsList &connectionsListView_;
    Controller *controller_;
    payload::Parser payloadParser_;
};

}
}
}
