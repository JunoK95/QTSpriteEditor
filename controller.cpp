#include "controller.h"
#include "./GIF/gif.h"

/*
 * Controls all functions of the Sprite Editor Application, ensures that none of the visible
 * interface functions freeze when the user interacts with the Sprite Editor.
 */
Controller::Controller()
{
    // Setting default values
    gintFramesPerSecond = 1000;
    gintPenSize = 1;
    gintEraserSize = 1;
    gintTileSize = 2;
    gblnHorizontalMirror = false;
    gblnVerticalMirror = false;
    gblnHorizontalFlip = false;
    gblnVerticalFlip = false;
    gvntPenColor = QColor(204,0,0,255); // UofU red
    gblnPenTool = true;
    gblnMirrorTool = false;
    gblnEraserTool = false;
    gblnTileTool = false;
    gblnFlipTool = false;
    gblnFillTool = false;
}
//Deconstructs Controller so that there are no memory leaks.
Controller::~Controller()
{
    qDeleteAll(gvntSprites);
}

// Loads the windows and sets the default settings for the controller. De
void Controller::loadWindows()
{
    setupEditorWindowConnections();
    setupFrameManagerConnections();
    setupToolsConnections();
    setupLayerManagerConnections();
    setupPreviewConnections();
    QObject::connect(&gvntTimer, &QTimer::timeout, this, &Controller::cyclePreviewImage);

    emit updatePenButton(true);
    emit updateColorDisplay(gvntPenColor);
    gvntSprites.append(new Sprite("", 32));
    QWidget* vntWidget = createWidgetForTab(gvntSprites.last()->getFrame(0)->getPixmap(true, 0));
    emit addEditorTab(vntWidget, "Untitled.ssp");
    tabChanged(gvntSprites.count() - 1);
    gvntEditorWindow.addDockWidget(Qt::BottomDockWidgetArea, &gvntFrameManager);
    gvntEditorWindow.addDockWidget(Qt::RightDockWidgetArea, &gvntLayerManager);
    gvntEditorWindow.addDockWidget(Qt::LeftDockWidgetArea, &gvntPreview);
    gvntEditorWindow.addDockWidget(Qt::LeftDockWidgetArea, &gvntTools);
    gvntEditorWindow.show();
}

// Updates the checkbox for the frame manager in the windows menu
void Controller::frameManagerVisibility(bool pblnVisible)
{
    gblnFrameManagerVisible = pblnVisible;
    emit updateFrameManagerCheckbox(gblnFrameManagerVisible);
}

// Updates the checkbox for the layer manager in the windows menu
void Controller::layerManagerVisibility(bool pblnVisible)
{
    gblnLayerManagerVisible = pblnVisible;
    emit updateLayerManagerCheckbox(gblnLayerManagerVisible);
}

// Updates the checkbox for the tools in the windows menu
void Controller::toolsVisibility(bool pblnVisible)
{
    gblnToolsVisible = pblnVisible;
    emit updateToolsCheckbox(gblnToolsVisible);
}

// Updates the checkbox for the preview in the windows menu
void Controller::previewVisibility(bool pblnVisible)
{
    gblnPreviewVisible = pblnVisible;
    emit updatePreviewCheckbox(gblnPreviewVisible);
}

// Requests that the frame manager be brought into focus
void Controller::focusOnFrameManager(bool pblnChecked)
{
    Q_UNUSED(pblnChecked)
    if(gblnFrameManagerVisible)
    {
        gvntFrameManager.activateWindow();
        gvntFrameManager.raise();
    }
    else
    {
        gvntFrameManager.show();
        gvntFrameManager.activateWindow();
        gvntFrameManager.raise();
        gvntFrameManager.setFloating(false);
    }
    emit updateFrameManagerCheckbox(gblnFrameManagerVisible);
}

// Requests that the layer manager be brought into focus
void Controller::focusOnLayerManager(bool pblnChecked)
{
    Q_UNUSED(pblnChecked)
    if(gblnLayerManagerVisible)
    {
        gvntLayerManager.activateWindow();
        gvntLayerManager.raise();
    }
    else
    {
        gvntLayerManager.show();
        gvntLayerManager.activateWindow();
        gvntLayerManager.raise();
        gvntLayerManager.setFloating(false);
    }
    emit updateLayerManagerCheckbox(gblnLayerManagerVisible);
}

// Requests that the tools be brought into focus
void Controller::focusOnTools(bool pblnChecked)
{
    Q_UNUSED(pblnChecked)
    if(gblnToolsVisible)
    {
        gvntTools.activateWindow();
        gvntTools.raise();
    }
    else
    {
        gvntTools.show();
        gvntTools.activateWindow();
        gvntTools.raise();
        gvntTools.setFloating(false);
    }
    emit updateToolsCheckbox(gblnToolsVisible);
}

// Requests that the preview be brought into focus
void Controller::focusOnPreview(bool pblnChecked)
{
    Q_UNUSED(pblnChecked)
    if(gblnPreviewVisible)
    {
        gvntPreview.activateWindow();
        gvntPreview.raise();
    }
    else
    {
        gvntPreview.show();
        gvntPreview.activateWindow();
        gvntPreview.raise();
        gvntPreview.setFloating(false);
    }
    emit updatePreviewCheckbox(gblnPreviewVisible);
}

// The selected tab (sprite) has been changed, updates controller information and respecting windows
void Controller::tabChanged(int pintIndex)
{
    gintCurrentTab = pintIndex;
    gintCurrentFrame = 0;
    gintCurrentLayer = 0;
    gintPreviewFrame = 0;
    emit updateFrameManager(gvntSprites[gintCurrentTab]->getFrameList(), gintCurrentFrame);
    emit updateLayerManager(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList(), gintCurrentLayer);
    refreshPreviewWindow();
}

// A request to quit the application was made, verify and save sprites if needed
void Controller::quitApplication(bool pblnChecked)
{
    Q_UNUSED(pblnChecked)

    for(int i = gvntSprites.size() - 1; i >= 0; i--)
    {
        if(gvntSprites[i]->hasChanged())
        {
            tabChanged(i);
            QString strFileName = gvntSprites[i]->fileName();
            if(!strFileName.isEmpty())
            {
                QStringList vntFileDirectories = strFileName.split("/");
                strFileName = vntFileDirectories.last();
            }
            int intResult = fileModifiedDialog(strFileName);
            switch (intResult) {
                case QMessageBox::Save:
                    saveToFile();
                    break;
                case QMessageBox::Cancel:
                    return;
                    break;
                default:
                    break;
            }
        }
    }
    gvntEditorWindow.close();
}

// Exports the current sprite to a GIF
void Controller::exportAsGIF()
{
    QStringList vntStringList;
    vntStringList << "Actual Size" << "Editor Size";

    bool blnStatus;
    QString strSelectedItem = QInputDialog::getItem(&gvntEditorWindow, "Export Sprite", "Export Size: ",
                                                    vntStringList, 0, false, &blnStatus);
    if(blnStatus && !strSelectedItem.isEmpty())
    {
        QString strFileName = QFileDialog::getSaveFileName(&gvntEditorWindow, "Export Sprite", QDir::homePath(),
                                                           "GIF (*.gif);;All Files (*)");
        GifWriter* vntGifWriter = new GifWriter();
        std::string strStdFileName = strFileName.toStdString();
        const char* charFileName = strStdFileName.c_str();

        if(vntStringList.indexOf(strSelectedItem) == 0)
        {
            int intSize = gvntSprites[gintCurrentTab]->getSize();
            GifBegin(vntGifWriter, charFileName, intSize, intSize, 100);
            for(int intFrame = 0; intFrame < gvntSprites[gintCurrentTab]->getFrameList()->rowCount(); intFrame++){
                QImage vntCurrentImage = gvntSprites[gintCurrentTab]->getFrame(intFrame)->getImage().copy();
                QImage vntSwappedImage = vntCurrentImage.rgbSwapped();
                for(int intRow = 0; intRow < vntSwappedImage.height(); intRow++)
                {
                    for(int intColumn = 0; intColumn < vntSwappedImage.width(); intColumn++)
                    {
                        QColor vntWhite(255, 255, 255, 255);
                        QColor vntPixelColor = vntSwappedImage.pixelColor(intRow, intColumn);
                        if(vntPixelColor.alpha() == 0)
                        {
                            vntSwappedImage.setPixelColor(intRow, intColumn, vntWhite);
                        }
                    }
                }
                GifWriteFrame(vntGifWriter, vntSwappedImage.constBits(), intSize, intSize, 100);
            }
            GifEnd(vntGifWriter);
        }
        else
        {
            int intSize = 512;
            GifBegin(vntGifWriter, charFileName, intSize, intSize, 100);
            for(int intFrame = 0; intFrame < gvntSprites[gintCurrentTab]->getFrameList()->rowCount(); intFrame++){
                QPixmap vntCurrentPixmap = gvntSprites[gintCurrentTab]->getFrame(intFrame)->getPixmap();
                QImage vntCurrentImage = vntCurrentPixmap.toImage();
                QImage vntSwappedImage = vntCurrentImage.rgbSwapped();
                for(int intRow = 0; intRow < vntSwappedImage.height(); intRow++)
                {
                    for(int intColumn = 0; intColumn < vntSwappedImage.width(); intColumn++)
                    {
                        QColor vntWhite(255, 255, 255, 255);
                        QColor vntPixelColor = vntSwappedImage.pixelColor(intRow, intColumn);
                        if(vntPixelColor.alpha() == 0)
                        {
                            vntSwappedImage.setPixelColor(intRow, intColumn, vntWhite);
                        }
                    }
                }
                GifWriteFrame(vntGifWriter, vntSwappedImage.constBits(), intSize, intSize, 100);
            }
            GifEnd(vntGifWriter);
        }
    }
}

// Displays the dialog to inform the user that they have unsaved changes
int Controller::fileModifiedDialog(QString pstrFileName)
{
    return QMessageBox::question(&gvntEditorWindow, QString("%1 has been modified").arg(pstrFileName),
                                 "Do you want to save your changes?",
                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                 QMessageBox::Save);
}

// Saves a sprite file with the requested file name.
bool Controller::saveFile(QString pstrFileName)
{
    QString strLayerFileName = QString(pstrFileName);
    strLayerFileName.remove(".ssp");
    strLayerFileName.append(".ssl");
    QFile vntSaveFile(pstrFileName);
    if (!vntSaveFile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(&gvntEditorWindow, "Unable to open file",
                                 vntSaveFile.errorString());
        return false;
    }
    else
    {
        int intSize = gvntSprites[gintCurrentTab]->getSize();
        int intNumFrames = gvntSprites[gintCurrentTab]->getFrameList()->rowCount();
        QTextStream vntOutputStream(&vntSaveFile);
        vntOutputStream << intSize << " " << intSize << "\n";
        vntOutputStream << intNumFrames << "\n";

        for(int intFrameIndex = 0; intFrameIndex < intNumFrames; intFrameIndex++)
        {
            QPixmap vntCurrentPixmap = gvntSprites[gintCurrentTab]->getFrame(intFrameIndex)->getPixmap();
            QPixmap vntScaledPixmap = vntCurrentPixmap.scaled(intSize, intSize, Qt::KeepAspectRatio);
            QImage vntCurrentImage = vntScaledPixmap.toImage();
            for(int intRow = 0; intRow < intSize; intRow++)
            {
                for(int intColumn = 0; intColumn < intSize; intColumn++)
                {
                    QRgb vntCurrentPixel = vntCurrentImage.pixel(intRow, intColumn);
                    vntOutputStream << qRed(vntCurrentPixel) << " ";
                    vntOutputStream << qGreen(vntCurrentPixel) << " ";
                    vntOutputStream << qBlue(vntCurrentPixel) << " ";
                    vntOutputStream << qAlpha(vntCurrentPixel);
                    if(intColumn != intSize - 1){
                        vntOutputStream << " ";
                    }
                }
                vntOutputStream << "\n";
            }
        }
    }
    QFile vntSaveLayerFile(strLayerFileName);
    if (!vntSaveLayerFile.open(QIODevice::WriteOnly)) {
        QMessageBox::information(&gvntEditorWindow, "Unable to open file", vntSaveLayerFile.errorString());
        return false;
    }
    else
    {
        int intSize = gvntSprites[gintCurrentTab]->getSize();
        int intNumFrames = gvntSprites[gintCurrentTab]->getFrameList()->rowCount();
        QTextStream vntOutputStream(&vntSaveLayerFile);
        vntOutputStream << intSize << " " << intSize << "\n";
        vntOutputStream << intNumFrames << "\n";

        for(int intFrameIndex = 0; intFrameIndex < intNumFrames; intFrameIndex++)
        {
            int intNumLayers = gvntSprites[gintCurrentTab]->getFrame(intFrameIndex)->getLayerList()->rowCount();
            vntOutputStream << intNumLayers << "\n";
            for(int intLayer=0; intLayer<intNumLayers; intLayer++)
            {
                QPixmap vntCurrentPixmap = gvntSprites[gintCurrentTab]->getFrame(intFrameIndex)->
                                           getPixmap(false, intLayer, false, 0);
                QPixmap vntScaledPixmap = vntCurrentPixmap.scaled(intSize, intSize, Qt::KeepAspectRatio);
                QImage vntCurrentImage = vntScaledPixmap.toImage();

                for(int intRow = 0; intRow < intSize; intRow++)
                {
                    for(int intColumn = 0; intColumn < intSize; intColumn++)
                    {
                        QRgb vntCurrentPixel = vntCurrentImage.pixel(intRow, intColumn);
                        vntOutputStream << qRed(vntCurrentPixel) << " ";
                        vntOutputStream << qGreen(vntCurrentPixel) << " ";
                        vntOutputStream << qBlue(vntCurrentPixel) << " ";
                        vntOutputStream << qAlpha(vntCurrentPixel);
                        if(intColumn != intSize - 1){
                            vntOutputStream << " ";
                        }
                    }
                    vntOutputStream << "\n";
                }
            }
        }
    }
    return true;
}

// Requests a save to file and verifies if the sprite already has a file name stored
void Controller::saveToFile()
{
    if(gvntSprites[gintCurrentTab]->fileName().isEmpty())
    {
        saveAsToFile();
    }
    else
    {
        if(saveFile(gvntSprites[gintCurrentTab]->fileName()))
        {
            gvntSprites[gintCurrentTab]->resetModified();
        }
    }
}

// Requests to save a sprite to a new file
void Controller::saveAsToFile()
{
    QString strFileName = QFileDialog::getSaveFileName(&gvntEditorWindow, "Save Sprite",
                                                       QDir::homePath(), "Sprite Sheet Project (*.ssp);;All Files (*)");
    if(!strFileName.isEmpty())
    {
        if(saveFile(strFileName))
        {
            gvntSprites[gintCurrentTab]->resetModified();
            gvntSprites[gintCurrentTab]->setFileName(strFileName);
            QStringList vntFileDirectories = strFileName.split("/");
            strFileName = vntFileDirectories.last();
            emit renameTab(gintCurrentTab, strFileName);
        }
    }
}

// Requests to open a new Sprite
void Controller::openFile()
{
    QString strFileLocation = QFileDialog::getOpenFileName(&gvntEditorWindow, "Open Sprite",
                                                           QDir::homePath(), "Sprite Sheet Project (*.ssp);;All Files (*)");
    if(!strFileLocation.isEmpty())
    {
        QString strLayerFileLocation = QString(strFileLocation);
        strLayerFileLocation.remove(".ssp");
        strLayerFileLocation.append(".ssl");
        QFile vntOpenFile(strLayerFileLocation);
        if (!vntOpenFile.open(QIODevice::ReadOnly))
        {
            QFile vntOpenFile(strFileLocation);
            if(!vntOpenFile.open(QIODevice::ReadOnly))
            {
                QMessageBox::information(&gvntEditorWindow,"Unable to open file", vntOpenFile.errorString());
                return;
            }
            try
            {
                QTextStream vntInputStream(&vntOpenFile);

                QString strSpriteSizeLine = vntInputStream.readLine();
                QString strSpriteFrameLine = vntInputStream.readLine();
                QStringList vntSpriteSizeList = strSpriteSizeLine.split(" ");
                int intWidth = vntSpriteSizeList[0].toInt();
                int intHeight = vntSpriteSizeList[1].toInt();
                int intNumFrames = strSpriteFrameLine.toInt();
                if(intWidth <= 0 || intHeight <= 0)
                {
                    QMessageBox::information(&gvntEditorWindow, "Error opening file",
                                             "File specifies a zero height and/or width");
                    return;
                }
                newEditorTabFromFile(true, intWidth, strFileLocation);
                for(int intFrameIndex = 0; intFrameIndex < intNumFrames; intFrameIndex++)
                {
                    if(intFrameIndex != 0)
                    {
                        gvntSprites.last()->addFrame();
                    }
                    for(int intRow = 0; intRow < intHeight; intRow++)
                    {
                        QString strCurrentRowLine = vntInputStream.readLine();
                        QStringList vntCurrentRow = strCurrentRowLine.split(" ");
                        for(int intColumn = 0; intColumn < intWidth; intColumn++)
                        {
                            int intRed = vntCurrentRow.at(0 + (intColumn * 4)).toInt();
                            int intGreen = vntCurrentRow.at(1 + (intColumn * 4)).toInt();
                            int intBlue = vntCurrentRow.at(2 + (intColumn * 4)).toInt();
                            int intAlpha = vntCurrentRow.at(3 + (intColumn * 4)).toInt();
                            QColor vntCellColor(intRed, intGreen, intBlue, intAlpha);
                            gvntSprites.last()->getFrame(intFrameIndex)->
                                    drawOnLayerFromFile(gintCurrentLayer, intRow, intColumn, vntCellColor);
                        }
                    }
                }
            }
            catch(const std::exception& ex){
                Q_UNUSED(ex);
                QMessageBox::information(&gvntEditorWindow, "Error opening file",
                                         "File does not have the correct format");
                return;
            }
        }
        else
        {
            try
            {
                QTextStream vntInputStream(&vntOpenFile);

                QString strSpriteSizeLine = vntInputStream.readLine();
                QString strSpriteFrameLine = vntInputStream.readLine();
                QStringList vntSpriteSizeList = strSpriteSizeLine.split(" ");
                int intWidth = vntSpriteSizeList[0].toInt();
                int intHeight = vntSpriteSizeList[1].toInt();
                int intNumFrames = strSpriteFrameLine.toInt();
                if(intWidth <= 0 || intHeight <= 0)
                {
                    QMessageBox::information(&gvntEditorWindow, "Error opening file",
                                             "File specifies a zero height and/or width");
                    return;
                }
                newEditorTabFromFile(true, intWidth, strFileLocation);
                for(int intFrameIndex = 0; intFrameIndex < intNumFrames; intFrameIndex++)
                {
                    if(intFrameIndex != 0)
                    {
                        gvntSprites.last()->addFrame();
                    }
                    QString strLayerNumLine = vntInputStream.readLine();
                    int intNumLayers = strLayerNumLine.toInt();
                    for(int intLayer = 0; intLayer < intNumLayers; intLayer++)
                    {
                        if(intLayer)
                        {
                            gvntSprites.last()->getFrame(intFrameIndex)->addLayer();
                        }
                        for(int intRow = 0; intRow < intHeight; intRow++)
                        {
                            QString strCurrentRowLine = vntInputStream.readLine();
                            QStringList vntCurrentRow = strCurrentRowLine.split(" ");
                            for(int intColumn = 0; intColumn < intWidth; intColumn++)
                            {
                                int intRed = vntCurrentRow.at(0 + (intColumn * 4)).toInt();
                                int intGreen = vntCurrentRow.at(1 + (intColumn * 4)).toInt();
                                int intBlue = vntCurrentRow.at(2 + (intColumn * 4)).toInt();
                                int intAlpha = vntCurrentRow.at(3 + (intColumn * 4)).toInt();
                                QColor vntCellColor(intRed, intGreen, intBlue, intAlpha);
                                gvntSprites.last()->getFrame(intFrameIndex)->
                                        drawOnLayerFromFile(intLayer, intRow, intColumn, vntCellColor);
                            }
                        }
                    }

                }
            }
            catch(const std::exception& ex){
                Q_UNUSED(ex);
                QMessageBox::information(&gvntEditorWindow, "Error opening file",
                                         "File does not have the correct format");
                return;
            }
        }

        refreshCanvas();
        emit updateFrameManager(gvntSprites[gintCurrentTab]->getFrameList(), gintCurrentFrame);
        emit updateLayerManager(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList(), gintCurrentLayer);
    }
}

// Creates a New Editor Tab from a file
void Controller::newEditorTabFromFile(bool pblnChecked, int pintSize, QString pstrName)
{
    Q_UNUSED(pblnChecked)

    gvntSprites.append(new Sprite(pstrName, pintSize));
    gintCurrentFrame = 0;
    gintCurrentLayer = 0;

    QWidget* vntWidget = createWidgetForTab(gvntSprites.last()->getFrame(gintCurrentFrame)->
                                            getPixmap(true, gintCurrentLayer, gblnTileTool, gintTileSize));

    QStringList vntFileDirectories = pstrName.split("/");
    pstrName = vntFileDirectories.last();

    emit addEditorTab(vntWidget, pstrName);
    tabChanged(gvntSprites.size() - 1);
    if(gvntSprites.count() == 2)
    {
        emit setTabsClosable(true);
    }
}

// Creates a new editor tab using informaiotn the user provides for the size
void Controller::newEditorTab(bool pblnChecked)
{
    Q_UNUSED(pblnChecked)

    QStringList vntStringList;
    vntStringList << "16 x 16" << "32 x 32" << "64 x 64" << "128 x 128" << "256 x 256";

    bool blnStatus;
    QString strSelectedItem = QInputDialog::getItem(&gvntEditorWindow, "New Sprite", "Sprite Size: ",
                                                    vntStringList, 0, false, &blnStatus);
    if(blnStatus && !strSelectedItem.isEmpty())
    {
        switch(vntStringList.indexOf(strSelectedItem))
        {
           case 0:
                gvntSprites.append(new Sprite("", 16));
                break;

           case 1:
                gvntSprites.append(new Sprite("", 32));
                break;

           case 2:
                gvntSprites.append(new Sprite("", 64));
                break;

           case 3:
                gvntSprites.append(new Sprite("", 128));
                break;

           case 4:
                gvntSprites.append(new Sprite("", 256));
                break;
        }

        gintCurrentFrame = 0;
        gintCurrentLayer = 0;
        QWidget* vntWidget = createWidgetForTab(gvntSprites.last()->
                                                getFrame(gintCurrentFrame)->
                                                getPixmap(true, gintCurrentLayer, gblnTileTool, gintTileSize));
        emit addEditorTab(vntWidget, "Untitled.ssp");
        tabChanged(gvntSprites.count() - 1);
        if(gvntSprites.count() == 2)
        {
            emit setTabsClosable(true);
        }
    }
}

// User requested to close tab, verify changes and save if needed
void Controller::tabCloseRequested(int pintIndex)
{
    int intCurrentTab = gintCurrentTab;
    if(gvntSprites[pintIndex]->hasChanged())
    {
        if(intCurrentTab != pintIndex)
        {
            tabChanged(pintIndex);
        }
        QString strFileName = gvntSprites[pintIndex]->fileName();
        if(!strFileName.isEmpty())
        {
            QStringList vntFileDirectories = strFileName.split("/");
            strFileName = vntFileDirectories.last();
        }
        int intResult = fileModifiedDialog(strFileName);
        switch (intResult) {
            case QMessageBox::Save:
                saveToFile();
                break;
            case QMessageBox::Cancel:
                if(intCurrentTab != pintIndex)
                {
                    tabChanged(intCurrentTab);
                }
                return;
                break;
            default:
                break;
        }
        if(intCurrentTab != pintIndex)
        {
            tabChanged(intCurrentTab);
        }
    }
    emit removeEditorTab(pintIndex);
    gvntSprites.removeAt(pintIndex);
    if(gvntSprites.count() == 1)
    {
        emit setTabsClosable(false);
    }
    if(gintCurrentTab >= pintIndex) {
        tabChanged(gintCurrentTab - 1);
    }
}

// User has clicked on canvas, use the selected tool to make modifications to the Sprite
void Controller::canvasClicked(CanvasLabel* pvntLabel, QPoint pvntPoint)
{
    // fixes(?) mirror edge bug while also enabling drawing with big pens just outside edge of canvas
    int scale = gvntSprites[gintCurrentTab]->getFrame(0)->getFrameScale();
    if (pvntPoint.x() < -scale * gintPenSize || pvntPoint.y() < -scale * gintPenSize)
    {
        return;
    }
    if(gblnPenTool)
    {
        gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->drawOnLayer(gintCurrentLayer, gintPenSize, pvntPoint, gvntPenColor);
    }
    else if(gblnFlipTool && (gblnHorizontalFlip || gblnVerticalFlip))
    {
        gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->flipLayer(gintCurrentLayer, gblnHorizontalFlip, gblnVerticalFlip);
    }
    else if(gblnMirrorTool)
    {
        for (int offset = 0; offset < gintPenSize * gintPenSize; offset++) {

            // stores point offset information
            QPoint vntOffsetPoint;
            vntOffsetPoint.setX(scale * (offset / gintPenSize));
            vntOffsetPoint.setY(scale * (offset % gintPenSize));

            // the normal drawing point
            QPoint vntNormalPoint;
            vntNormalPoint.setX(pvntPoint.x() + vntOffsetPoint.x());
            vntNormalPoint.setY(pvntPoint.y() + vntOffsetPoint.y());

            // the mirror of the normal point
            QPoint vntMirrorPoint;
            int coord;
            coord = pvntPoint.x() + vntOffsetPoint.x();
            vntMirrorPoint.setX(gblnHorizontalMirror ? 512 - coord : coord);
            coord = pvntPoint.y() + vntOffsetPoint.y();
            vntMirrorPoint.setY(gblnVerticalMirror ? 512 - coord : coord);

            // draw the two points (partial pens)
            gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->drawOnLayer(gintCurrentLayer, 1, vntNormalPoint, gvntPenColor);
            gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->drawOnLayer(gintCurrentLayer, 1, vntMirrorPoint, gvntPenColor);
        }
    }
    else if(gblnEraserTool)
    {
        gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->drawOnLayer(gintCurrentLayer, gintEraserSize, pvntPoint, Qt::transparent);
    }
    else if (gblnTileTool)
    {
        for (int i = 1; i <= gintTileSize; i++)
        {
            for (int j = 1; j <= gintTileSize; j++)
            {
                int intNewX = pvntPoint.x() + 512 / gintTileSize * i;
                int intNewY = pvntPoint.y() + 512 / gintTileSize * j;
                if (intNewX > 512)
                {
                    intNewX -= 512;
                }
                if (intNewY > 512)
                {
                    intNewY -= 512;
                }
                gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->drawOnLayer(gintCurrentLayer, gintPenSize, QPoint(intNewX, intNewY), gvntPenColor);
            }
        }
    }
    else if (gblnFillTool)
    {
        gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->fillLayer(gintCurrentLayer, gvntPenColor);
    }

    pvntLabel->setPixmap(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getPixmap(true, gintCurrentLayer, gblnTileTool, gintTileSize));
    emit updateSelectedFrame(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getPixmap());
    refreshPreviewWindow();
}

// Refresh the canvas with the current frame and layer
void Controller::refreshCanvas()
{
    QWidget* vntWidget = createWidgetForTab(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getPixmap(true, gintCurrentLayer, gblnTileTool, gintTileSize));
    emit replaceTab(gintCurrentTab, vntWidget);
}

// User selected the pen button on the tools window
void Controller::penButtonPressed()
{
    if(gblnPenTool)
    {
        emit updatePenButton(false);
        return;
    }
    gblnPenTool = true;
    gblnMirrorTool = false;
    gblnEraserTool = false;
    gblnTileTool = false;
    gblnFlipTool = false;
    gblnFillTool = false;
    refreshToolsWindow();
    refreshCanvas();
}

// User selected the mirror button on the tools window
void Controller::mirrorButtonPressed()
{
    if(gblnMirrorTool)
    {
        emit updateMirrorButton(false);
        return;
    }
    gblnPenTool = false;
    gblnMirrorTool = true;
    gblnEraserTool = false;
    gblnTileTool = false;
    gblnFlipTool = false;
    gblnFillTool = false;
    refreshToolsWindow();
    refreshCanvas();
}

// User selected the flip button on the tools window
void Controller::flipButtonPressed()
{
    if(gblnFlipTool)
    {
        emit updateFlipButton(false);
        return;
    }
    gblnPenTool = false;
    gblnMirrorTool = false;
    gblnEraserTool = false;
    gblnTileTool = false;
    gblnFlipTool = true;
    gblnFillTool = false;
    refreshToolsWindow();
    refreshCanvas();
}

// User selected the eraser button on the tools window
void Controller::eraserButtonPressed()
{
    if(gblnEraserTool)
    {
        emit updateEraserButton(false);
        return;
    }
    gblnPenTool = false;
    gblnMirrorTool = false;
    gblnEraserTool = true;
    gblnTileTool = false;
    gblnFlipTool = false;
    gblnFillTool = false;
    refreshToolsWindow();
    refreshCanvas();
}

// User selected the tile button on the tools window
void Controller::tileButtonPressed()
{
    if(gblnTileTool)
    {
        emit updateTileButton(false);
        return;
    }
    gblnPenTool = false;
    gblnMirrorTool = false;
    gblnEraserTool = false;
    gblnTileTool = true;
    gblnFlipTool = false;
    gblnFillTool = false;
    refreshToolsWindow();
    refreshCanvas();
}

// User selected the fill button on the tools window
void Controller::fillButtonPressed()
{
    if(gblnFillTool)
    {
        emit updateFillButton(false);
        return;
    }
    gblnPenTool = false;
    gblnMirrorTool = false;
    gblnEraserTool = false;
    gblnTileTool = false;
    gblnFlipTool = false;
    gblnFillTool = true;
    refreshToolsWindow();
    refreshCanvas();
}

// User selected the color button on the tools window
void Controller::colorButtonPressed()
{
    QColor color = QColorDialog::getColor(gvntPenColor, &gvntEditorWindow, "", QColorDialog::ShowAlphaChannel);
    if(color.isValid())
    {
        gvntPenColor = color;
        emit updateColorDisplay(gvntPenColor);
    }
}

// User selected the horizontal flip check box on the tools window
void Controller::horizontalFlipCheckboxPressed()
{
    gblnHorizontalFlip = !gblnHorizontalFlip;
}

// User selected the vertical flip check box on the tools window
void Controller::verticalFlipCheckboxPressed()
{
    gblnVerticalFlip = !gblnVerticalFlip;
}

// User selected the horizontal mirror check box on the tools window
void Controller::horizontalMirrorCheckboxPressed()
{
    gblnHorizontalMirror = !gblnHorizontalMirror;
}

// User selected the vertical mirror check box on the tools window
void Controller::verticalMirrorCheckboxPressed()
{
    gblnVerticalMirror = !gblnVerticalMirror;
}

// User changed the pen size on the tools window
void Controller::changePenSize(int pintSize)
{
    gintPenSize = pintSize;
}

// User changed the eraser size on the tools window
void Controller::changeEraserSize(int pintSize)
{
    gintEraserSize = pintSize;
}

// User changed the tile size on the tools window
void Controller::changeTileSize(int pintSize)
{
    gintTileSize = 1 << pintSize;
    refreshCanvas();
}

// Refresh the tools window selected buttons
void Controller::refreshToolsWindow()
{
    if(!gblnPenTool)
    {
        emit updatePenButton(gblnPenTool);
    }
    if(!gblnMirrorTool)
    {
        emit updateMirrorButton(gblnMirrorTool);
    }
    if(!gblnEraserTool)
    {
        emit updateEraserButton(gblnEraserTool);
    }
    if(!gblnTileTool)
    {
        emit updateTileButton(gblnTileTool);
    }
    if(!gblnFlipTool)
    {
        emit updateFlipButton(gblnFlipTool);
    }
    if(!gblnFillTool)
    {
        emit updateFillButton(gblnFillTool);
    }
}

// User requested a new frame for the current sprite
void Controller::addNewFrame()
{
    // Tell Sprite to add a new SpriteFrame
    gvntSprites[gintCurrentTab]->addFrame();
    gintCurrentLayer = 0;
    gintCurrentFrame = gvntSprites[gintCurrentTab]->getFrameList()->rowCount() - 1;

    // refresh
    emit updateLayerManager(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList(), gintCurrentLayer);
    emit updateFrameManager(gvntSprites[gintCurrentTab]->getFrameList(), gintCurrentFrame);
    refreshPreviewWindow();
    refreshCanvas();
}

// User requested to delete the current frame
void Controller::deleteFrame()
{
    int frameCount = gvntSprites[gintCurrentTab]->getFrameList()->rowCount();

    if(frameCount == 1)
    {
        gvntSprites[gintCurrentTab]->addFrame();
        frameCount++;
    }

    // and the old one gets deleted anyway
    gvntSprites[gintCurrentTab]->deleteFrame(gintCurrentFrame);
    frameCount--;

    // move currentFrame index if necessary
    if (gintCurrentFrame > frameCount - 1)
    {
        gintCurrentFrame = frameCount - 1;
    }

    gintCurrentLayer = 0;
    // if there's only one frame, add a blank one so that there's always a frame
    emit updateLayerManager(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList(), gintCurrentLayer);
    emit updateFrameManager(gvntSprites[gintCurrentTab]->getFrameList(), gintCurrentFrame);
    refreshPreviewWindow();
    refreshCanvas();
}

// User requested to duplicate the current frame
void Controller::duplicateFrame()
{
    gvntSprites[gintCurrentTab]->copyFrame(gintCurrentFrame);
    gintCurrentLayer = 0;
    gintCurrentFrame = gvntSprites[gintCurrentTab]->getFrameList()->rowCount() - 1;

    // refresh
    emit updateLayerManager(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList(), gintCurrentLayer);
    emit updateFrameManager(gvntSprites[gintCurrentTab]->getFrameList(), gintCurrentFrame);
    refreshPreviewWindow();
    refreshCanvas();
}

// A diffrent frame was selected in the frame manager
void Controller::frameSelected(QModelIndex pvntIndex)
{
    gintCurrentFrame = pvntIndex.row();
    gintCurrentLayer = 0;
    emit updateLayerManager(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList(), gintCurrentLayer);
    refreshPreviewWindow();
	refreshCanvas();
}

// User requested a new layer on the current frame
void Controller::addLayer()
{
    // Tell SpriteFrame to Add New Layer
    gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->addLayer();
    gintCurrentLayer = gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList()->rowCount() - 1;

    // refresh layer manager
    emit updateLayerManager(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList(), gintCurrentLayer);
    refreshCanvas();
}

// User requested to delete the currently selected layer
void Controller::deleteLayer()
{
    int frameCount = gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList()->rowCount();

    if(frameCount == 1)
    {
        gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->addLayer();
        frameCount++;
    }
    // Tell SpriteFrame to Delete Layer
    gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->deleteLayer(gintCurrentLayer);
    frameCount--;

    if (gintCurrentLayer > frameCount - 1)
    {
        gintCurrentLayer = frameCount - 1;
    }

    // refresh layer manager
    emit updateLayerManager(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList(), gintCurrentLayer);
    emit updateFrameManager(gvntSprites[gintCurrentTab]->getFrameList(), gintCurrentFrame);
    refreshPreviewWindow();
    refreshCanvas();
}

// User requested to duplicate the current layer
void Controller::duplicateLayer()
{
    gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->copyLayer(gintCurrentLayer);
    gintCurrentLayer = gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList()->rowCount() - 1;

    // refresh layer manager
    emit updateLayerManager(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList(), gintCurrentLayer);
    refreshCanvas();
}

// User requested to combine the selected layer with another layer
void Controller::combineLayers()
{
    LayerList* vntLayers = gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList();
    QStringList vntStringList;
    for(int intLayers = 0; intLayers < vntLayers->rowCount(); intLayers++)
    {
        if(intLayers != gintCurrentLayer)
        {
            vntStringList << vntLayers->data(vntLayers->index(intLayers)).value<QString>();
        }
    }

    bool blnStatus;
    QString strSelectedItem = QInputDialog::getItem(&gvntEditorWindow, "Combine Layers", "Combine with: ",
                                                    vntStringList, 0, false, &blnStatus);
    if(blnStatus && !strSelectedItem.isEmpty())
    {
        if(vntStringList.indexOf(strSelectedItem) >= gintCurrentLayer)
        {
            gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->combineLayers(gintCurrentLayer, vntStringList.indexOf(strSelectedItem) + 1);
        }
        else
        {
            gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->combineLayers(vntStringList.indexOf(strSelectedItem), gintCurrentLayer);
            gintCurrentLayer = vntStringList.indexOf(strSelectedItem);
        }
    }

    // refresh layer manager
    emit updateLayerManager(gvntSprites[gintCurrentTab]->getFrame(gintCurrentFrame)->getLayerList(), gintCurrentLayer);
    refreshCanvas();
}

// A different layer was selected by the user
void Controller::selectLayer(QModelIndex pvntIndex)
{
    gintCurrentLayer = pvntIndex.row();
    refreshCanvas();
}

// The user adjusted the preview speed on the preview window
void Controller::previewSpeed(int pintFramesPerSecond)
{
    gintFramesPerSecond = 1000 / pintFramesPerSecond;
    gvntTimer.setInterval(gintFramesPerSecond);
}

// The user paused or played the preview
void Controller::playPauseButtonPressed()
{
    if(gvntTimer.isActive())
    {
        gvntTimer.stop();
        emit updatePlayPauseButton("Play");
        refreshPreviewWindow();
    }
    else
    {
        gvntTimer.start(gintFramesPerSecond);
        gintPreviewFrame = gintCurrentFrame;
        emit updatePlayPauseButton("Pause");
    }
}

// Updates the preview window with the next frame in the Sprite
void Controller::cyclePreviewImage()
{
    gintPreviewFrame++;
    if(gintPreviewFrame == gvntSprites[gintCurrentTab]->getFrameList()->rowCount())
    {
        gintPreviewFrame = 0;
    }
    refreshPreviewWindow();
}

// The user selected the actual size checkbox on the preview window
void Controller::actualSizeCheckboxPressed()
{
    gblnAcutalSize = !gblnAcutalSize;
    refreshPreviewWindow();
}

// Refreshes the preview window with the current sprite information
void Controller::refreshPreviewWindow()
{
    int intSize = 256;
    int intFrame = gintPreviewFrame;
    if(gblnAcutalSize)
    {
        intSize = gvntSprites[gintCurrentTab]->getSize();
    }
    if(!gvntTimer.isActive())
    {
        intFrame = gintCurrentFrame;
    }
    emit updatePreviewImage(gvntSprites[gintCurrentTab]->getFrame(intFrame)->getPixmap(), QSize(intSize, intSize));
}

// Creates a Canvas Layer and parent QWidget for the editor windows tabs
QWidget* Controller::createWidgetForTab(QPixmap pvntPixmap)
{
    QWidget* vntWidget = new QWidget;
    CanvasLabel* vntLabel = new CanvasLabel;
    vntLabel->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    vntLabel->setContentsMargins(0, 0, 0, 0);
    vntLabel->setPixmap(pvntPixmap);
    vntLabel->setMinimumSize(512, 512);
    vntLabel->setMaximumSize(512, 512);
    vntLabel->setStyleSheet("border: 2px solid");
    QObject::connect(vntLabel, &CanvasLabel::clicked, this, &Controller::canvasClicked);

    QGridLayout *vntLayout = new QGridLayout;
    vntLayout->setContentsMargins(0, 0, 0, 10);
    vntWidget->setLayout(vntLayout);
    vntLayout->addWidget(vntLabel);

    return vntWidget;
}

// Sets the connections for the editor window
void Controller::setupEditorWindowConnections()
{
    QObject::connect(&gvntEditorWindow, &EditorWindow::tabCloseRequested, this, &Controller::tabCloseRequested);
    QObject::connect(&gvntEditorWindow, &EditorWindow::quitRequested, this, &Controller::quitApplication);
    QObject::connect(&gvntEditorWindow, &EditorWindow::newTabRequested, this, &Controller::newEditorTab);
    QObject::connect(&gvntEditorWindow, &EditorWindow::saveRequested, this, &Controller::saveToFile);
    QObject::connect(&gvntEditorWindow, &EditorWindow::saveAsRequested, this, &Controller::saveAsToFile);
    QObject::connect(&gvntEditorWindow, &EditorWindow::exportRequested, this, &Controller::exportAsGIF);
    QObject::connect(&gvntEditorWindow, &EditorWindow::openRequested, this, &Controller::openFile);
    QObject::connect(&gvntEditorWindow, &EditorWindow::tabChanged, this, &Controller::tabChanged);
    QObject::connect(&gvntEditorWindow, &EditorWindow::frameManagerRequested, this, &Controller::focusOnFrameManager);
    QObject::connect(&gvntEditorWindow, &EditorWindow::layerManagerRequested, this, &Controller::focusOnLayerManager);
    QObject::connect(&gvntEditorWindow, &EditorWindow::toolsRequested, this, &Controller::focusOnTools);
    QObject::connect(&gvntEditorWindow, &EditorWindow::previewRequested, this, &Controller::focusOnPreview);

    QObject::connect(this, &Controller::addEditorTab, &gvntEditorWindow, &EditorWindow::addTab);
    QObject::connect(this, &Controller::removeEditorTab, &gvntEditorWindow, &EditorWindow::closeTab);
    QObject::connect(this, &Controller::setTabsClosable, &gvntEditorWindow, &EditorWindow::setTabsClosable);
    QObject::connect(this, &Controller::replaceTab, &gvntEditorWindow, &EditorWindow::replaceTab);
    QObject::connect(this, &Controller::renameTab, &gvntEditorWindow, &EditorWindow::renameTab);
    QObject::connect(this, &Controller::updateFrameManagerCheckbox, &gvntEditorWindow, &EditorWindow::setFrameManagerVisible);
    QObject::connect(this, &Controller::updateLayerManagerCheckbox, &gvntEditorWindow, &EditorWindow::setLayerManagerVisible);
    QObject::connect(this, &Controller::updateToolsCheckbox, &gvntEditorWindow, &EditorWindow::setToolsVisible);
    QObject::connect(this, &Controller::updatePreviewCheckbox, &gvntEditorWindow, &EditorWindow::setPreviewVisible);
}

// Sets the connections for the Frame Manager
void Controller::setupFrameManagerConnections()
{
    QObject::connect(&gvntFrameManager, &FrameManagerWidget::addButtonClicked, this, &Controller::addNewFrame);
    QObject::connect(&gvntFrameManager, &FrameManagerWidget::deleteButtonClicked, this, &Controller::deleteFrame);
    QObject::connect(&gvntFrameManager, &FrameManagerWidget::frameDupButtonClicked, this, &Controller::duplicateFrame);
    QObject::connect(&gvntFrameManager, &FrameManagerWidget::frameClicked, this, &Controller::frameSelected);
    QObject::connect(&gvntFrameManager, &FrameManagerWidget::visibilityChanged, this, &Controller::frameManagerVisibility);

    QObject::connect(this, &Controller::updateFrameManager, &gvntFrameManager, &FrameManagerWidget::updateListView);
    QObject::connect(this, &Controller::updateSelectedFrame, &gvntFrameManager, &FrameManagerWidget::updateFrame);
}

// Sets the connections for the layer manager
void Controller::setupLayerManagerConnections()
{
    QObject::connect(&gvntLayerManager, &LayerManagerWidget::addButtonClicked, this, &Controller::addLayer);
    QObject::connect(&gvntLayerManager, &LayerManagerWidget::deleteButtonClicked, this, &Controller::deleteLayer);
    QObject::connect(&gvntLayerManager, &LayerManagerWidget::duplicateButtonClicked, this, &Controller::duplicateLayer);
    QObject::connect(&gvntLayerManager, &LayerManagerWidget::combineButtonClicked, this, &Controller::combineLayers);
    QObject::connect(&gvntLayerManager, &LayerManagerWidget::visibilityChanged, this, &Controller::layerManagerVisibility);
    QObject::connect(&gvntLayerManager, &LayerManagerWidget::layerSelected, this, &Controller::selectLayer);

    QObject::connect(this, &Controller::updateLayerManager, &gvntLayerManager, &LayerManagerWidget::updateListView);
}

// Sets the connections for the preview window
void Controller::setupPreviewConnections()
{
    QObject::connect(&gvntPreview, &PreviewWidget::visibilityChanged, this, &Controller::previewVisibility);
    QObject::connect(&gvntPreview, &PreviewWidget::framesPerSecondValueChanged, this, &Controller::previewSpeed);
    QObject::connect(&gvntPreview, &PreviewWidget::playpauseButtonClicked, this, &Controller::playPauseButtonPressed);
    QObject::connect(&gvntPreview, &PreviewWidget::actualSizeClicked, this, &Controller::actualSizeCheckboxPressed);

    QObject::connect(this, &Controller::updatePlayPauseButton, &gvntPreview, &PreviewWidget::changePlayPauseButton);
    QObject::connect(this, &Controller::updatePreviewImage, &gvntPreview, &PreviewWidget::updateImage);
}

// Sets the connections for the tools window
void Controller::setupToolsConnections()
{
    QObject::connect(&gvntTools, &ToolsWidget::penButtonClicked, this, &Controller::penButtonPressed);
    QObject::connect(&gvntTools, &ToolsWidget::mirrorButtonClicked, this, &Controller::mirrorButtonPressed);
    QObject::connect(&gvntTools, &ToolsWidget::flipButtonClicked, this, &Controller::flipButtonPressed);
    QObject::connect(&gvntTools, &ToolsWidget::eraserButtonClicked, this, &Controller::eraserButtonPressed);
    QObject::connect(&gvntTools, &ToolsWidget::tileButtonClicked, this, &Controller::tileButtonPressed);
    QObject::connect(&gvntTools, &ToolsWidget::fillButtonClicked, this, &Controller::fillButtonPressed);
    QObject::connect(&gvntTools, &ToolsWidget::colorButtonClicked, this, &Controller::colorButtonPressed);
    QObject::connect(&gvntTools, &ToolsWidget::penSliderValueChanged, this, &Controller::changePenSize);
    QObject::connect(&gvntTools, &ToolsWidget::eraserSliderValueChanged, this, &Controller::changeEraserSize);
    QObject::connect(&gvntTools, &ToolsWidget::tileSliderValueChanged, this, &Controller::changeTileSize);
    QObject::connect(&gvntTools, &ToolsWidget::horizontalFlipClicked, this, &Controller::horizontalFlipCheckboxPressed);
    QObject::connect(&gvntTools, &ToolsWidget::verticalFlipClicked, this, &Controller::verticalFlipCheckboxPressed);
    QObject::connect(&gvntTools, &ToolsWidget::horizontalMirrorClicked, this, &Controller::horizontalMirrorCheckboxPressed);
    QObject::connect(&gvntTools, &ToolsWidget::verticalMirrorClicked, this, &Controller::verticalMirrorCheckboxPressed);
    QObject::connect(&gvntTools, &ToolsWidget::visibilityChanged, this, &Controller::toolsVisibility);

    QObject::connect(this, &Controller::updateColorDisplay, &gvntTools, &ToolsWidget::updateColor);
    QObject::connect(this, &Controller::updatePenButton, &gvntTools, &ToolsWidget::changePenButton);
    QObject::connect(this, &Controller::updateMirrorButton, &gvntTools, &ToolsWidget::changeMirrorButton);
    QObject::connect(this, &Controller::updateEraserButton, &gvntTools, &ToolsWidget::changeEraserButton);
    QObject::connect(this, &Controller::updateTileButton, &gvntTools, &ToolsWidget::changeTileButton);
    QObject::connect(this, &Controller::updateFlipButton, &gvntTools, &ToolsWidget::changeFlipButton);
    QObject::connect(this, &Controller::updateFillButton, &gvntTools, &ToolsWidget::changeFillButton);
}
