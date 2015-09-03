#include "BackImage.h"

BackImage::BackImage() {
    
}

// setup
void BackImage::setup() {
    // 読み込む画像の設定
    img.loadImage("Image/back.png");
    // soundPlayer
    jumpSE.loadSound("SE/jump.mp3");
    fallSE.loadSound("SE/fall.mp3");
    
    init();
}

void BackImage::init() {
    // 初期化
    takePicCount  = 0;
    dispImgHeight = img.height * ofGetWidth() / img.width;
    position      = -dispImgHeight;
    ratioSpeed    = 0.1;
    gravity       = 1.6;
    speed         = 0;
    isGoal        = false;
    moveFlag      = false;
}

// ジャンプした判定をコピーする
void BackImage::copyFlag(bool flag) {
    moveFlag = flag;
}

// ジャンプした時に呼ばれる関数
void BackImage::jamping(float intiSpeed) {
    ratioSpeed = sqrt(-position / 1000000.0) - 1.0/((1.1-position/dispImgHeight) * 50.0);
    printf("ratioSpeed : %f\n", ratioSpeed);
    speed = intiSpeed * ratioSpeed;
    if ( speed >= 100 ) {
        speed = 100;
    }
    printf("initSpeed : %f\n", speed);
    printf("gravity : %f\n", gravity);
}

// update
void BackImage::update() {
    gravity =  -position / 10000.0;
    if ( moveFlag ) {
        speed -= gravity;
        position += speed;
        stopper();
    }
    goal();
    if ( position >= -ofGetHeight() * 2 - 6000 ) {
        isGoal   = true;
    }
    takePictureMode(isGoal);
}


// ゴール
void BackImage::goal() {
    if ( position >=  -ofGetHeight() * 2) {
        speed    = 0;
        position = -ofGetHeight() * 2;
    }
}

// 最地点の設定
void BackImage::stopper() {
    if ( position <= -dispImgHeight ) {
        position = -dispImgHeight;
    }
}


// draw
void BackImage::draw(float x, float y) {
    img.draw(x, y+position, ofGetWidth(), dispImgHeight);
}

// 写真を撮る
void BackImage::takePictureMode(bool isGoal) {
    if ( isGoal ) {
        if ( position <= -ofGetHeight() * 2 - 6000) {
            position = -ofGetHeight() * 2 -6000;
        }
        if ( takePicCount == 260 ) {
            takePicture = true;
        }
        takePicCount ++;
    } else {
        takePicCount = 0;
    }
}

BackImage::~BackImage() {
    
}