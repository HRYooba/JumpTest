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
    imgNum      = 0;
    fadeinCount = 40;
    isStart     = false;
    isJamp      = false;
    isPic     = false;
    
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
    camShader.setup();
    back.setup();
    
    // box2D
    world.init();
    world.setFPS(60.0);
    world.setGravity(0, 10);
    
    // 画面の比
    ratioWidth  = ofGetWidth() / (float)camShader.camWidth;
    ratioHeight = ofGetHeight() / (float)camShader.camHeight;
    printf("width : %d\n", width);
    printf("height : %d\n", height);
    printf("ratioWidth : %f\n", ratioWidth);
    printf("ratioHeight : %f\n", ratioHeight);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // osc---------------------------------------------
    while ( receiver.hasWaitingMessages() )
    {
        //次のメッセージを取得
        ofxOscMessage m;
        receiver.getNextMessage( &m );
        
        if ( m.getAddress() == "/getOn" ) {
            printf("\n   on\n");
            fadeinCount = 40;
            isStart       = true;
        }
        
        if ( m.getAddress() == "/getOff" ) {
            printf("\n   off\n");
            fadeinCount = 40;
            isStart       = false;
        }
        
        if ( m.getAddress() == "/jump" ){
            printf("\n   JUMP\n");
            isJamp = true;
            back.jamping(m.getArgAsInt32(0));
            back.copyFlag(isJamp);
        }
    }
    // ------------------------------------------------
    
    if ( isStart ) {
        if ( fadeinCount >= 255 ) {
            fadeinCount = 255;
        } else {
            fadeinCount += 10;
        }
    }
    
    /*
     // もし球のある座標に力があれば球を消す
     for ( int i=0; i<circles.size(); i++ ) {
     ofVec2f frc = camShader.VF.getForceFromPos(circles[i].get()->getPosition().x, circles[i].get()->getPosition().y);
     if ( frc.length() > 0.01 ) {
     circles[i].get()->setPosition(-10000, height+10000);
     camShader.decisionEffect();
     itemSE.play();
     camShader.isEffect = true;
     }
     }
     // 球を発生させている
     if ( ofGetFrameNum()%300 == 100 && !camShader.isEffect ) {
     shared_ptr<ofxBox2dCircle> circle = shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle);
     circle.get()->setPhysics(1, 0.5, 1);
     float size = ofRandom(10, 15);
     circle.get()->setup(world.getWorld(), ofRandom(camShader.camWidth/3, camShader.camWidth*2/3), -height/2 + 10, 15);
     circles.push_back(circle);
     }
     
     // 球が外に出たらvector配列から消去
     ofRemove(circles, ofxBox2dBaseShape::shouldRemoveOffScreen);
     world.update();
     */
    if ( !isPic ) {
        camShader.update();
        back.update();
    } else {
        back.init();
    }
    
    if ( back.takePicture ) {
        flashSE.play();
        outImg.grabScreen((width - width/2)/2, 0, ofGetWidth()/2, ofGetHeight());
        string imgName =  "img" + ofToString(imgNum) + ".png";
        outImg.saveImage("/Users/oobahiroya/Desktop/SavedImage/" + imgName, OF_IMAGE_QUALITY_BEST);
        imgNum ++;
        back.takePicture = false;
        isPic          = true;
        picCount       = 0;
        back.setup();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if ( !isPic ) {
        ofPushMatrix();
        ofTranslate((width - width/2)/2, height/2);
        ofScale(0.5, 0.5);
        ofPushMatrix();
        ofTranslate(0, height);
        ofSetColor(255);
        back.draw(0, 0);
        ofPopMatrix();
        
        // size(640, 480)のカメラ関連のものを画面の大きさに合わせる
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofPushMatrix();
        ofScale(-ratioWidth, ratioWidth);
        ofTranslate(-(camShader.camWidth + (width/ratioWidth-camShader.camWidth)), 0);
        ofScale(1.5, 1.5); //-----------
        ofTranslate(0, -200); //---------------
        ofSetColor(255, fadeinCount);
        camShader.draw(0, 0);
        /* for ( int i=0; i<circles.size(); i++ ) {
         ofSetColor(255, 50, 150);
         float x = circles[i].get()->getPosition().x;
         float y = circles[i].get()->getPosition().y;
         float r = circles[i].get()->getRadius();
         ofCircle(x, y, r);
         }*/
        ofPopMatrix();
        
        // 宇宙飛行士
        if ( back.takePicCount > 0 && back.takePicCount < 240 ) {
            float x = (width - width/2)*3/2 - sqrt(back.takePicCount)*100;
            if ( x <= (width - width/2)/2 ) {
                x = (width - width/2)/2;
            }
            ofSetColor(255, 255);
            joney.draw(x, -joney.height + 100);
        }
        // カウントダウン
        ofSetColor(255, 255);
        if ( back.takePicCount >= 60  && back.takePicCount < 120) {
            if ( back.takePicCount == 60 ) {
                countSE[2].play();
            }
            countImg[2].draw(width/2 - countImg[2].width/2, -countImg[2].height/2);
        }
        if ( back.takePicCount >= 120  && back.takePicCount < 180) {
            if ( back.takePicCount == 120 ) {
                countSE[1].play();
            }
            countImg[1].draw(width/2 - countImg[1].width/2, -countImg[1].height/2);
        }
        if ( back.takePicCount >= 180  && back.takePicCount < 240) {
            if ( back.takePicCount == 180 ) {
                countSE[0].play();
            }
            countImg[0].draw(width/2 - countImg[0].width/2, -countImg[0].height/2);
        }
        ofDisableBlendMode();
        ofPopMatrix();
    } else {
        // フラッシュ
        /*if ( picCount == 0 ) {
            flashSE.play();
        }*/
        if ( picCount < 180) {
           /* ofEnableBlendMode(OF_BLENDMODE_ADD);
            ofSetColor(255, 255*sin(picCount));
            ofRect(0, 0, width, height);
            ofDisableBlendMode();*/
            ofSetColor(0);
            ofRect(0, 0, width, height);
            picCount += 30;
        } else if (picCount >= 180 && picCount < 380) {
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofSetColor(255, picCount*1.5 - 180);
            outImg.draw((width - width/2)/2, 0);
            ofDisableBlendMode();
            picCount ++;
        } else {
            isPic = false;
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
        back.copyFlag(isJamp);
    }
    if ( key == '1' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 0;
        camShader.isEffect = true;
    }
    if ( key == '2' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 1;
        camShader.isEffect = true;
    }
    if ( key == '3' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 2;
        camShader.isEffect = true;
    }
    if ( key == '4' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 3;
        camShader.isEffect = true;
    }
    if ( key == '5' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 4;
        camShader.isEffect = true;
    }
    if ( key == '6' ) {
        camShader.isEffect = camShader.isEffect ? false : true;
        itemSE.play();
        camShader.effectNum = 5;
        camShader.isEffect = true;
    }
    if ( key == '0' ) {
        camShader.isEffect = false;
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
    if (button == 0) {
        camShader.pastUpdate();
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
