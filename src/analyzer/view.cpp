#include <analyzer/view.hpp>
#include <analyzer/controller.hpp>

#include <functional>
#include <glog/logging.h>
#include <boost/lexical_cast.hpp>

namespace eMU
{
namespace analyzer
{

View::View():
    usersViewItemModel_(new QStandardItemModel()),
    usersViewItem_(usersViewItemModel_->invisibleRootItem())
{
    gui_.setupUi(this);
    gui_.retranslateUi(this);
    gui_.usersView->setModel(usersViewItemModel_);
    readStreamParserView_.setParent(gui_.scrollAreaWidgetContents);
}

void View::displayUser(const std::string &userId)
{
    usersViewItem_.insert(userId);
}

void View::removeUserFromDisplay(const std::string &userId)
{
    usersViewItem_.remove(userId);
}

void View::displayUserReadStreams(const std::string &userId, const std::vector<std::string> &streamIds)
{
    try
    {
        StandardItemWrapper userReadStreamsViewItem = usersViewItem_.find(userId);
        userReadStreamsViewItem.clear();

        for(const auto& streamId: streamIds)
        {
            userReadStreamsViewItem.insert(streamId);
        }
    }
    catch(const StandardItemWrapper::ItemNotFoundException&)
    {
        LOG(ERROR) << "Could not find userId: " << userId;
    }
}

void View::setController(Controller *controller)
{
    controller_ = controller;
}

void View::loadReadStream(const QModelIndex &index)
{
    if(index.isValid() && index.parent().isValid())
    {
        StandardItemWrapper userId = usersViewItem_.find(gui_.usersView->currentIndex().parent());
        StandardItemWrapper streamId = userId.find(gui_.usersView->currentIndex());

        controller_->loadReadStream(userId.getText(), streamId.getText());
    }
    else
    {
        LOG(ERROR) << "Could not load stream from selected object, row: " << gui_.usersView->currentIndex().row();
    }
}

void View::display()
{
    connect(gui_.usersView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(loadReadStream(const QModelIndex&)));
    connect(gui_.disconnectUserButton, SIGNAL(clicked()), this, SLOT(disconnectUser()));
    connect(gui_.numberOfFieldsSlider, SIGNAL(valueChanged(int)), this, SLOT(refreshReadStreamFields(int)));
    this->show();
}

void View::displayReadStream(const std::string &streamHex, uint16_t streamId, size_t streamSize)
{
    gui_.streamTextEdit->clear();
    gui_.streamTextEdit->insertPlainText(QString::fromStdString(streamHex));

    std::stringstream streamIdHex; streamIdHex << std::hex << streamId;
    gui_.streamIdLabel->setText(QString::fromStdString(streamIdHex.str()));
    gui_.streamSizeLabel->setText(QString::fromStdString(boost::lexical_cast<std::string>(streamSize)));
}

void View::disconnectUser()
{
    QModelIndex index = gui_.usersView->currentIndex();

    if(index.isValid())
    {
        try
        {
            StandardItemWrapper userId = usersViewItem_.find(index);
            controller_->disconnectUser(userId.getText());
        }
        catch(const StandardItemWrapper::ItemNotFoundException)
        {
            LOG(ERROR) << "Could not find user index: " << index.row();
        }

    }
}

void View::refreshReadStreamFields(int numberOfFields)
{
    gui_.numberOfFieldsLabel->setText(QString::fromStdString(boost::lexical_cast<std::string>(numberOfFields)));
    readStreamParserView_.resize(numberOfFields);
}

}
}
