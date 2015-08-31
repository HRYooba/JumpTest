#pragma once

#include "ofMain.h"

class BackImage {
    
public:
    float   position; // 後ろの空の画像を動かすための座標
    float   speed; // それに伴う速度
    float   gravity; // 重力加速度
    float   minSpeed; // 最小の速度
    bool    moveFlag; // isJampを保存する
    
    // 画像
    ofImage img; // 背景の画像
    //音
    ofSoundPlayer jumpSE; // ジャンプ音
    
    //コンストラクタとデストラクタ
    BackImage();
    virtual ~BackImage();
    
    void setup();
    void init();
    void copyFlag(bool flag);
    void jamping();
    void update();
    void draw(float x, float y);
    void goal();
    void stopper();
    
};