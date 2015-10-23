#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // OSCの初期設定
    receiver.setup(PORT);
    
    // setup
    ofSetFrameRate(60);
    ofBackground(0);
    width       = ofGetWidth();
    height      = ofGetHeight();
    fadeinCount = 40;
    isStart     = false;
    isJamp      = false;
    isShowPic   = false;
    
    // 画像
    for ( int i=0; i<3; i++ ) {
        countImg[i].loadImage("Image/count" + ofToString(i+1) + ".png");
    }
    joney.loadImage("Image/joney2.png");
    
    // 音
    itemSE.loadSound("SE/item.mp3");
    flashSE.loadSound("SE/flash.mp3");
    for ( int i=0; i<3; i++ ) {
        countSE[i].loadSound("SE/count" + ofToString(i+1) + ".mp3");
    }
    
    // 各クラス
    cam.setup();
    back.setup();
    
    // box2D
    world.init();
    world.setFPS(60.0);
    world.setGravity(0, 10);
    
    // 画面の比
    ratioWidth  = ofGetWidth() / (float)cam.camWidth;
    ratioHeight = ofGetHeight() / (float)cam.camHeight;
    printf("width : %d\n", width);
    printf("height : %d\n", height);
    printf("ratioWidth : %f\n", ratioWidth);
    printf("ratioHeight : %f\n", ratioHeight);
}

//--------------------------------------------------------------
void ofApp::update(){
    isJamp = false;
    // osc---------------------------------------------
    while ( receiver.hasWaitingMessages() )
    {
        //次のメッセージを取得
        ofxOscMessage m;
        receiver.getNextMessage( &m );
        
        // 登ったら
        if ( m.getAddress() == "/getOn" ) {
            printf("\n------on------\n");
            fadeinCount = 40;
            isStart     = true;
        }
        
        // 降りたら
        if ( m.getAddress() == "/getOff" ) {
            printf("\n------off------\n");
            fadeinCount = 40;
            isStart     = false;
        }
        
        // ジャンプしたら
        if ( m.getAddress() == "/jump" && !isShowPic ){
            printf("\n   JUMP\n");
            isJamp      = true;
            back.jamping(m.getArgAsInt32(0));
            if ( isStart && back.position >= -ofGetHeight() * 2 - 6000 ) {
                if ( jumpLastTime == 0 ) {
                    jumpLastTime = ofGetElapsedTimef();
                } else {
                    jumpCount ++;
                    jumpBetween  = ofGetElapsedTimef() - jumpLastTime;
                    jumpLastTime = ofGetElapsedTimef();
                    jumpSum      += jumpBetween;
                    jumpAvarage  = jumpSum / jumpCount;
                    printf("Between : %f\n", jumpBetween);
                    printf("Average : %f\n", jumpAvarage);
                }
                if ( takePicCount > 240 && startShutter == 0 ) {
                    startShutter = ofGetElapsedTimef();
                }
            }
        }
    }
    // ------------------------------------------------
    
    // もしスタートしたらファードインする
    if ( isStart ) {
        if ( fadeinCount >= 255 ) {
            fadeinCount = 255;
        } else {
            fadeinCount += 10;
        }
    } else {
        jumpCount    = 0;
        jumpLastTime = 0;
        jumpBetween  = 0;
        jumpSum      = 0;
        jumpAvarage  = 0;
        startShutter = 0;
    }
    
    //　撮った写真を表示してないなら
    if ( !isShowPic ) {
        cam.update();
        back.update();
    } else {
        back.init();
        jumpCount    = 0;
        jumpLastTime = 0;
        jumpBetween  = 0;
        jumpSum      = 0;
        jumpAvarage  = 0;
        startShutter = 0;
    }
    
    // 写真を撮る
    if ( back.isGoal ) {
        if ( startShutter != 0 ) {
            float start = ofGetElapsedTimef() - startShutter;
            printf("start : %f\n", start);
            if ( start >= jumpAvarage/4.0 ) {
                printf("\n==========Shutter==========\n");
                flashSE.play();
                outImg.grabScreen((width - width/2)/2, 0, width/2, height);
                string imgName =  ofToString(ofGetYear()) + "_" + ofToString(ofGetMonth()) + ":" + ofToString(ofGetDay()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + "_" + ofToString(ofGetSeconds()) + ".png";
                outImg.saveImage("/Users/oobahiroya/Google ドライブ/SaveImage/" + imgName, OF_IMAGE_QUALITY_BEST);
                isShowPic    = true;
                showPicCount = 0;
                back.setup();
            }
        } else {
            takePicCount ++;
            printf("takePicCount : %d\n", takePicCount);
        }
    } else {
        takePicCount = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( !isShowPic ) {
        // 背景
        ofPushMatrix();
        ofTranslate((width - width/2)/2, height/2);
        ofScale(0.5, 0.5);
        ofPushMatrix();
        ofTranslate(0, height);
        ofSetColor(255);
        back.draw(0, 0);
        ofPopMatrix();
        
        // size(640, 480)のカメラ関連のものを画面の大きさに合わせる
        //　カメラ
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPushMatrix();
        ofScale(-ratioWidth, ratioWidth);
        ofTranslate(-(cam.camWidth + (width/ratioWidth-cam.camWidth)), 0);
        ofScale(1.5, 1.5); //-----------
        ofTranslate(0, -200); //---------------
        ofSetColor(255, fadeinCount);
        cam.draw(0, 0);
        ofPopMatrix();
        
        if ( startShutter == 0 ) {
            // 宇宙飛行士
            if ( takePicCount > 0 ) {
                float x = (width - width/2)*3/2 - sqrt(takePicCount)*100;
                if ( x <= (width - width/2)/2 ) {
                    x = (width - width/2)/2;
                }
                ofSetColor(255, 255);
                joney.draw(x, -joney.height + 100);
            }
            // カウントダウン
            ofSetColor(255, 255);
            if ( takePicCount >= 60  && takePicCount < 120 ) {
                if ( takePicCount == 60 ) {
                    countSE[2].play();
                }
                countImg[2].draw(width/2 - countImg[2].width/2, -countImg[2].height/2);
            }
            if ( takePicCount >= 120  && takePicCount < 180 ) {
                if ( takePicCount == 120 ) {
                    countSE[1].play();
                }
                countImg[1].draw(width/2 - countImg[1].width/2, -countImg[1].height/2);
            }
            if ( takePicCount >= 180 ) {
                if ( takePicCount == 180 ) {
                    countSE[0].play();
                }
                countImg[0].draw(width/2 - countImg[0].width/2, -countImg[0].height/2);
            }
        }
        ofDisableBlendMode();
        ofPopMatrix();
    } else {
        // 撮った写真を表示
        if ( showPicCount < 180) {
            ofSetColor(0);
            ofRect(0, 0, width, height);
            showPicCount += 30;
        } else if (showPicCount >= 180 && showPicCount < 380) {
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofSetColor(255, showPicCount*1.5 - 180);
            outImg.draw((width - width/2)/2, 0);
            ofDisableBlendMode();
            showPicCount ++;
        } else {
            isShowPic = false;
        }
    }
    // サイドを消してる
    ofSetColor(0);
    ofRect(0, 0, (width - width/2)/2, height);
    ofRect((width - width/2)*3/2, 0, width - (width - width/2)*3/2, height);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if ( key == ' ' ) {
        isJamp = true;
        back.jamping(600);
    }
    if ( key == '1' ) {
        cam.isEffect = cam.isEffect ? false : true;
        itemSE.play();
        cam.effectNum = 0;
        cam.isEffect = true;
    }
    if ( key == '2' ) {
        cam.isEffect = cam.isEffect ? false : true;
        itemSE.play();
        cam.effectNum = 1;
        cam.isEffect = true;
    }
    if ( key == '3' ) {
        cam.isEffect = cam.isEffect ? false : true;
        itemSE.play();
        cam.effectNum = 2;
        cam.isEffect = true;
    }
    if ( key == '4' ) {
        cam.isEffect = cam.isEffect ? false : true;
        itemSE.play();
        cam.effectNum = 3;
        cam.isEffect = true;
    }
    if ( key == '5' ) {
        cam.isEffect = cam.isEffect ? false : true;
        itemSE.play();
        cam.effectNum = 4;
        cam.isEffect = true;
    }
    if ( key == '6' ) {
        cam.isEffect = cam.isEffect ? false : true;
        itemSE.play();
        cam.effectNum = 5;
        cam.isEffect = true;
    }
    if ( key == '0' ) {
        cam.isEffect = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
