#include "MultiTouchActions.h"



//--------------------------------------------------------------
void MultiTouchActions::setup(){

	reset( ) ; 

    minScale = 0.1f ;
    maxScale = 1.0f ;
}
//--------------------------------------------------------------
void MultiTouchActions::update(){
    if (objTouch.size() != 0)
	{
        updateTime();
        updateTranslate();
        if (objTouch.size() > 1)
		{
            updateRotation();
            updateScale();
        }
		else
		{
			storedNormal += normal - startNormal ; 
			startNormal = ofPoint( ) ; 
			calcNormal = ofPoint( ) ; 
            deltaDeg = 0;
        }
    }else
	{
    //    reset();
    }
}
void MultiTouchActions::draw(){
    ofPushMatrix();
    ofNoFill();
    ofSetColor(0);
    ofDrawBitmapString("Org", origin.x +5,origin.y-10);
    ofCircle( origin.x , origin.y , 10);
    ofSetColor(255,0,0);
    ofCircle( calc.x , calc.y , 10);
    ofDrawBitmapString("Avg", calc.x+5, calc.y-10);
    ofFill();
    ofPopMatrix();


}


void MultiTouchActions::reset()
{
     calcNormal = ofPoint( ) ;
	storedNormal = ofPoint( ) ; 
	startNormal = ofPoint( ) ;
	calc = ofPoint() ; 
	last = ofPoint( ) ; 
	origin = ofPoint( ) ; 
	offset = ofPoint( ) ; 
	delta = ofPoint( ) ; 
	scale = 1.0f ; 
    deltaDeg = 0;
    firstPass = 0;
    firstScale = 0;
    fingerAdded = 0;
    fingerRemoved = 0;
    isLerp = 0;
    lastDist = 0;
    cursorTime = -1;
    elapsedTime = 0;
    w = ofGetWidth();
    h = ofGetHeight();

    cursorTime = -1;
    elapsedTime = 0;

    if (fAngles.size() != 0){fAngles.erase(fAngles.begin());}

    if (objTouch.size() != 0){objTouch.erase(objTouch.begin());}
}

void MultiTouchActions::updateTranslate(){
    calc = ofPoint( ) ; 
    for (int i=0;objTouch.size() > i ;i++){
        calc.x += objTouch[i].getX();
        calc.y += objTouch[i].getY();
		calc.z += objTouch[i].getZ() ; 
    }
    if (fingerAdded == 1){
        fingerAdded = 0;
		offset = calc / objTouch.size() - origin ;
		if ( objTouch.size() == 2 ) 
		{
			startNormal = getNormal( ) ;
		}
//       offset.x = calc.x/objTouch.size() - origin.x;
//       offset.y = calc.y/objTouch.size() - origin.y;
    }
    if (fingerRemoved == 1){
        fingerRemoved = 0;
		offset = calc / objTouch.size() - origin ; 
        //offset.x = calc.x/objTouch.size() - origin.x;
        //offset.y = calc.y/objTouch.size() - origin.y;
    }
    if (firstPass == 0){
        firstPass = 1;
		last = calc / objTouch.size() ; 
        delta = ofPoint( ) ; 
		origin = calc / objTouch.size() ; 
		offset = ofPoint( ) ; 
    }else{
		calc = calc / objTouch.size() ; 
        if (isLerp == 0)
		{
			delta = calc - offset - origin ; 
        }else{
            delta.x = ofLerp( delta.x, calc.x - offset.x - origin.x, 0.2f );
            delta.y = ofLerp( delta.y, calc.y - offset.y - origin.y, 0.2f );
			delta.z = ofLerp( delta.z, calc.z - offset.z - origin.z, 0.2f ) ;
		

        }

		offset += delta ; 
	//s	origin += delta ;
    }
}
void MultiTouchActions::updateScale(){
    float totalDist = 1.0f;
    //for (int i=0;objTouch.size() > i ;i++){
	totalDist = objTouch[0].position.distance( objTouch[1].position ) ; // = sqrtf(pow(objTouch[0].getX()*w - objTouch[1].getX()*w,2)+pow(objTouch[0].getY()*w - objTouch[1].getY()*w,2));
    //}
    //totalDist = totalDist/objTouch.size();
    if (firstScale == 0){
        firstScale = 1;
        lastDist = totalDist;
    }else{
        //clog << "scale Calc: " << (totalDist - lastDist)/100 << endl;
        scale *= (totalDist/lastDist);
		//if ( scale < minScale )
		//	scale = minScale ; 
		if ( scale > maxScale )
			scale = maxScale; 
		else if ( scale > minScale )
		{
			// all ok 
		}
		else
		{
			scale = minScale; 
		}
        //scale = ( scale < minScale ) ? minScale : scale ;
        //scale = ( scale > maxScale ) ? maxScale : scale ;
        //scaleY = scaleX;
        lastDist = totalDist;
    }

}

ofPoint MultiTouchActions::getNormal( ) 
{
	if ( objTouch.size() == 2 ) 
	{
		return ( objTouch[1].position - objTouch[0].position  ) ; //);
	}
	else
		return ofPoint( ) ; 
}

void MultiTouchActions::updateRotation(){
    float tempAngle = 0.f;
    float touchAngle = 0;

    for (int i=0;objTouch.size() > i ;i++)
	{
        touchAngle = atan2(objTouch[i].getY()*h-calc.y,objTouch[i].getX()*w-calc.x);
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

	if ( objTouch.size() == 2 ) 
	{
		ofPoint diff = getNormal() - startNormal ; 	
		calcNormal = storedNormal + diff ;
	}
}

void MultiTouchActions::updateTime(){
    if (cursorTime < 0){
        cursorTime = ofGetElapsedTimef();
    }else{
        elapsedTime = ofGetElapsedTimef()-cursorTime;
    }
}

void MultiTouchActions::touchDown( ofPoint p , int id )  
{
	MultiTouchCursor c ; 
	c.setup( id , p.x , p.y ) ; 
	touchDown( c ) ; 
}

void MultiTouchActions::touchDrag( ofPoint p , int id )  
{
	MultiTouchCursor c ; 
	c.setup( id , p.x , p.y ) ; 
	touchDrag( c ) ; 
}
	

void MultiTouchActions::touchUp( ofPoint p , int id )  
{
	MultiTouchCursor c ; 
	c.setup( id , p.x , p.y ) ; 
	touchUp( c ) ; 
}
	
void MultiTouchActions::rotateToNormal(ofVec3f normal) 
{
	//asnormal.normalize();
	
	float rotationAmount;
	ofVec3f rotationAngle;
	ofQuaternion rotation;
	
	ofVec3f axis(0, 0, 0);
	rotation.makeRotate(axis, normal);
	rotation.getRotate(rotationAmount, rotationAngle);
	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
}

		//void touchDrag( ofPoint p , int id ) ; 
		//void touchRelease( ofPoint p , int id ) ; 
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
void MultiTouchActions::touchUp(MultiTouchCursor & tcur){
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

