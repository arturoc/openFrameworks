#pragma once

#include "ofPoint.h"
#include "ofTypes.h"

class ofBaseApp;

class ofAppBaseWindow{

public:

	ofAppBaseWindow(){};
	virtual ~ofAppBaseWindow(){};

	virtual void setupOpenGL(int w, int h, int screenMode)=0;
	virtual void initializeWindow()=0;
	virtual void runAppViaInfiniteLoop(ofBaseApp * appPtr)=0;

	virtual void hideCursor()=0;
	virtual void showCursor()=0;

	virtual void	setWindowPosition(int x, int y)=0;
	virtual void	setWindowShape(int w, int h)=0;

	virtual int		getFrameNum()=0;
	virtual	float	getFrameRate()=0;
	virtual double  getLastFrameTime()=0;

	virtual ofPoint	getWindowPosition()=0;
	virtual ofPoint	getWindowSize()=0;
	virtual ofPoint	getScreenSize()=0;

	virtual void			setOrientation(ofOrientation orientation)=0;
	virtual ofOrientation	getOrientation()=0;
	virtual bool	doesHWOrientation()=0;

	//this is used by ofGetWidth and now determines the window width based on orientation
	virtual int		getWidth()=0;
	virtual int		getHeight()=0;

	virtual void	setFrameRate(float targetRate)=0;
	virtual void	setWindowTitle(string title)=0;

	virtual int		getWindowMode()=0;

	virtual void	setFullscreen(bool fullscreen)=0;
	virtual void	toggleFullscreen()=0;

	virtual void	enableSetupScreen()=0;
	virtual void	disableSetupScreen()=0;
	
	virtual void	setVerticalSync(bool enabled)=0;

};

