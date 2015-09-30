#pragma once

#include "ofMain.h"

class BackImage {
    
public:
    float   ratioSpeed; // speedにかける比率
    float   dispImgHeight; // ディスプレイを使う時の背景の高さ
    float   position; // 後ろの空の画像を動かすための座標
    float   speed; // それに伴う速度
    float   gravity; // 重力加速度
    bool    isGoal; //　ゴールしたかの判定
    
    // 画像
    ofImage img; // 背景の画像
    
    //音
    ofSoundPlayer jumpSE; // ジャンプ音
    ofSoundPlayer fallSE;
    
    //コンストラクタとデストラクタ
    BackImage();
    virtual ~BackImage();
    
    void setup();
    void init();
    void jamping(float initSpeed);
    void update();
    void draw(float x, float y);
    void goal();
    void stopper();    
};