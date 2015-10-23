#include "CamShader.h"


CamShader::CamShader() {
    
}

// setup
void CamShader::setup() {
    // setup
    camWidth    = 640;
    camHeight   = 480;
    isEffect    = false;
    effectNum   = 0;
    
    // SoundPlayer
    hologramSE.loadSound("SE/hologram.mp3");
    hologramSE.setLoop(true);
    
    //　カメラの初期設定
    camera.setVerbose(true);
    /*ofSetLogLevel(OF_LOG_VERBOSE);
     camera.listDevices();*/
    camera.setDeviceID(0);
    camera.initGrabber(camWidth, camHeight);
    
    // シェーダの初期化
    shaderSetup();
    
    // pixelsBufferの初期化
    pixelsBuffer = new unsigned char[(int)camWidth * (int)camHeight * 3];
}

// シェーダsetup
void CamShader::shaderSetup() {
    // テクスチャ類のallocate
    nowImage.allocate(camWidth, camHeight, GL_RGB);
    pastImage.allocate(camWidth, camHeight, GL_RGB);
    resultChromaKey.allocate(camWidth, camHeight, GL_RGB);
    resultComposition.allocate(camWidth, camHeight, GL_RGB);
    resultEffect.allocate(camWidth, camHeight, GL_RGB);
    
    // シェーダの設定
    chromaKey.load("", "Shaders/Chroma_key.frag");
    composition.load("", "Shaders/Composition.frag");
    // エフェクト
    effect[0].load("", "Shaders/Echo_effect.frag");
    effect[1].load("", "Shaders/Shining_effect.frag");
    effect[2].load("", "Shaders//Hologram_effect.frag");
    effect[3].load("", "Shaders/Invisible_effect.frag");
    effect[4].load("", "Shaders/Gray_effect.frag");
    effect[5].load("", "Shaders/Crayon_effect.frag");
    
    // fbo
    fboChromaKey.allocate(camWidth, camHeight);
    fboComposition.allocate(camWidth, camHeight);
    fboEffect.allocate(camWidth, camHeight);
}

// シェーダの更新
void CamShader::shaderUpdate() {
    // クロマキー合成
    fboChromaKey.begin();
    chromaKey.begin();
    chromaKey.setUniformTexture("u_nowImage", nowImage, 0);
    ofRect(0, 0, camWidth, camHeight);
    chromaKey.end();
    fboChromaKey.end();
    resultChromaKey = fboChromaKey.getTextureReference();
    
    // 背景と合成
    fboComposition.begin();
    composition.begin();
    composition.setUniformTexture("u_nowImage", nowImage, 0);
    composition.setUniformTexture("u_chromaKey", resultChromaKey, 1);
    ofRect(0, 0, camWidth, camHeight);
    composition.end();
    fboComposition.end();
    resultComposition = fboComposition.getTextureReference();
    
    // エフェクト
    fboEffect.begin();
    effect[effectNum].begin();
    effect[effectNum].setUniformTexture("u_composition", resultComposition, 0);
    effect[effectNum].setUniformTexture("u_effect", resultEffect, 1);
    effect[effectNum].setUniform2f("u_resolution", camWidth, camHeight);
    effect[effectNum].setUniform1f("u_time", ofGetFrameNum());
    effect[effectNum].setUniform1f("u_count", effectCount);
    effect[effectNum].setUniform1f("u_random", ofRandom(1.0));
    ofRect(0, 0, camWidth, camHeight);
    effect[effectNum].end();
    fboEffect.end();
    if ( isEffect ) {
        resultEffect = fboEffect.getTextureReference();
    } else {
        resultEffect = fboComposition.getTextureReference();
        hologramSE.stop();
    }
}

// update
void CamShader::update() {
    
    // カメラとピクセル情報の更新
    camera.update();
    if ( camera.isFrameNew() ) {
        unsigned char *pixels = camera.getPixels();
        nowImage.loadData(pixels, camWidth, camHeight, GL_RGB);
    }
    
    // シェーダの更新
    shaderUpdate();
}

// draw
void CamShader::draw(float x, float y) {
    if ( !isEffect ) {
        effectCount = 0;
        fboComposition.draw(x, y);
    } else {
        effectCount ++;
        int alpha = effectCount * 10;
        if ( alpha > 255 ) {
            alpha = 255;
        }
        if ( effectCount > 200 ) {
            alpha = MAX(abs(255*sin((float)effectCount * 0.15)), 400 - effectCount);
            if ( effectCount > 400 ) {
                isEffect = false;
            }
        }
//        ofEnableAlphaBlending();
        ofSetColor(255, 255 - alpha);
        fboComposition.draw(x, y);
        ofSetColor(255, alpha);
        fboEffect.draw(x, y);
//        ofDisableAlphaBlending();
    }
}

// 現在のカメラ情報と比べるための昔の画像の更新
void CamShader::pastUpdate() {
    unsigned char *pixels = camera.getPixels();
    pastImage.loadData(pixels, camWidth, camHeight, GL_RGB);
    composition.setUniformTexture("u_pastImage", pastImage, 1);
}

// 現在のフレームと一個前のフレームのピクセルを比べる
void CamShader::differPixels() {
    // 現在のカメラのpixelデータを取得
    unsigned char *pixels = camera.getPixels();
    for ( int y=0; y<camHeight; y++ ) {
        for ( int x=0; x<camWidth; x++ ) {
            // pixelのindexの設定
            int index = (y*camWidth + x) * 3;
            // 前フレームとのRGBの差分を計算
            int R = int(pixels[index]) - int(pixelsBuffer[index]);
            int G = int(pixels[index+1]) - int(pixelsBuffer[index+1]);
            int B = int(pixels[index+2]) - int(pixelsBuffer[index+2]);
            // 前フレームとの差分が一定値を超えたら
            if ( R*R + G*G + B*B >= 70*70 ) {
            }
        }
    }
    // 最後に今のフレームのpixelをbufferに保存
    for ( int i=0; i<camHeight*camWidth*3; i++ ) {
        pixelsBuffer[i] = pixels[i];
    }
}

// エフェクトを決定する
void CamShader::decisionEffect() {
    int num = (int)ofRandom(0, EFFECT_SUM);
    effectNum = num;
    if ( effectNum == 2 ) {
        hologramSE.play();
    }
}

CamShader::~CamShader() {
    
}
