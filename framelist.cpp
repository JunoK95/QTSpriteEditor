#include "framelist.h"

/*
 * Custom Frame List using QAbstractListModel as an Abstract Interface, used to store the
 * individual frames in a Sprite. pvntParent represents the parent of the newly constructed FrameList.
 */
FrameList::FrameList(QObject* pvntParent)
    : QAbstractListModel(pvntParent)
{
}
// Assures safe deletion of FrameList
FrameList::~FrameList()
{
    qDeleteAll(gvntFrames);
}

// returns the number of SpriteFrames in this FrameList.
int FrameList::rowCount(const QModelIndex& pvntParent) const
{
    if(pvntParent.isValid())
    {
        return 0;
    }
    else
    {
        return gvntFrames.count();
    }
}

// returns the indexed QPixmap in the FrameList
QVariant FrameList::data(const QModelIndex& pvntIndex, int pintRole) const
{
    if(!pvntIndex.isValid())
    {
        return QVariant();
    }
    else
    {
        switch(pintRole)
        {
           case Qt::DecorationRole:
               return gvntFrames.value(pvntIndex.row())->getPixmap();

           case Qt::EditRole:
               return QVariant::fromValue(gvntFrames[pvntIndex.row()]);

           default:
               return gvntFrames.value(pvntIndex.row())->getPixmap();
        }
    }
}

// sets the indexed element in the FrameList to the given value
bool FrameList::setData(const QModelIndex& pvntIndex, const QVariant& pvntValue, int pintRole)
{
    if(data(pvntIndex, pintRole) != pvntValue)
    {
        gvntFrames[pvntIndex.row()] = pvntValue.value<SpriteFrame*>();
        emit dataChanged(pvntIndex, pvntIndex, QVector<int>() << pintRole);
        return true;
    }
    return false;
}

//
Qt::ItemFlags FrameList::flags(const QModelIndex &pvntIndex) const
{
    if(!pvntIndex.isValid())
    {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEditable;
}

// inserts at the given row a count of empty spots for setData() to assign elements to
bool FrameList::insertRows(int pintRow, int pintCount, const QModelIndex& pvntParent)
{
    beginInsertRows(pvntParent, pintRow, pintRow + pintCount - 1);
    while(pintCount > 0)
    {
        SpriteFrame* vntSpriteFrame;
        gvntFrames.insert(pintRow, vntSpriteFrame);
        pintCount--;
    }
    endInsertRows();
    return true;
}

// removes at the given row a count of elements of this FrameList
bool FrameList::removeRows(int pintRow, int pintCount, const QModelIndex &pvntParent)
{
    beginRemoveRows(pvntParent, pintRow, pintRow + pintCount - 1);
    while(pintCount > 0)
    {
        delete gvntFrames[pintRow];
        gvntFrames.removeAt(pintRow);
        pintCount--;
    }
    endRemoveRows();
    return true;
}
