#include "toolswidget.h"
#include "ui_toolswidget.h"

/*
 * Tools window used to change editor tools in the Sprite Editor.
 */
ToolsWidget::ToolsWidget(QWidget* pvntParent) :
    QDockWidget(pvntParent),
    gvntUi(new Ui::ToolsWidget)
{
    gvntUi->setupUi(this);
    QObject::connect(gvntUi->PenButton, &QPushButton::pressed, this, &ToolsWidget::penButtonClicked);
    QObject::connect(gvntUi->FlipButton, &QPushButton::pressed, this, &ToolsWidget::flipButtonClicked);
    QObject::connect(gvntUi->MirrorButton, &QPushButton::pressed, this, &ToolsWidget::mirrorButtonClicked);
    QObject::connect(gvntUi->EraserButton, &QPushButton::pressed, this, &ToolsWidget::eraserButtonClicked);
    QObject::connect(gvntUi->TileButton, &QPushButton::pressed, this, &ToolsWidget::tileButtonClicked);
    QObject::connect(gvntUi->FillButton, &QPushButton::pressed, this, &ToolsWidget::fillButtonClicked);
    QObject::connect(gvntUi->ColorButton, &QPushButton::pressed, this, &ToolsWidget::colorButtonClicked);
    QObject::connect(gvntUi->PenSizeSlider, &QSlider::valueChanged, this, &ToolsWidget::penSliderValueChanged);
    QObject::connect(gvntUi->EraserSizeSlider, &QSlider::valueChanged, this, &ToolsWidget::eraserSliderValueChanged);
    QObject::connect(gvntUi->HorizontalFlip, &QCheckBox::pressed, this, &ToolsWidget::horizontalFlipClicked);
    QObject::connect(gvntUi->VerticalFlip, &QCheckBox::pressed, this, &ToolsWidget::verticalFlipClicked);
    QObject::connect(gvntUi->HorizontalMirror, &QCheckBox::pressed, this, &ToolsWidget::horizontalMirrorClicked);
    QObject::connect(gvntUi->VerticalMirror, &QCheckBox::pressed, this, &ToolsWidget::verticalMirrorClicked);
    QObject::connect(gvntUi->TileSizeSlider, &QSlider::valueChanged, this, &ToolsWidget::tileSliderValueChanged);
}
// Assures safe deletion of ToolsWidget.
ToolsWidget::~ToolsWidget()
{
    delete gvntUi;
}
// Changes selected tool button to pen button.
void ToolsWidget::changePenButton(bool pblnChecked)
{
    gvntUi->PenButton->setChecked(pblnChecked);
}
// Changes selected tool button to flip button.
void ToolsWidget::changeFlipButton(bool pblnChecked)
{
    gvntUi->FlipButton->setChecked(pblnChecked);
}
// Changes selected tool button to mirror button.
void ToolsWidget::changeMirrorButton(bool pblnChecked)
{
    gvntUi->MirrorButton->setChecked(pblnChecked);
}
// Changes selected tool button to eraser button.
void ToolsWidget::changeEraserButton(bool pblnChecked)
{
    gvntUi->EraserButton->setChecked(pblnChecked);
}
// Changes selected tool button to tile button.
void ToolsWidget::changeTileButton(bool pblnChecked)
{
    gvntUi->TileButton->setChecked(pblnChecked);
}
// Changes selected tool button to fill button.
void ToolsWidget::changeFillButton(bool pblnChecked)
{
    gvntUi->FillButton->setChecked(pblnChecked);
}
// Updates the color display to th right color.
void ToolsWidget::updateColor(QColor pvntColor)
{
    gvntUi->ColorButton->setStyleSheet(QString("background-color: %1; border-style: outset; border-width: 1px; border-radius: 5px; border-color: black;").arg(pvntColor.name()));
}
