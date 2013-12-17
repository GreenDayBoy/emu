#include <analyzer/view.hpp>
#include <analyzer/controller.hpp>

#include <functional>
#include <glog/logging.h>
#include <boost/lexical_cast.hpp>
#include <iomanip>

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
    readStreamView_.setParent(gui_.scrollAreaWidgetContents);
    writeStreamView_.setParent(gui_.scrollAreaWidgetContents_2);
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
    connect(gui_.writeStreamFieldsCountSlider, SIGNAL(valueChanged(int)), this, SLOT(resizeWriteStreamFieldsCount(int)));
    connect(gui_.readStreamParseNumericFieldButton, SIGNAL(clicked()), this, SLOT(parseReadStreamNumericField()));
    connect(gui_.readStreamParseStringFieldButton, SIGNAL(clicked()), this, SLOT(parseReadStreamStringField()));
    connect(gui_.sendWriteStreamFieldsButton, SIGNAL(clicked()), this, SLOT(sendFields()));
    connect(gui_.getWriteStreamHexDumpButton, SIGNAL(clicked()), this, SLOT(getWriteStreamHexDump()));
    connect(gui_.clearWriteStreamFieldsButton, SIGNAL(clicked()), this, SLOT(clearWriteStreamView()));
    connect(gui_.clearWriteStreamHexDumpButton, SIGNAL(clicked()), this, SLOT(clearWriteStreamHexDump()));
    connect(gui_.sendWriteStreamHexDumpButton, SIGNAL(clicked()), this, SLOT(sendHexDump()));
    this->show();
}

void View::displayReadStream(const std::string &streamHex, uint16_t streamId, size_t streamSize)
{
    readStreamView_.clear();

    gui_.readStreamHexDumpEdit->clear();
    gui_.readStreamHexDumpEdit->insertPlainText(QString::fromStdString(streamHex));

    std::stringstream streamIdHex; streamIdHex << std::hex << streamId;
    gui_.readStreamIdLabel->setText(QString::fromStdString(streamIdHex.str()));
    gui_.readStreamSizeLabel->setText(QString::fromStdString(boost::lexical_cast<std::string>(streamSize)));
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

void View::resizeWriteStreamFieldsCount(int numberOfFields)
{
    gui_.writeStreamFieldsCountLabel->setText(QString::fromStdString(boost::lexical_cast<std::string>(numberOfFields)));
    writeStreamView_.resize(numberOfFields);
}

void View::parseReadStreamNumericField()
{
    QString selectedString = gui_.readStreamHexDumpEdit->textCursor().selectedText();

    if(selectedString.length() > 0)
    {
        readStreamView_.insertNumericField(selectedString.toStdString());
    }
}

void View::parseReadStreamStringField()
{
    QString selectedString = gui_.readStreamHexDumpEdit->textCursor().selectedText();

    if(selectedString.length() > 0)
    {
        readStreamView_.insertStringField(selectedString.toStdString());
    }
}

void View::sendFields()
{
    if(gui_.writeStreamIdEdit->text().length() == 0)
    {
        LOG(ERROR) << "stream id not set!";
        return;
    }

    QModelIndex index = gui_.usersView->currentIndex();

    if(index.isValid())
    {
        try
        {
            StandardItemWrapper userId = usersViewItem_.find(index);
            controller_->send(userId.getText(), gui_.writeStreamIdEdit->text().toStdString(), writeStreamView_.getFields());
        }
        catch(const StandardItemWrapper::ItemNotFoundException)
        {
            LOG(ERROR) << "Could not find user index: " << index.row();
        }
    }
}

void View::getWriteStreamHexDump()
{
    if(gui_.writeStreamIdEdit->text().length() == 0)
    {
        LOG(ERROR) << "stream id not set!";
        return;
    }

    clearWriteStreamHexDump();

    const std::string preview = controller_->generateStreamPreview(gui_.writeStreamIdEdit->text().toStdString(), writeStreamView_.getFields());
    gui_.writeStreamHexDumpEdit->insertPlainText(QString::fromStdString(preview));
}

void View::clearWriteStreamHexDump()
{
    gui_.writeStreamHexDumpEdit->clear();
}

void View::clearWriteStreamView()
{
    gui_.writeStreamFieldsCountSlider->setValue(0);
    writeStreamView_.clear();
}

void View::sendHexDump()
{
    const std::string &dump = gui_.writeStreamHexDumpEdit->toPlainText().toStdString();

    if(dump.length() == 0)
    {
        LOG(ERROR) << "empty hex dump!";
        return;
    }

    QModelIndex index = gui_.usersView->currentIndex();

    if(index.isValid())
    {
        try
        {
            StandardItemWrapper userId = usersViewItem_.find(index);
            controller_->send(userId.getText(), dump);
        }
        catch(const StandardItemWrapper::ItemNotFoundException)
        {
            LOG(ERROR) << "Could not find user index: " << index.row();
        }
    }
}

}
}
