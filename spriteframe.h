#ifndef SPRITEFRAME_H
#define SPRITEFRAME_H

#include <QPixmap>
#include <QPainter>
#include <QImage>
#include "layerlist.h"

/*
 * Sprite Frame that contains the layers and methods that represent a frame within a Sprite.
 */
class SpriteFrame
{
public:
    SpriteFrame(int pintSize);
    ~SpriteFrame();
    QPixmap getPixmap(bool pblnIncludeGrid = false, int pintLayer = -1, bool pblnIncludeTileGrid = false, int pintTileSize = 0);
    void drawOnLayer(int pintLayer, int pintSize, QPoint pvntPoint, QColor pvntColor);
    void drawOnLayerFromFile(int pintLayer, int pintX, int pintY, QColor pvntColor);
    void flipLayer(int pintLayer, bool pblnHorizontally, bool pblnVertically);
    void fillLayer(int pintLayer, QColor pvntColor);
    bool hasChanged();
    void resetModified();
    LayerList* getLayerList();
    void addLayer();
    void addLayer(QImage pvntImage);
    void copyLayer(int pintLayer);
    void deleteLayer(int pintLayer);
    void combineLayers(int pintLayer1, int pintLayer2);
    int getFrameScale();
    QImage getImage();

private:
    void drawSpriteGrid(QPainter* pvntPainter, QColor pvntColor, int pintGridSize, int pintWidth, int pintHeight);
    LayerList gvntLayers;
    bool gblnModified;
    int gintFrameSize;
    int gintFrameScale;
};

Q_DECLARE_METATYPE(SpriteFrame*)

#endif // SPRITEFRAME_H
