#pragma once

#include "ofMain.h"
#include "VectorField.h"

#define EFFECT_NUM 3

class CamShader {
public:
    int       camWidth; // カメラのwidth
    int       camHeight; // カメラのheight
    int       effectNum;
    bool      isEffect;
    
    unsigned char *pixelsBuffer; //1フレーム前のpixel情報を保存するところ

    // テクスチャ類
    ofTexture   nowImage; // 現在のカメラを保存するテクスチャ
    ofTexture   pastImage; // 過去のカメラを保存するテクスチャ
    ofTexture   resultChromaKey; // クロマキー合成した結果のテクスチャ
    ofTexture   resultComposition; // 背景と合成した結果のテクスチャ
    ofTexture   resultEffect; // エフェクトをかけた結果のテクスチャ
    // シェーダ
    ofShader    chromaKey; // クロマキー合成するシャーだ
    ofShader    composition; // 背景合成のシェーダ
    ofShader    effect[EFFECT_NUM]; // エフェクト
    // Fbo
    ofFbo       fboChromaKey;
    ofFbo       fboComposition;
    ofFbo       fboEffect;
    //　カメラ
    ofVideoGrabber camera;
    
    // ベクトル場
    VectorField VF;
    
    CamShader();
    virtual ~CamShader();
    
    void setup();
    void update();
    void draw(float x, float y);
    void shaderSetup();
    void shaderUpdate();
    void pastUpdate();
    void differPixels();
    void decisionEffect();
};