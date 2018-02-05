#include "layerlist.h"

/*
 * Custom Layer List using QAbstractListModel as an Abstract Interface, used to store the
 * individual layers in a SpriteFrame.
 */
LayerList::LayerList(QObject* pvntParent)
    : QAbstractListModel(pvntParent)
{
}

LayerList::~LayerList()
{
}
// Returns number of layers
int LayerList::rowCount(const QModelIndex& pvntParent) const
{
    if(pvntParent.isValid())
    {
        return 0;
    }
    else
    {
        return gvntLayers.count();
    }
}
// Returns indexed layer
QVariant LayerList::data(const QModelIndex& pvntIndex, int pintRole) const
{
    if(!pvntIndex.isValid())
    {
        return QVariant();
    }
    else
    {
        switch(pintRole)
        {
            case Qt::DisplayRole:
                return QString("Layer %1").arg(pvntIndex.row() + 1);

            default:
                return gvntLayers[pvntIndex.row()];
        }
    }
}
// Sets indexed element to a given value
bool LayerList::setData(const QModelIndex& pvntIndex, const QVariant& pvntValue, int pintRole)
{
    if(data(pvntIndex, pintRole) != pvntValue)
    {
        QImage vntImage = pvntValue.value<QImage>();
        gvntLayers[pvntIndex.row()] = vntImage;
        emit dataChanged(pvntIndex, pvntIndex, QVector<int>() << pintRole);
        return true;
    }
    return false;
}

Qt::ItemFlags LayerList::flags(const QModelIndex& pvntIndex) const
{
    if(!pvntIndex.isValid())
    {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEditable;
}
// Adds a new layer to the list of layers
bool LayerList::insertRows(int pintRow, int pintCount, const QModelIndex& pvntParent)
{
    beginInsertRows(pvntParent, pintRow, pintRow + pintCount - 1);
    while(pintCount > 0)
    {
        QImage vntImage;
        gvntLayers.insert(pintRow, vntImage);
        pintCount--;
    }
    endInsertRows();
    return true;
}
// Removes the indexed layer from the list of layers
bool LayerList::removeRows(int pintRow, int pintCount, const QModelIndex& pvntParent)
{
    beginRemoveRows(pvntParent, pintRow, pintRow + pintCount - 1);
    while(pintCount > 0)
    {
        gvntLayers.removeAt(pintRow);
        pintCount--;
    }
    endRemoveRows();
    return true;
}
//
void LayerList::addLayer(QImage layer)
{
    gvntLayers.append(layer);
}
