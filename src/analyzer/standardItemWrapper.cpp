#include <analyzer/standardItemWrapper.hpp>

namespace eMU
{
namespace analyzer
{

StandardItemWrapper::StandardItemWrapper(QStandardItem *item):
    item_(item) {}

void StandardItemWrapper::insert(const std::string &name)
{
    int32_t rowId = item_->rowCount();

    QStandardItem *row = new QStandardItem(QString::fromStdString(name));
    item_->insertRow(rowId, row);
}

void StandardItemWrapper::remove(const std::string &name)
{
    QStandardItem *child = this->findChild(name);

    if(child != nullptr)
    {
        item_->removeRow(child->row());
    }
}

StandardItemWrapper StandardItemWrapper::find(const std::string &name)
{
    QStandardItem *child = this->findChild(name);

    if(child != nullptr)
    {
        return StandardItemWrapper(child);
    }
    else
    {
        throw ItemNotFoundException();
    }
}

QStandardItem* StandardItemWrapper::findChild(const std::string &name)
{
    for(uint32_t i = 0; i < item_->rowCount(); ++i)
    {
        if(item_->child(i)->text() == QString::fromStdString(name))
        {
            return item_->child(i);
        }
    }

    return nullptr;
}

StandardItemWrapper StandardItemWrapper::find(const QModelIndex &index)
{
    QStandardItem *child = this->findChild(index);

    if(child != nullptr)
    {
        return StandardItemWrapper(child);
    }
    else
    {
        throw ItemNotFoundException();
    }
}

QStandardItem* StandardItemWrapper::findChild(const QModelIndex &index)
{
    for(uint32_t i = 0; i < item_->rowCount(); ++i)
    {
        if(item_->child(i)->index() == index)
        {
            return item_->child(i);
        }
    }

    return nullptr;
}

void StandardItemWrapper::clear()
{
    item_->removeRows(0, item_->rowCount());
}

std::string StandardItemWrapper::getText() const
{
    return item_->text().toStdString();
}

}
}
