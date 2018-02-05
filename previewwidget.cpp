#include "previewwidget.h"
#include "ui_previewwidget.h"

/*
 * Window used to display the preivew of the active Sprite.
 */
PreviewWidget::PreviewWidget(QWidget* pvntParent) :
    QDockWidget(pvntParent),
    gvntUi(new Ui::PreviewWidget)
{
    gvntUi->setupUi(this);
    QObject::connect(gvntUi->FramesPerSecond, &QSlider::valueChanged, this, &PreviewWidget::sliderValueChanged);
    QObject::connect(gvntUi->PlayPauseButton, &QPushButton::pressed, this, &PreviewWidget::playpauseButtonClicked);
    QObject::connect(gvntUi->ActualSizeCheck, &QCheckBox::pressed, this, &PreviewWidget::actualSizeClicked);
}
// Assures safe deletion of PreviewWidget.
PreviewWidget::~PreviewWidget()
{
    delete gvntUi;
}
// Changes how fast the preiew images cycle. pintFPS represents the speed (per second) images cycle.
void PreviewWidget::sliderValueChanged(int pintFPS)
{
    if(pintFPS > 1)
    {
        gvntUi->FPSLabel->setText(QString("%1 Frames Per Second").arg(pintFPS));
    }
    else
    {
        gvntUi->FPSLabel->setText(QString("%1 Frame Per Second").arg(pintFPS));
    }
    emit framesPerSecondValueChanged(pintFPS);
}
// Changes text of play/pause button. pstrText is the text the button will display.
void PreviewWidget::changePlayPauseButton(QString pstrText)
{
    gvntUi->PlayPauseButton->setText(pstrText);
}
// updates image int preview. pvntPixMap is the image being displayed. pvntSize is the size theimage is displayed at.
void PreviewWidget::updateImage(QPixmap pvntPixmap, QSize pvntSize)
{
    gvntUi->SpritePreview->setMinimumSize(pvntSize);
    gvntUi->SpritePreview->setMaximumSize(pvntSize);
    gvntUi->SpritePreview->setPixmap(pvntPixmap);
}
