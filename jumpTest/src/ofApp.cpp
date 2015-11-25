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
        countSE[i].loadSound("SE/count" + ofToString(i+4) + ".mp3");
        countSE[i].setSpeed(2.4);
        countSE[i].setVolume(0.8);
        countSE[i].setLoop(false);
        isCountSE[i] = false;
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
    printf("\nwidth : %d\n", width);
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
            fadeinCount  = 40;
            isStart      = true;
            jumpCount    = 0;
            jumpLastTime = 0;
            jumpBetween  = 0;
            jumpSum      = 0;
            jumpAvarage  = 0;
            startShutter = 0;
            for ( int i=0; i<3; i++ ) {
                countSE[i].setPosition(0);
                isCountSE[i] = false;
            }
        }
        
        // 降りたら
        if ( m.getAddress() == "/getOff" ) {
            printf("\n------off------\n");
            fadeinCount  = 40;
            isStart      = false;
            jumpCount    = 0;
            jumpLastTime = 0;
            jumpBetween  = 0;
            jumpSum      = 0;
            jumpAvarage  = 0;
            startShutter = 0;
            for ( int i=0; i<3; i++ ) {
                countSE[i].setPosition(0);
                isCountSE[i] = false;
            }
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
                    testTime = jumpLastTime;
                    jumpLastTime = ofGetElapsedTimef();
                    jumpSum      += jumpBetween;
                    jumpAvarage  = jumpSum / jumpCount;
                    printf("Between : %f\n", jumpBetween);
                    printf("Average : %f\n", jumpAvarage);
                }
                if ( jumpCount == 4 && startShutter == 0 ) {
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
        for ( int i=0; i<3; i++ ) {
            countSE[i].setPosition(0);
            isCountSE[i] = false;
        }
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
        for ( int i=0; i<3; i++ ) {
            countSE[i].setPosition(0);
            isCountSE[i] = false;
        }
    }
    
    // 写真を撮る
    if ( back.isGoal ) {
        if ( startShutter != 0 ) {
            float start = ofGetElapsedTimef() - startShutter;
            printf("start : %f\n", start);
            if ( start >= jumpAvarage / 3.0 ) {
                flashSE.play();
                outImg.grabScreen((width - width/2)/2, 0, ofGetWidth()/2, ofGetHeight());
                string imgName =  ofToString(ofGetYear()) + "_" + ofToString(ofGetMonth()) + ":" + ofToString(ofGetDay()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + "_" + ofToString(ofGetSeconds()) + ".png";
                outImg.saveImage("/Users/oobahiroya/Google ドライブ/SaveImage/" + imgName, OF_IMAGE_QUALITY_BEST);
                printf("\n==========Shutter==========\n");
                isShowPic    = true;
                showPicCount = 0;
                back.setup();
            }
        }
        takePicCount ++;
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
        //        ofTranslate(-(cam.camWidth + (width/ratioWidth-cam.camWidth)), 0);
        ofTranslate(-cam.camWidth, 0);
        ofScale(1.5, 1.5); //-----------
        ofTranslate(0, -ratioHeight * 106.67); //---------------
        ofSetColor(255, fadeinCount);
        cam.draw(0, 0);
        ofPopMatrix();
        
        // 宇宙飛行士
        if ( takePicCount > 0 && jumpCount < 4) {
            float x = (width - width/2) * 3/2 - sqrt(takePicCount) * 100;
            if ( x <= (width - width/2) / 2 ) {
                x = (width - width/2) / 2;
            }
            ofSetColor(255, 255);
            joney.draw(x, -joney.height + ratioHeight * 53.333);
        }
        // カウントダウン
        ofSetColor(255, 255);
        if ( back.isGoal && jumpCount < 4) {
            if ( jumpCount == 1 ) {
                 if ( !isCountSE[2] ) {
                 float time = ofGetElapsedTimef() - testTime;
                 printf("Three : %f\n", time);
                 countSE[2].play();
                 isCountSE[2] = true;
                 }
                if ( countSE[2].getIsPlaying() ) {
                    countImg[2].draw(width/2 - countImg[2].width/2, -countImg[2].height/2);
                }
            }
            if ( jumpCount == 2 ) {
                 if ( !isCountSE[1] ) {
                 float time = ofGetElapsedTimef() - testTime;
                 printf("Two : %f\n", time);
                 countSE[1].play();
                 isCountSE[1] = true;
                 }
                if ( countSE[1].getIsPlaying() ) {
                    countImg[1].draw(width/2 - countImg[2].width/2, -countImg[2].height/2);
                }
            }
            if ( jumpCount == 3 ) {
                 if ( !isCountSE[0] ) {
                 float time = ofGetElapsedTimef() - testTime;
                 printf("One : %f\n", time);
                 countSE[0].play();
                 isCountSE[0] = true;
                 }
                if ( countSE[0].getIsPlaying() ) {
                    countImg[0].draw(width/2 - countImg[2].width/2, -countImg[2].height/2);
                }
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
        printf("\n   JUMP\n");
        isStart      = true;
        isJamp      = true;
        back.jamping(500);
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
            if ( jumpCount == 4 && startShutter == 0 ) {
                startShutter = ofGetElapsedTimef();
            }
        }
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
    testCount ++;
    if ( testCount == 1 && !isCountSE[2] ) {
        float time = ofGetElapsedTimef() - testTime;
        printf("ThreeMouse : %f\n", time);
        countSE[2].play();
        isCountSE[2] = true;
    }
    if ( testCount == 2 && !isCountSE[1] ) {
        float time = ofGetElapsedTimef() - testTime;
        printf("TwoMouse : %f\n", time);
        countSE[1].play();
        isCountSE[2] = true;
    }
    if ( testCount == 3  && !isCountSE[0] ) {
        float time = ofGetElapsedTimef() - testTime;
        printf("OneMouse : %f\n", time);
        countSE[0].play();
        isCountSE[2] = true;
    }
    if ( testCount == 4 ) {
        flashSE.play();
        outImg.grabScreen((width - width/2)/2, 0, ofGetWidth()/2, ofGetHeight());
        string imgName =  ofToString(ofGetYear()) + "_" + ofToString(ofGetMonth()) + ":" + ofToString(ofGetDay()) + "_" + ofToString(ofGetHours()) + "_" + ofToString(ofGetMinutes()) + "_" + ofToString(ofGetSeconds()) + ".png";
        outImg.saveImage("/Users/oobahiroya/Google ドライブ/SaveImage/" + imgName, OF_IMAGE_QUALITY_BEST);
        printf("\n==========Shutter==========\n");
        isShowPic    = true;
        showPicCount = 0;
        back.setup();
        testCount = 0;
    }
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
