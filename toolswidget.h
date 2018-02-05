#ifndef TOOLSWIDGET_H
#define TOOLSWIDGET_H

#include <QDockWidget>

/*
 * Tools window used to change editor tools in the Sprite Editor.
 */
namespace Ui
{
    class ToolsWidget;
}

class ToolsWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ToolsWidget(QWidget* pvntParent = 0);
    ~ToolsWidget();

signals:
    void penButtonClicked();
    void mirrorButtonClicked();
    void flipButtonClicked();
    void eraserButtonClicked();
    void tileButtonClicked();
    void colorButtonClicked();
    void horizontalFlipClicked();
    void verticalFlipClicked();
    void horizontalMirrorClicked();
    void verticalMirrorClicked();
    void penSliderValueChanged(int);
    void eraserSliderValueChanged(int);
    void tileSliderValueChanged(int);
    void fillButtonClicked();

public slots:
    void changePenButton(bool pblnChecked);
    void changeMirrorButton(bool pblnChecked);
    void changeFlipButton(bool pblnChecked);
    void changeEraserButton(bool pblnChecked);
    void changeTileButton(bool pblnChecked);
    void changeFillButton(bool pblnChecked);
    void updateColor(QColor pvntColor);

private:
    Ui::ToolsWidget* gvntUi;
};

#endif // TOOLSWIDGET_H
