#include "spriteframe.h"

/*
 * Sprite Frame that contains the layers and methods that represent a frame within a Sprite.
 */
SpriteFrame::SpriteFrame(int pintSize)
{
    gintFrameScale = 512 / pintSize;
    gvntLayers.insertRows(0, 1);
    QImage vntImage(pintSize, pintSize, QImage::Format_ARGB32);
    vntImage.fill(Qt::transparent);
    gvntLayers.setData(gvntLayers.index(0), vntImage, Qt::DecorationRole);
    gblnModified = false;
    gintFrameSize = pintSize;
}

SpriteFrame::~SpriteFrame()
{
}
// Returns a pointer to the contained list of layers
LayerList* SpriteFrame::getLayerList()
{
    return &gvntLayers;
}

// Adds a new blank layer to the list of layers
void SpriteFrame::addLayer()
{
    gvntLayers.insertRows(gvntLayers.rowCount(), 1);
    QImage vntImage(gintFrameSize, gintFrameSize, QImage::Format_ARGB32);
    vntImage.fill(Qt::transparent);
    gvntLayers.setData(gvntLayers.index(gvntLayers.rowCount() - 1), vntImage, Qt::DecorationRole);
    gblnModified = true;
}

// Adds a new layer that matches the image that is passed in.
void SpriteFrame::addLayer(QImage pvntImage)
{
    gvntLayers.insertRows(gvntLayers.rowCount(), 1);
    gvntLayers.setData(gvntLayers.index(gvntLayers.rowCount() - 1), pvntImage, Qt::DecorationRole);
    gblnModified = true;
}

// Adds a new layer which is identical to another layer.
void SpriteFrame::copyLayer(int pintLayer)
{
    gvntLayers.insertRows(gvntLayers.rowCount(), 1);
    QImage vntImage = gvntLayers.data(gvntLayers.index(pintLayer), Qt::DecorationRole).value<QImage>().copy();
    gvntLayers.setData(gvntLayers.index(gvntLayers.rowCount() - 1), vntImage, Qt::DecorationRole);
    gblnModified = true;
}

// Removes a given layer from the specified index. index is defined by pintLayer.
void SpriteFrame::deleteLayer(int pintLayer)
{
    gvntLayers.removeRows(pintLayer, 1);
    gblnModified = true;
}

// Mirrors the layer pintLayer horizontally, vertically, or both, based on th boolean values passed in. True means a mirror occurs.
void SpriteFrame::flipLayer(int pintLayer, bool pblnHorizontally, bool pblnVertically)
{
    QImage vntImage = gvntLayers.data(gvntLayers.index(pintLayer), Qt::DecorationRole).value<QImage>();
    vntImage = vntImage.mirrored(pblnHorizontally, pblnVertically);
    gvntLayers.setData(gvntLayers.index(pintLayer), vntImage, Qt::DecorationRole);
    gblnModified = true;
}

// Fills a specified layer (pintLayer) with a designated color (pvntColor).
void SpriteFrame::fillLayer(int pintLayer, QColor pvntColor)
{
    QImage vntImage = gvntLayers.data(gvntLayers.index(pintLayer), Qt::DecorationRole).value<QImage>();
    vntImage.fill(pvntColor);
    gvntLayers.setData(gvntLayers.index(pintLayer), vntImage, Qt::DecorationRole);
    gblnModified = true;
}

// Draws on a layer.
void SpriteFrame::drawOnLayer(int pintLayer, int pintSize, QPoint pvntPoint, QColor pvntColor)
{
    QImage vntImage = gvntLayers.data(gvntLayers.index(pintLayer), Qt::DecorationRole).value<QImage>();
    int intXStart = ((pvntPoint.x() - 1) / gintFrameScale);
    int intYStart = ((pvntPoint.y() - 1) / gintFrameScale);
    for(int intX = intXStart; intX < intXStart + pintSize; intX++)
    {
        for(int intY = intYStart; intY < intYStart + pintSize; intY++)
        {
            if (vntImage.valid(intX, intY))
            {
                vntImage.setPixel(intX, intY, pvntColor.rgba());
            }
        }
    }
    gvntLayers.setData(gvntLayers.index(pintLayer), vntImage, Qt::DecorationRole);
    gblnModified = true;
}

// Draws on a layer using file informatnion
void SpriteFrame::drawOnLayerFromFile(int pintLayer, int pintX, int pintY, QColor pvntColor)
{
    QImage vntImage = gvntLayers.data(gvntLayers.index(pintLayer), Qt::DecorationRole).value<QImage>();
    if (vntImage.valid(pintX, pintY))
    {
        vntImage.setPixel(pintX, pintY, pvntColor.rgba());
    }
    gvntLayers.setData(gvntLayers.index(pintLayer), vntImage, Qt::DecorationRole);
}

// Retrieves the PixMap of the  frame.
QPixmap SpriteFrame::getPixmap(bool pblnIncludeGrid, int pintLayer, bool pblnIncludeTileGrid, int pintTileSize)
{
    QPixmap vntPixmap(512, 512);
    vntPixmap.fill(Qt::transparent);
    QPainter painter(&vntPixmap);

    if(pintLayer == -1)
    {
        for(int intLayer = gvntLayers.rowCount() - 1; intLayer >= 0; intLayer--)
        {
            QImage vntTempImage = gvntLayers.data(gvntLayers.index(intLayer), Qt::DecorationRole).value<QImage>();
            QPixmap vntTempPixmap(vntPixmap.width(), vntPixmap.height());
            vntTempPixmap.convertFromImage(vntTempImage);
            painter.drawPixmap(0, 0, vntPixmap.width(), vntPixmap.height(), vntTempPixmap);
        }
    }
    else
    {
        QImage vntTempImage = gvntLayers.data(gvntLayers.index(pintLayer), Qt::DecorationRole).value<QImage>();
        QPixmap vntTempPixmap(vntPixmap.width(), vntPixmap.height());
        vntTempPixmap.convertFromImage(vntTempImage);
        painter.drawPixmap(0, 0, vntPixmap.width(), vntPixmap.height(), vntTempPixmap);
    }

    if(pblnIncludeGrid)
    {
        drawSpriteGrid(&painter, QColor(200, 200, 200, 255), gintFrameScale, vntPixmap.width(), vntPixmap.height());
    }

    if(pblnIncludeTileGrid && pintTileSize > 0)
    {
        drawSpriteGrid(&painter, QColor(0, 0, 255, 255), 512 / pintTileSize, vntPixmap.width(), vntPixmap.height());
    }
    painter.end();
    return vntPixmap;
}

// Returns this sprite frame as an image
QImage SpriteFrame::getImage()
{
    // load highest layer
    QImage vntImage = gvntLayers.data(gvntLayers.index(gvntLayers.rowCount() - 1), Qt::DecorationRole).value<QImage>();
    QPixmap vntPixmap(vntImage.width(), vntImage.height());
    vntPixmap.convertFromImage(vntImage);
    QPainter painter(&vntPixmap);

    for(int intLayer = gvntLayers.rowCount() - 2; intLayer >= 0; intLayer--)
    {
        QImage vntTempImage = gvntLayers.data(gvntLayers.index(intLayer), Qt::DecorationRole).value<QImage>();
        QPixmap vntTempPixmap(vntPixmap.width(), vntPixmap.height());
        vntTempPixmap.convertFromImage(vntTempImage);
        painter.drawPixmap(0, 0, vntPixmap.width(), vntPixmap.height(), vntTempPixmap);
    }

    return vntPixmap.toImage();
}

// Draws the grid for the Canvas.
void SpriteFrame::drawSpriteGrid(QPainter* pvntPainter, QColor pvntColor, int pintGridSize, int pintWidth, int pintHeight)
{
    pvntPainter->setPen(pvntColor);
    for(int x = pintGridSize; x < pintWidth; x += pintGridSize)
    {
        pvntPainter->drawLine(x, 0, x, pintHeight);
    }
    for(int y = pintGridSize; y < pintHeight; y += pintGridSize)
    {
        pvntPainter->drawLine(0, y, pintWidth, y);
    }
}

// Combines the two layers on this sprite frame
void SpriteFrame::combineLayers(int pintLayer1, int pintLayer2)
{
    // load layer 1
    QImage vntLayer1Image = gvntLayers.data(gvntLayers.index(pintLayer1), Qt::DecorationRole).value<QImage>();
    QPixmap vntLayer1Pixmap(vntLayer1Image.width(), vntLayer1Image.height());
    vntLayer1Pixmap.convertFromImage(vntLayer1Image);

    // load layer 2
    QImage vntLayer2Image = gvntLayers.data(gvntLayers.index(pintLayer2), Qt::DecorationRole).value<QImage>();
    QPixmap vntLayer2Pixmap(vntLayer2Image.width(), vntLayer2Image.height());
    vntLayer2Pixmap.convertFromImage(vntLayer2Image);

    // assign painter to image 1 and draw image 2 on top
    QPainter painter(&vntLayer1Pixmap);
    painter.drawPixmap(0, 0, vntLayer1Pixmap.width(), vntLayer1Pixmap.height(), vntLayer2Pixmap);

    // convert to image and set the data over layer 1
    QImage vntImage = vntLayer1Pixmap.toImage();
    gvntLayers.setData(gvntLayers.index(pintLayer1), vntImage, Qt::DecorationRole);
    gvntLayers.removeRows(pintLayer2, 1);
    gblnModified = true;
}

// Returns the frame scale for this sprite frame
int SpriteFrame::getFrameScale()
{
    return gintFrameScale;
}
// Indicates file has been changed.
bool SpriteFrame::hasChanged()
{
    return gblnModified;
}
// Indicates there are no new changes on the file.
void SpriteFrame::resetModified()
{
    gblnModified = false;
}
