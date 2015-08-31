#include "BackImage.h"

BackImage::BackImage() {
    
}

// setup
void BackImage::setup() {
    // 読み込む画像の設定
    img.loadImage("back.jpg");
    jumpSE.loadSound("jump.mp3");
    position = -img.height;
    //init();
}

// 初期化
void BackImage::init() {
    speed = 70;
    gravity = 2.0;
    minSpeed = -80.0;
    moveFlag = false;
}

// ジャンプした判定をコピーする
void BackImage::copyFlag(bool flag) {
    moveFlag = flag;
}

// ジャンプした時に呼ばれる関数
void BackImage::jamping() {
    init();
    jumpSE.play();
}

// update
void BackImage::update() {
    goal();
    if ( moveFlag ) {
        speed -= gravity;
        position += speed;
        if (speed < minSpeed) {
            moveFlag = false;
        }
        stopper();
    } else {
        speed -= gravity;
        position += speed;
        stopper();
    }
}


// ゴール
void BackImage::goal() {
    if ( position >= 0 ) {
        moveFlag = false;
        position = -img.height;
    }
}

// 最地点の設定
void BackImage::stopper() {
    if ( position <= -img.height ) {
        position = -img.height;
    }
}


// draw
void BackImage::draw(float x, float y) {
    img.draw(x, y+position, ofGetWidth(), img.height);
}

BackImage::~BackImage() {
    
}