#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>

/*
 * The primary window used for editing Sprites.
 */
namespace Ui
{
    class EditorWindow;
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget* pvntParent = 0);
    ~EditorWindow();

signals:
    void tabChanged(int);
    void tabCloseRequested(int);
    void newTabRequested(bool);
    void saveRequested();
    void saveAsRequested();
    void openRequested();
    void quitRequested(bool);
    void frameManagerRequested(bool);
    void layerManagerRequested(bool);
    void toolsRequested(bool);
    void previewRequested(bool);
    void exportRequested();

public slots:
    void addTab(QWidget* pvntWidget, const QString& pstrLabel);
    void closeTab(int pintIndex);
    void setTabsClosable(bool pblnClosable);
    void setFrameManagerVisible(bool pblnVisible);
    void setLayerManagerVisible(bool pblnVisible);
    void setToolsVisible(bool pblnVisible);
    void setPreviewVisible(bool pblnVisible);
    void replaceTab(int pintIndex, QWidget* pvntWidget);
    void renameTab(int pintIndex, QString pstrName);

private:
    Ui::EditorWindow* gvntUi;
    QTabWidget* gvntTabWidget;
};

#endif // EDITORWINDOW_H
