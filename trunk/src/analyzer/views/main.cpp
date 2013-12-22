#include <analyzer/views/main.hpp>

namespace eMU
{
namespace analyzer
{
namespace views
{

Main::Main():
    connectionsViewItemModel_(new QStandardItemModel()),
    connectionsListView_(gui_, connectionsViewItemModel_->invisibleRootItem()),
    receiverView_(gui_, connectionsListView_),
    senderView_(gui_, connectionsListView_)
{
    gui_.setupUi(this);
    gui_.retranslateUi(this);
    gui_.connectionsView->setModel(connectionsViewItemModel_);
}

void Main::initialize(Controller *controller)
{
    connectionsListView_.initialize(controller);
    receiverView_.initialize(controller);
    senderView_.initialize(controller);
}

ConnectionsList& Main::getConnectionsListView()
{
    return connectionsListView_;
}

void Main::display()
{
    this->show();
}

}
}
}
