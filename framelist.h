#ifndef FRAMELIST_H
#define FRAMELIST_H

#include <QAbstractListModel>
#include "spriteframe.h"

/*
 * Custom Frame List using QAbstractListModel as an Abstract Interface, used to store the
 * individual frames in a Sprite.
 */
class FrameList : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FrameList(QObject* pvntParent = 0);
   ~FrameList();

    // Basic functionality:
    int rowCount(const QModelIndex& pvntParent = QModelIndex()) const override;

    QVariant data(const QModelIndex& pvntIndex, int pintRole = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex& pvntIndex, const QVariant& pvntValue,
                 int pintRole = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& pvntIndex) const override;

    // Add data:
    bool insertRows(int pintRow, int pintCount, const QModelIndex& pvntParent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int pintRow, int pintCount, const QModelIndex& pvntParent = QModelIndex()) override;

private:
    QList<SpriteFrame*> gvntFrames;
};

#endif // FRAMELIST_H
