#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QApplication>
#include <QColorDialog>
#include <QGridLayout>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QTimer>
#include <QTimerEvent>
#include "editorwindow.h"
#include "framemanagerwidget.h"
#include "layermanagerwidget.h"
#include "previewwidget.h"
#include "toolswidget.h"
#include "sprite.h"
#include "canvaslabel.h"

/*
 * Controls all functions of the Sprite Editor Application, ensures that none of the visible
 * interface functions freeze when the user interacts with the Sprite Editor.
 */
class Controller : public QObject
{
    Q_OBJECT

public:
    Controller();
    ~Controller();
    void loadWindows();

signals:
    void addEditorTab(QWidget*, const QString&);
    void removeEditorTab(int);
    void setTabsClosable(bool);
    void replaceTab(int, QWidget*);
    void renameTab(int, QString);
    void updateFrameManager(FrameList*, int);
    void updateLayerManager(LayerList*, int);
    void updateColorDisplay(QColor);
    void updateSelectedFrame(QPixmap);
    void updatePenButton(bool);
    void updateMirrorButton(bool);
    void updateEraserButton(bool);
    void updateTileButton(bool);
    void updateFlipButton(bool);
    void updateFillButton(bool);
    void updateFrameManagerCheckbox(bool);
    void updateLayerManagerCheckbox(bool);
    void updateToolsCheckbox(bool);
    void updatePreviewCheckbox(bool);
    void updatePlayPauseButton(QString);
    void updatePreviewImage(QPixmap, QSize);

public slots:
    void tabChanged(int pintIndex);
    void tabCloseRequested(int pintIndex);
    void newEditorTab(bool pblnChecked);
    void quitApplication(bool pblnChecked);
    void canvasClicked(CanvasLabel* pvntLabel, QPoint pvntPoint);
    void exportAsGIF();
    void saveToFile();
    void saveAsToFile();
    void openFile();
    void penButtonPressed();
    void mirrorButtonPressed();
    void flipButtonPressed();
    void eraserButtonPressed();
    void tileButtonPressed();
    void fillButtonPressed();
    void colorButtonPressed();
    void horizontalFlipCheckboxPressed();
    void verticalFlipCheckboxPressed();
    void horizontalMirrorCheckboxPressed();
    void verticalMirrorCheckboxPressed();
    void changePenSize(int pintSize);
    void changeEraserSize(int pintSize);
    void changeTileSize(int pintSize);
    void addNewFrame();
    void deleteFrame();
    void duplicateFrame();
    void frameSelected(QModelIndex pvntIndex);
    void addLayer();
    void selectLayer(QModelIndex pvntIndex);
    void deleteLayer();
    void duplicateLayer();
    void combineLayers();
    void frameManagerVisibility(bool pblnVisible);
    void layerManagerVisibility(bool pblnVisible);
    void toolsVisibility(bool pblnVisible);
    void previewVisibility(bool pblnVisible);
    void focusOnFrameManager(bool pblnChecked);
    void focusOnLayerManager(bool pblnChecked);
    void focusOnTools(bool pblnChecked);
    void focusOnPreview(bool pblnChecked);
    void previewSpeed(int pintFramesPerSecond);
    void playPauseButtonPressed();
    void actualSizeCheckboxPressed();
    void cyclePreviewImage();

private:
    void setupEditorWindowConnections();
    void setupFrameManagerConnections();
    void setupLayerManagerConnections();
    void setupPreviewConnections();
    void setupToolsConnections();
    void refreshToolsWindow();
    void refreshCanvas();
    void refreshPreviewWindow();
    bool saveFile(QString pstrFileName);
    int fileModifiedDialog(QString pstrFileName);
    QWidget* createWidgetForTab(QPixmap pvntPixmap);
    void newEditorTabFromFile(bool pblnChecked, int pintSize, QString pstrName);

    EditorWindow gvntEditorWindow;
    FrameManagerWidget gvntFrameManager;
    LayerManagerWidget gvntLayerManager;
    PreviewWidget gvntPreview;
    ToolsWidget gvntTools;
    QList<Sprite*> gvntSprites;
    QTimer gvntTimer;
    QColor gvntPenColor;
    int gintCurrentTab;
    int gintCurrentFrame;
    int gintCurrentLayer;
    int gintPreviewFrame;
    int gintPenSize;
    int gintEraserSize;
    int gintTileSize;
    int gintFramesPerSecond;
    bool gblnAcutalSize;
    bool gblnFrameManagerVisible;
    bool gblnLayerManagerVisible;
    bool gblnToolsVisible;
    bool gblnPreviewVisible;
    bool gblnPenTool;
    bool gblnMirrorTool;
    bool gblnEraserTool;
    bool gblnTileTool;
    bool gblnFlipTool;
    bool gblnHorizontalMirror;
    bool gblnVerticalMirror;
    bool gblnHorizontalFlip;
    bool gblnVerticalFlip;
    bool gblnFillTool;
};

#endif // CONTROLLER_H
