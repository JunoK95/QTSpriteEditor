#ifndef LAYERMANAGERWIDGET_H
#define LAYERMANAGERWIDGET_H

#include <QDockWidget>
#include <QModelIndexList>
#include <QStandardItemModel>
#include "layerlist.h"

/*
 * Window used to manage the layers in a SpriteFrame.
 */
namespace Ui
{
    class LayerManagerWidget;
}

class LayerManagerWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit LayerManagerWidget(QWidget* pvntParent = 0);
    ~LayerManagerWidget();

signals:
    void combineButtonClicked();
    void addButtonClicked();
    void deleteButtonClicked();
    void duplicateButtonClicked();
    void layerSelected(QModelIndex);

public slots:
    void updateListView(LayerList* pvntListModel, int pintSelectedIndex);

private:
    Ui::LayerManagerWidget* gvntUi;
};

#endif // LAYERMANAGERWIDGET_H
