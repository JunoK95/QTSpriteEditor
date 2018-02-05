#include "sprite.h"

/*
 * Custom Sprite object to store the information contained within the Sprite.
 */
Sprite::Sprite(QString pstrFileName, int pintSize)
{
    gstrFileName = pstrFileName;
    gintSize = pintSize;
    gvntFrames.insertRows(0, 1);
    SpriteFrame* vntSpriteFrame = new SpriteFrame(pintSize);
    gvntFrames.setData(gvntFrames.index((0)), QVariant::fromValue(vntSpriteFrame));
    gblnModified = false;
}

Sprite::~Sprite()
{
}

// returns the indexed SpriteFrame in this Sprite
SpriteFrame* Sprite::getFrame(int pintFrame)
{
    return gvntFrames.data(gvntFrames.index(pintFrame), Qt::EditRole).value<SpriteFrame*>();
}

// returns all the SpriteFrames in this Sprite
FrameList* Sprite::getFrameList()
{
    return &gvntFrames;
}

// adds a SpriteFrame to the end of the FrameList, in this Sprite
void Sprite::addFrame()
{
    gvntFrames.insertRows(gvntFrames.rowCount(), 1);
    SpriteFrame* vntSpriteFrame = new SpriteFrame(gintSize);
    gvntFrames.setData(gvntFrames.index(gvntFrames.rowCount() - 1), QVariant::fromValue(vntSpriteFrame));
    gblnModified = true;
}

// removes the indexed SpriteFrame from this Sprite
void Sprite::deleteFrame(int pintFrame)
{
    gvntFrames.removeRows(pintFrame, 1);
    gblnModified = true;
}

void Sprite::copyFrame(int pintFrame)
{
    gvntFrames.insertRows(gvntFrames.rowCount(), 1);
    SpriteFrame* vntNewFrame = new SpriteFrame(gintSize);
    LayerList* vntOldLayers = gvntFrames.data(gvntFrames.index(pintFrame), Qt::EditRole).value<SpriteFrame*>()->getLayerList();
    for (int i = 0; i < vntOldLayers->rowCount(); i++)
    {
        QImage vntImageCopy = vntOldLayers->data(vntOldLayers->index(i), Qt::DecorationRole).value<QImage>().copy();
        vntNewFrame->addLayer(vntImageCopy);
    }
    vntNewFrame->deleteLayer(0);
    gvntFrames.setData(gvntFrames.index(gvntFrames.rowCount() - 1), QVariant::fromValue(vntNewFrame));
    gblnModified = true;
}

// the size of the Sprite image(s), which is the height and width of every SpriteFrame
int Sprite::getSize()
{
    return gintSize;
}

// returns whether this Sprite has been changed since it was last saved
bool Sprite::hasChanged()
{
    if(!gblnModified)
    {
        for(int i = gvntFrames.rowCount() - 1; i >= 0; i--)
        {
            if(gvntFrames.data(gvntFrames.index(i), Qt::EditRole).value<SpriteFrame*>()->hasChanged())
            {
                gblnModified = true;
                break;
            }
        }
    }

    return gblnModified;
}

// sets the Sprite as unchanged, indicating it does not need to be saved first
void Sprite::resetModified()
{
    if(hasChanged())
    {
        for(int i = gvntFrames.rowCount() - 1; i >= 0; i--)
        {
            gvntFrames.data(gvntFrames.index(i), Qt::EditRole).value<SpriteFrame*>()->resetModified();
        }
        gblnModified = false;
    }
}

// the file name of this Sprite. An unsaved image has an empty name
QString Sprite::fileName()
{
    return gstrFileName;
}

// sets the name of the file which this Sprite was saved to
void Sprite::setFileName(QString pstrFileName)
{
    gstrFileName = pstrFileName;
}
