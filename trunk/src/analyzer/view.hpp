#pragma once

#include <analyzer/user.hpp>
#include <analyzer/gui/gui.hpp>
#include <analyzer/standardItemWrapper.hpp>
#include <QtGui/QStandardItemModel>

namespace eMU
{
namespace analyzer
{

class Controller;

class View
{
public:
    View();

    void displayUser(User &user);
    void removeUserFromDisplay(User &user);
    void displayUserReadStreams(User &user);
    void setController(Controller *controller);

    void show();

private:
    Ui::Gui gui_;
    QMainWindow mainWindow_;
    QStandardItemModel *usersViewItemModel_;
    StandardItemWrapper usersViewItem_;
    Controller *controller_;
};

}
}
