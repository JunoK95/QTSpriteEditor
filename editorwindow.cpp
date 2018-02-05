#include "editorwindow.h"
#include "ui_editorwindow.h"

/*
 * The primary window used for editing Sprites. pvntParents points to the widget that is this widgets parent.
 */
EditorWindow::EditorWindow(QWidget* pvntParent) :
    QMainWindow(pvntParent),
    gvntUi(new Ui::EditorWindow)
{
    gvntUi->setupUi(this);
    gvntTabWidget = new QTabWidget;
    QObject::connect(gvntTabWidget, &QTabWidget::tabCloseRequested, this, &EditorWindow::tabCloseRequested);
    QObject::connect(gvntTabWidget, &QTabWidget::tabBarClicked, this, &EditorWindow::tabChanged);
    QObject::connect(gvntUi->actionNew, &QAction::triggered, this, &EditorWindow::newTabRequested);
    QObject::connect(gvntUi->actionSave, &QAction::triggered, this, &EditorWindow::saveRequested);
    QObject::connect(gvntUi->actionSaveAs, &QAction::triggered, this, &EditorWindow::saveAsRequested);
    QObject::connect(gvntUi->actionOpen, &QAction::triggered, this, &EditorWindow::openRequested);
    QObject::connect(gvntUi->actionQuit, &QAction::triggered, this, &EditorWindow::quitRequested);
    QObject::connect(gvntUi->actionFrameManager, &QAction::triggered, this, &EditorWindow::frameManagerRequested);
    QObject::connect(gvntUi->actionLayerManager, &QAction::triggered, this, &EditorWindow::layerManagerRequested);
    QObject::connect(gvntUi->actionTools, &QAction::triggered, this, &EditorWindow::toolsRequested);
    QObject::connect(gvntUi->actionPreview, &QAction::triggered, this, &EditorWindow::previewRequested);
    QObject::connect(gvntUi->actionExportAsGIF, &QAction::triggered, this, &EditorWindow::exportRequested);
    setCentralWidget(gvntTabWidget);
}
// Allows for EditorWindow to be terminated safetly.
EditorWindow::~EditorWindow()
{
    delete gvntUi;
    delete gvntTabWidget;
}
// Adds a new tab to EditorWindow. pvntWidget points to the widget tabs are being added to. pstrLabel sets the text of the tab.
void EditorWindow::addTab(QWidget* pvntWidget, const QString& pstrLabel)
{    
    gvntTabWidget->addTab(pvntWidget, pstrLabel);
    gvntTabWidget->setCurrentIndex(gvntTabWidget->count() - 1);
}
// Closes the selected tab. Selected tab is defined by pintIndex
void EditorWindow::closeTab(int pintIndex)
{
    delete gvntTabWidget->widget(pintIndex);
}
// Sets whether or not a tab may be closed. Tabs are closable when pblnClosable is true.
void EditorWindow::setTabsClosable(bool pblnClosable)
{
    gvntTabWidget->setTabsClosable(pblnClosable);
}
// Modifies the visibility of the FrameManager.FrameManager is visible when pblnVisible is true.
void EditorWindow::setFrameManagerVisible(bool pblnVisible)
{
    gvntUi->actionFrameManager->setChecked(pblnVisible);
}
// Modifies the visibility of the LayerManager. LayerManager is visible when pblnVisible is true.
void EditorWindow::setLayerManagerVisible(bool pblnVisible)
{
    gvntUi->actionLayerManager->setChecked(pblnVisible);
}
// Modifies the visibility of the Tools window. Tool window is visible when pblnVisible is true.
void EditorWindow::setToolsVisible(bool pblnVisible)
{
    gvntUi->actionTools->setChecked(pblnVisible);
}
// Modifies the visibility of the Preview window. Preview window is visible when pblnVisible is true.
void EditorWindow::setPreviewVisible(bool pblnVisible)
{
    gvntUi->actionPreview->setChecked(pblnVisible);
}
// Replaces selected tab with a different tab. Selected tab is defined by pintIndex. pvntWidget points to the parent
// of the replacing tab.
void EditorWindow::replaceTab(int pintIndex, QWidget* pvntWidget)
{
    QString strName = gvntTabWidget->tabText(pintIndex);
    delete gvntTabWidget->widget(pintIndex);
    gvntTabWidget->insertTab(pintIndex, pvntWidget, strName);
    gvntTabWidget->setCurrentIndex(pintIndex);
}
// Changes the text of a tab. pintIndex indicates which tab's text is changed. pstrName is what the text is changed to.
void EditorWindow::renameTab(int pintIndex, QString pstrName)
{
    gvntTabWidget->setTabText(pintIndex, pstrName);
}
