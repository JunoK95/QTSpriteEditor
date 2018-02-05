#ifndef SPRITE_H
#define SPRITE_H

#include "framelist.h"

/*
 * Custom Sprite object to store the information contained within the Sprite.
 */
class Sprite
{
public:
    Sprite(QString pstrFileName, int pintSize);
    ~Sprite();
    SpriteFrame* getFrame(int pintFrame);
    FrameList* getFrameList();
    void addFrame();
    void deleteFrame(int pintFrame);
    void copyFrame(int pintFrame);
    bool hasChanged();
    void resetModified();
    QString fileName();
    void setFileName(QString pstrFileName);
    int getSize();

private:
    FrameList gvntFrames;
    QString gstrFileName;
    bool gblnModified;
    int gintSize;
};

#endif // SPRITE_H
