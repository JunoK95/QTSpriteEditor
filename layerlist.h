#ifndef LAYERLIST_H
#define LAYERLIST_H

#include <QAbstractListModel>
#include <QImage>

/*
 * Custom Layer List using QAbstractListModel as an Abstract Interface, used to store the
 * individual layers in a SpriteFrame.
 */
class LayerList : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LayerList(QObject* pvntParent = 0);
    ~LayerList();

    // Basic functionality:
    int rowCount(const QModelIndex& pvntParent = QModelIndex()) const override;

    QVariant data(const QModelIndex& pvntIndex, int pintRole = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex& pvntIndex, const QVariant &pvntValue,
                 int pintRole = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& pvntIndex) const override;

    // Add data:
    bool insertRows(int pintRow, int pintCount, const QModelIndex& pvntParent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int pintRow, int pintCount, const QModelIndex& pvntParent = QModelIndex()) override;

    void addLayer(QImage layer);

private:
    QList<QImage> gvntLayers;
};

#endif // LAYERLIST_H
