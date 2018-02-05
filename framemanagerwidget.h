#ifndef FRAMEMANAGERWIDGET_H
#define FRAMEMANAGERWIDGET_H

#include <QDockWidget>
#include <QListView>
#include <QStandardItemModel>
#include "framelist.h"

/*
 * Window used to manage the frames in a Sprite.
 */
namespace Ui
{
    class FrameManagerWidget;
}

class FrameManagerWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit FrameManagerWidget(QWidget* pvntParent = 0);
    ~FrameManagerWidget();

signals:
    void addButtonClicked();
    void deleteButtonClicked();
    void frameClicked(QModelIndex);
    void frameDupButtonClicked();

public slots:    
    void updateListView(FrameList* pvntList, int pintSelectedIndex);
    void updateFrame(QPixmap pvntPixmap);

private:
    Ui::FrameManagerWidget* gvntUi;
};

#endif // FRAMEMANAGERWIDGET_H
