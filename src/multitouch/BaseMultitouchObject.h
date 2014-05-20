#pragma once

#include "ofMain.h"

class BaseMultitouchObject
{
public: 
	BaseMultitouchObject( ) { } 
	~BaseMultitouchObject( ) { } 

	virtual void inputDown( float x , float y , int id ) { }  
	virtual void inputDrag( float x , float y , int id ) { }  
	virtual void inputUp( float x , float y , int id ) { }  
};