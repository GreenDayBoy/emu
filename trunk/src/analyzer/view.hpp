#pragma once

#include <analyzer/gui/gui.hpp>
#include <analyzer/stream/views/readStreamView.hpp>
#include <analyzer/stream/views/writeStreamView.hpp>
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

    void displayUser(const std::string &userId);
    void removeUserFromDisplay(const std::string &userId);
    void displayUserReadStreams(const std::string &userId, const std::vector<std::string> &streamIds);
    void setController(Controller *controller);
    void display();
    void displayReadStream(const std::string &streamHex, uint16_t streamId, size_t streamSize);

public slots:
    void loadReadStream(const QModelIndex &index);
    void disconnectUser();
    void resizeWriteStreamFieldsCount(int numberOfFields);
    void parseReadStreamNumericField();
    void parseReadStreamStringField();
    void sendFields();
    void sendHexDump();
    void getWriteStreamHexDump();
    void clearWriteStreamHexDump();
    void clearWriteStreamView();

private:
    Ui::Gui gui_;
    QStandardItemModel *usersViewItemModel_;
    StandardItemWrapper usersViewItem_;
    Controller *controller_;
    stream::views::ReadStreamView readStreamView_;
    stream::views::WriteStreamView writeStreamView_;
};

}
}
