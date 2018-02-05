#include "framemanagerwidget.h"
#include "ui_framemanagerwidget.h"

/*
 * Window used to manage the frames in a Sprite.
 */
FrameManagerWidget::FrameManagerWidget(QWidget* pvntParent) :
    QDockWidget(pvntParent),
    gvntUi(new Ui::FrameManagerWidget)
{
    gvntUi->setupUi(this);
    QObject::connect(gvntUi->btnAdd, &QPushButton::pressed, this, &FrameManagerWidget::addButtonClicked);
    QObject::connect(gvntUi->btnDelete, &QPushButton::pressed, this, &FrameManagerWidget::deleteButtonClicked);
    QObject::connect(gvntUi->lstFrames, &QListView::clicked, this, &FrameManagerWidget::frameClicked);
    QObject::connect(gvntUi->btnDup, &QPushButton::pressed, this, &FrameManagerWidget::frameDupButtonClicked);
}
// Assures safe deletion of FrameManagerWidget.
FrameManagerWidget::~FrameManagerWidget()
{
    delete gvntUi;
}

// Populates the frame manager list with the images of the given FrameList(pvntList) and selects the given index(pintSelectedIndex).
void FrameManagerWidget::updateListView(FrameList* pvntList, int pintSelectedIndex)
{
    QStandardItemModel* contentItemModel = new QStandardItemModel(this);
    gvntUi->lstFrames->setModel(contentItemModel);
    gvntUi->lstFrames->clearSelection();

    for(int i = 0; i < pvntList->rowCount(); i++)
    {
        QPixmap vntPixmap = pvntList->data(pvntList->index(i)).value<QPixmap>();
        QStandardItem* item1 = new QStandardItem();
        item1->setData(vntPixmap.scaled(128, 128), Qt::DecorationRole);

        contentItemModel->appendRow(item1);
    }
    gvntUi->lstFrames->setCurrentIndex(contentItemModel->index(pintSelectedIndex, 0));
}

// updates the currently selected frame thumbnail(pvntPixmap) in the frame manager list.
void FrameManagerWidget::updateFrame(QPixmap pvntPixmap)
{
    gvntUi->lstFrames->model()->setData(gvntUi->lstFrames->currentIndex(), pvntPixmap.scaled(128, 128), Qt::DecorationRole);
}
