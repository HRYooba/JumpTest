#pragma once

#include "ofMain.h"

class Charactor {
public:
    float     charaSpeed; // キャラクタの速度
    float     charaGravity; // キャラクタの加速度
    float     charaPos; // キャラクタの座標
    float     charaCount; // キャラクタが動けるカウント(max48)
    bool      moveFlag; // isJampを保存す
    
    ofImage   img; // キャラクタの画像
    
    Charactor();
    virtual ~Charactor();
    
    void setup();
    void init();
    void copyFlag(bool flag);
    void jamping();
    void update();
    void draw(float x, float y);
};