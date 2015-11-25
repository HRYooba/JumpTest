/*
 プロジェクターで投影する場合
 */


#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxBox2D.h"
#include "BackImage.h"
#include "CamShader.h"

#define PORT 5555 //自分のポート番号

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    int     width; // 画面の幅
    int     height; // 画面の高さ
    int     fadeinCount; // フェードインするカウント
    int     showPicCount; // 撮った写真を表示する時間
    int     takePicCount; // 写真を撮るカウント
    float   ratioWidth; // 画面の大きさ / カメラの大きさ
    float   ratioHeight; // 画面の大きさ / カメラの大きさ
    float   jumpAvarage; // ジャンプの時間の平均
    float   jumpSum; // ジャンプ時間の合計
    float   jumpCount; // ジャンプのカウント
    float   jumpLastTime; // 前のジャンプしたタイミング
    float   jumpBetween; // ジャンプ間の時間
    float   startShutter; // シャッターのタイミング
    float   testTime;
    int     testCount;
    bool    isJamp; // ジャンプしたかのフラグ
    bool    isStart; // スタート
    bool    isShowPic; // 撮った写真を表示するか
    bool    isCountSE[3]; // カウントダウンSEのフラグ
    
    // 画像
    ofImage     outImg; // 写真
    ofImage     countImg[3]; // カウントの画像
    ofImage     joney; // 宇宙飛行士の画像
    
    // 音
    ofSoundPlayer   itemSE;
    ofSoundPlayer   flashSE;
    ofSoundPlayer   countSE[5];
    
    // osc
    ofxOscReceiver receiver;
    
    // クラス
    CamShader   cam;
    BackImage   back;
    
    ofxBox2d    world;
    vector<ofPtr<ofxBox2dCircle> > circles;
};
