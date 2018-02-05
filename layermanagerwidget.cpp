#include "layermanagerwidget.h"
#include "ui_layermanagerwidget.h"

/*
 * Window used to manage the layers in a SpriteFrame.
 */
LayerManagerWidget::LayerManagerWidget(QWidget*pvntParent) :
    QDockWidget(pvntParent),
    gvntUi(new Ui::LayerManagerWidget)
{
    gvntUi->setupUi(this);
    QObject::connect(gvntUi->btnAdd, &QPushButton::pressed, this, &LayerManagerWidget::addButtonClicked);
    QObject::connect(gvntUi->btnDelete, &QPushButton::pressed , this, &LayerManagerWidget::deleteButtonClicked);
    QObject::connect(gvntUi->btnDuplicate, &QPushButton::pressed, this, &LayerManagerWidget::duplicateButtonClicked);
    QObject::connect(gvntUi->btnCombine, &QPushButton::pressed, this, &LayerManagerWidget::combineButtonClicked);
    QObject::connect(gvntUi->lstLayers, &QListView::clicked, this, &LayerManagerWidget::layerSelected);
}
// Assures safe deletion of LayerManagerWidget.
LayerManagerWidget::~LayerManagerWidget()
{
    delete gvntUi;
}
// Updates the list of layers so that they are displayed correctly. pvntList represents the updated list.
// pintSelectedIndex represents the current selected layer.
void LayerManagerWidget::updateListView(LayerList* pvntList, int pintSelectedIndex)
{
    QStandardItemModel* contentItemModel = new QStandardItemModel(this);
    gvntUi->lstLayers->setModel(contentItemModel);
    gvntUi->lstLayers->clearSelection();
    for(int i = 0; i < pvntList->rowCount(); i++)
    {
        QStandardItem* vntItem = new QStandardItem();
        vntItem->setData(pvntList->data(pvntList->index(i)).value<QString>(), Qt::DisplayRole);
        contentItemModel->appendRow(vntItem);
    }
    gvntUi->lstLayers->setCurrentIndex(contentItemModel->index(pintSelectedIndex, 0));
}
