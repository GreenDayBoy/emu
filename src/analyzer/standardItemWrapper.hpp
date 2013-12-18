#pragma once

#include <QtGui/QStandardItem>
#include <core/common/exception.hpp>

namespace eMU
{
namespace analyzer
{

class StandardItemWrapper
{
public:
    class ItemNotFoundException: public core::common::Exception {};

    StandardItemWrapper(QStandardItem *item);

    void insert(const std::string &name);
    void remove(const std::string &name);
    StandardItemWrapper find(const std::string &name) const;
    StandardItemWrapper find(const QModelIndex &index) const;
    void clear();
    std::string getText() const;

private:
    QStandardItem* findChild(const std::string &name) const;
    QStandardItem* findChild(const QModelIndex &index) const;

    QStandardItem *item_;
};

}
}
