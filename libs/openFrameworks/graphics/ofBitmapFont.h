#pragma once

#include "ofConstants.h"
#include "ofPixels.h"
#include "ofVboMesh.h"
#include "ofTexture.h"


/*
 
 note, the data in this code is taken from freeglut, and included in OF for compatability 
 with non glut windowing toolkits.  see .cpp for license info
 
 also, note that while this is used internally in ofGraphics, it's not really useful for end user usage.
 
 */

class ofBitmapFont{
public:
	void setup();
	void begin(int stringLength);
	void drawCharacter(int character, int x , int y );
	void end();

private:
	static bool bBitmapPixelsPrepared;
	static ofPixels myLetterPixels;
	static float widthTex;
	static float heightTex;
	int vC;
	ofVboMesh charMesh;
	ofTexture bitmappedFontTexture;
};


