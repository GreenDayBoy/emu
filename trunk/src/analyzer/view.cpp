#include <analyzer/view.hpp>
#include <boost/lexical_cast.hpp>
#include <functional>

#include <glog/logging.h>

namespace eMU
{
namespace analyzer
{

View::View():
    usersViewItemModel_(new QStandardItemModel()),
    usersViewItem_(usersViewItemModel_->invisibleRootItem())
{
    gui_.setupUi(&mainWindow_);
    gui_.retranslateUi(&mainWindow_);
    gui_.usersView->setModel(usersViewItemModel_);
}

void View::displayUser(User &user)
{
    usersViewItem_.insert(boost::lexical_cast<std::string>(user.getHash()));
}

void View::removeUserFromDisplay(User &user)
{
    usersViewItem_.remove(boost::lexical_cast<std::string>(user.getHash()));
}

void View::displayUserReadStreams(User &user)
{
    try
    {
        StandardItemWrapper userReadStreamsViewItem = usersViewItem_.find(boost::lexical_cast<std::string>(user.getHash()));
        userReadStreamsViewItem.clear();

        for(const auto& stream: user.getReadStreams())
        {
            userReadStreamsViewItem.insert(boost::lexical_cast<std::string>(reinterpret_cast<size_t>(&stream)));
        }
    }
    catch(const StandardItemWrapper::ItemNotFoundException&)
    {
        LOG(ERROR) << "Could not found user by hash: " << user.getHash();
    }
}

void View::setController(Controller *controller)
{
    controller_ = controller;
}

void View::show()
{
    mainWindow_.show();
}

}
}
