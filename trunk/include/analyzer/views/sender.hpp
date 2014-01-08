#pragma once

#include <analyzer/gui/gui.hpp>
#include <analyzer/views/connectionsList.hpp>
#include <analyzer/payload/builder.hpp>

namespace eMU
{
namespace analyzer
{

class Controller;

namespace views
{

class Sender: public QObject
{ Q_OBJECT
public:
    Sender(Ui::Gui &gui, ConnectionsList &connectionsListView);

    void initialize(Controller *controller);

private slots:
    void sendDump();
    void sendFields();
    void resizeFieldsCount(int numberOfFields);
    void calculateSize();
    void generateDump();
    void clear();

private:
    Ui::Gui &gui_;
    ConnectionsList &connectionsListView_;
    Controller *controller_;
    payload::Builder payloadBuilder_;
};

}
}
}
