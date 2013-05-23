#include "MultiTouchActions.h"

//--------------------------------------------------------------
void MultiTouchActions::setup(){
    calcX = 0;
    calcY = 0;
    lastX = 0;
    lastY = 0;
    deltaX = 0;
    deltaY = 0;
    deltaDeg = 0;
    firstPass = 0;
    firstScale = 0;
    fingerAdded = 0;
    fingerRemoved = 0;
    orgX = 0;
    orgY = 0;
    offsetX = 0;
    offsetY = 0;
    isLerp = 0;
    scaleX = 1;
    scaleY = 1;
    lastDist = 0;
    cursorTime = -1;
    elapsedTime = 0;
    w = ofGetWidth();
    h = ofGetHeight();

    minScale = 0.1f ;
    maxScale = 1.0f ;
}
//--------------------------------------------------------------
void MultiTouchActions::update(){
    if (objTouch.size() != 0){
        updateTime();
        updateTranslate();
        if (objTouch.size() > 1){
            updateRotation();
            updateScale();
        }else{
            deltaDeg = 0;
        }
    }else{
        reset();
    }
}
void MultiTouchActions::draw(){
    ofPushMatrix();
    ofNoFill();
    ofSetColor(0);
    ofDrawBitmapString("Org", orgX+5,orgY-10);
    ofCircle(orgX, orgY, 10);
    ofSetColor(255,0,0);
    ofCircle(calcX, calcY, 10);
    ofDrawBitmapString("Avg", calcX+5, calcY-10);
    ofFill();
    ofPopMatrix();
}
void MultiTouchActions::reset(){
    firstPass = 0;
    firstScale = 0;
    fingerAdded = 0;
    fingerRemoved = 0;

    deltaDeg = 0;
    deltaX = 0;
    deltaY = 0;

    orgX = 0;
    orgY = 0;

    cursorTime = -1;
    elapsedTime = 0;

    offsetX = 0;
    offsetY = 0;

    if (fAngles.size() != 0){fAngles.erase(fAngles.begin());}

    if (objTouch.size() != 0){objTouch.erase(objTouch.begin());}
}
void MultiTouchActions::updateTranslate(){
    calcX = 0;
    calcY = 0;
    for (int i=0;objTouch.size() > i ;i++){
        calcX += objTouch[i].getX()*w;
        calcY += objTouch[i].getY()*h;
    }
    if (fingerAdded == 1){
        fingerAdded = 0;
        offsetX = calcX/objTouch.size() - orgX;
        offsetY = calcY/objTouch.size() - orgY;
    }
    if (fingerRemoved == 1){
        fingerRemoved = 0;
        offsetX = calcX/objTouch.size() - orgX;
        offsetY = calcY/objTouch.size() - orgY;
    }
    if (firstPass == 0){
        firstPass = 1;
        lastX = calcX/objTouch.size();
        lastY = calcY/objTouch.size();
        deltaX = 0;
        deltaY = 0;
        orgX = calcX/objTouch.size();
        orgY = calcY/objTouch.size();
        offsetX = 0;
        offsetY = 0;
    }else{
        calcX = calcX/objTouch.size();
        calcY = calcY/objTouch.size();
        if (isLerp == 0){
            deltaX = (calcX - offsetX - orgX);
            deltaY = (calcY - offsetY - orgY);
        }else{
            deltaX = ofLerp(deltaX, calcX - offsetX - orgX, 0.2f);
            deltaY = ofLerp(deltaY, calcY - offsetY - orgY, 0.2f);
        }

        orgX += deltaX;
        orgY += deltaY;
    }
}
void MultiTouchActions::updateScale(){
    float totalDist = 0;
    //for (int i=0;objTouch.size() > i ;i++){
        totalDist = sqrtf(pow(objTouch[0].getX()*w - objTouch[1].getX()*w,2)+pow(objTouch[0].getY()*w - objTouch[1].getY()*w,2));
    //}
    //totalDist = totalDist/objTouch.size();
    if (firstScale == 0){
        firstScale = 1;
        lastDist = totalDist;
    }else{
        //clog << "scale Calc: " << (totalDist - lastDist)/100 << endl;
        scaleX *= (totalDist/lastDist);
        scaleX = ( scaleX < minScale ) ? minScale : scaleX ;
        scaleX = ( scaleX > maxScale ) ? maxScale : scaleX ;
        scaleY = scaleX;
        lastDist = totalDist;
    }

}
void MultiTouchActions::updateRotation(){
    float tempAngle = 0.f;
    float touchAngle = 0;
    for (int i=0;objTouch.size() > i ;i++){
        touchAngle = atan2(objTouch[i].getY()*h-calcY,objTouch[i].getX()*w-calcX);
        if (fAngles[i].x != 0){
            if (abs(fAngles[i].x - touchAngle) < .35){
                tempAngle += fAngles[i].x - touchAngle;
                fAngles[i].x = touchAngle;
            }else{
                fAngles[i].x = touchAngle;
            }
        }else{
            fAngles[i].x = touchAngle;
        }
    }
    tempAngle = ((180/PI) * tempAngle)/objTouch.size();
    deltaDeg = ofLerp(deltaDeg, tempAngle*-1, 0.2f);
}
void MultiTouchActions::updateTime(){
    if (cursorTime < 0){
        cursorTime = ofGetElapsedTimef();
    }else{
        elapsedTime = ofGetElapsedTimef()-cursorTime;
    }
}

//--------------------------------------------------------------
void MultiTouchActions::touchDown(MultiTouchCursor & tcur){
    objTouch.push_back(tcur);
    fAngles.push_back(ofVec2f(0,0));
    fingerAdded = 1;
}
void MultiTouchActions::touchDrag(MultiTouchCursor & tcur){
    //if touch start drag
    if (objTouch.size() != 0){
        for (int i=0;objTouch.size() > i ;i++){
            if (tcur.getCursorID() == objTouch[i].getCursorID()){
                objTouch[i] = tcur;
            }
        }
    }
}
void MultiTouchActions::touchRelease(MultiTouchCursor & tcur){
    //remove touch from object stack
    float timeCheck = elapsedTime;
    if (objTouch.size() != 0){
        for (int i=0;objTouch.size() > i ;i++){
            if (objTouch[i].getCursorID() == tcur.getCursorID()){
                objTouch.erase(objTouch.begin()+i);
                fAngles.erase(fAngles.begin()+i);
               // clog << "removed: " << i << " " << objTouch.size() << endl;
                fingerRemoved = 1;
                if ((i == 0) || (i == 1)){
                    firstScale = 0;
                }
            }
        }
        /*if (objTouch.size() == 0){
            if (timeCheck < .75){
                if (timeCheck > 0){
                    //cout << "tap not drag" << endl;
                }
            }
        }*/
    }
}

