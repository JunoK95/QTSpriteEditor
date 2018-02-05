#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QDockWidget>

/*
 * Window used to display the preivew of the active Sprite.
 */
namespace Ui
{
    class PreviewWidget;
}

class PreviewWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit PreviewWidget(QWidget* pvntParent = 0);
    ~PreviewWidget();

signals:
    void playpauseButtonClicked();
    void framesPerSecondValueChanged(int);
    void actualSizeClicked();

public slots:
    void changePlayPauseButton(QString pstrText);
    void sliderValueChanged(int pintFPS);
    void updateImage(QPixmap pvntPixmap, QSize pvntSize);

private:
    Ui::PreviewWidget* gvntUi;
};

#endif // PREVIEWWIDGET_H
