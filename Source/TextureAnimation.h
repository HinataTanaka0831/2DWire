#pragma once
#pragma once
#include "DxLib.h"
#include <string>

class TextureAnimation
{
public:
    // Initialize animation by loading sprite sheet and splitting frames
    TextureAnimation(
        std::string filename,
        VECTOR initPos,
        int allNum,
        int NumX,
        int NumY,
        int interval,
        float scale = 1.0f,
        bool type = true
    );

    // Destructor releases allocated handles
    ~TextureAnimation();

    // Set enemy flip flag (used for enemy animations)
    void SetReverse(bool rev) { mbEnemyReverseX = rev;}

    // Update animation state and player/enemy flip flags
    void Update();

    // Draw current frame with optional horizontal flip
    void Draw(float cameraX, float cameraY);

    // Reset animation to first frame
    void Reset();

    // Position setter
    void SetPosition(const VECTOR& pos) { mvPosition = pos; }

    float GetRadius() { return mfRadius; }

    // Member variables
    VECTOR mvPosition;
    int mnCounter = 0;
    int mnInterval;
    int mnCurrentNum = 0;
    int* mnHandleList = nullptr;
    int mnAllNum;
    int mnSizeX;
    int mnSizeY;
    float mfRadius = 60.0f;
    float mnScale;
    bool mbPlayerReverseX = false;
    bool mbEnemyReverseX = false;
    bool mbType;

};
