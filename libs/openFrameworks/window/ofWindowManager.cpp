/*
 *  ofWindowManager.cpp
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */
#include <ofEvents.h>
#include "ofWindowManager.h"
#include "ofConstants.h"
#include "ofAppRunner.h"

#define USE_PROGRAMMABLE_GL


/********* C STYLE FUNCTION DEFINTIONS ************************************************/



ofPtr<ofWindow> ofCreateWindow(int x, int y, int width, int height) {
	return ofWindow::createWindow(x, y, width, height);
}

ofPtr<ofWindow> ofCreateWindow(int width, int height, int monitor=0) {
	return ofWindow::createWindow(width, height,OF_WINDOW,monitor);
}

ofPtr<ofWindow> ofGetMainWindow() {
	return ofWindowManager::getWindowManager()->getMainWindow();
}

ofPtr<ofWindow> ofGetLastCreatedWindow() {
	return ofWindowManager::getWindowManager()->getLastCreatedWindow();
}

ofPtr<ofWindow> ofGetCurrentWindow(){
	return ofWindowManager::getWindowManager()->getCurrentWindow();
}

void error_callback(int error, const char* description)
{
    ofLogError("OF", description);
}

/********** ofWindowManager DEFINITIONS ******************************************/

static float timeNow, timeThen, fps;
static int nFrameCount;
static bool bFrameRateSet;
static unsigned long long millisForFrame;
static unsigned long long prevMillis;
static unsigned long long diffMillis;
static float frameRate;
static double lastFrameTime;


ofPtr<ofWindowManager> ofWindowManager::getWindowManager() {
	static ofPtr<ofWindowManager> windowManager(new ofWindowManager());
	return windowManager;
}

ofWindowManager::ofWindowManager() {
	timeNow = 0;
	timeThen = 0;
	fps = 60.0;                 //give a realistic starting value - win32 issues
	frameRate = 60.0;
	millisForFrame = 0;
	prevMillis = 0;
	diffMillis = 0;
	setFrameRate(60);
	nFrameCount = 0;
	bFrameRateSet = false;
}

ofWindowManager::~ofWindowManager() {
	glfwTerminate();
}

void ofWindowManager::addWindow(ofPtr<ofWindow> win) {
	windows.push_back(win);
}

void ofWindowManager::removeWindow(int id) {
	ofPtr<ofWindow> win = getWindowById(id);
	ofWindowList::iterator it = windows.begin();
	while(it != windows.end()) {
		if(*it == win) {
			windows.erase(it);
			break;
		}
		++it;
	}
	if(windows.size() == 0) {
		ofExit();
	}
}


ofPtr<ofWindow> ofWindowManager::getWindowById(int id) {
	ofWindowList::iterator it = windows.begin();
	while(it != windows.end()) {
		if((*it)->id == id) {
			return *it;
		}
		++it;
	}
	ofLogWarning("ofWindowManager::getWindowById - ID was not found, returning mainWindow instead");
	return mainWindow;
}

ofPtr<ofWindow> ofWindowManager::getCurrentWindow(){
	return activeWindow;
}

void ofWindowManager::setupOpenGL(int w, int h, ofWindowMode screenMode) {
	setupOpenGL(w,h,screenMode,0);
}

void ofWindowManager::setupOpenGL(int w, int h, ofWindowMode screenMode, int monitor) {
	if(!glfwInit()) {
		ofLogError("Failed to initialize GLFW");
		ofExit(0);
	}

	glfwSetErrorCallback(error_callback);

	//glfwWindowHint(GLFW_DEPTH_BITS, 32);

#ifdef USE_PROGRAMMABLE_GL
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	mainWindow = ofWindow::createWindow(w, h, screenMode, monitor);
	activeWindow = mainWindow;

	glfwMakeContextCurrent(mainWindow->getGlfwWindow());

	glfwSwapInterval( 0 );

	glfwSetTime( 0.0 );


	ofAddListener(ofEvents().exit, this, &ofWindowManager::exit);
}


void ofWindowManager::initializeWindow() {
	//define all callbacks now, so there won't happen anything before OF is fully initialized
	//glfwSetErrorCallback(&glfwErrorCallback);

	mainWindow->setup();
}

void ofWindowManager::runAppViaInfiniteLoop(ofBaseApp * appPtr) {

	mainWindow->addListener(appPtr);

	//run the main loop
	while(true) {
		//glfwWaitEvents();
		glfwPollEvents();
		update();
		draw();
	}
}

void ofWindowManager::update() {
	//	thanks to jorge for the fix:
	//	http://www.openframeworks.cc/forum/viewtopic.php?t=515&highlight=frame+rate
	if(nFrameCount != 0 && bFrameRateSet == true) {
		diffMillis = ofGetElapsedTimeMillis() - prevMillis;
		if(diffMillis > millisForFrame) {
			// we do nothing, we are already slower than target frame
		} else {
			int waitMillis = millisForFrame - diffMillis;
#ifdef TARGET_WIN32
			Sleep(waitMillis);     //windows sleep in milliseconds
#else
			usleep(waitMillis * 1000); //mac sleep in microseconds - cooler :)
#endif
		}
	}
	prevMillis = ofGetElapsedTimeMillis(); // you have to measure here
	// -------------- fps calculation:
	// theo - now moved from display to idle_cb
	// discuss here: http://github.com/openframeworks/openFrameworks/issues/labels/0062#issue/187
	//
	//
	// theo - please don't mess with this without letting me know.
	// there was some very strange issues with doing ( timeNow-timeThen ) producing different values to: double diff = timeNow-timeThen;
	// http://www.openframeworks.cc/forum/viewtopic.php?f=7&t=1892&p=11166#p11166
	timeNow = ofGetElapsedTimef();
	double diff = timeNow - timeThen;
	if(diff  > 0.00001) {
		fps = 1.0 / diff;
		frameRate *= 0.9f;
		frameRate += 0.1f * fps;
	}
	lastFrameTime = diff;
	timeThen = timeNow;
	nFrameCount++;
	// --------------

	for(int i=0;i<windows.size();i++){
		activeWindow = windows[i];
		ofSetMouseValues(activeWindow->mouseX, activeWindow->mouseY, activeWindow->previousMouseX, activeWindow->previousMouseY);
		windows[i]->update();
	}

	ofNotifyUpdate();
}

void ofWindowManager::draw() {
	for(int i=0;i<windows.size();i++){
		activeWindow = windows[i];
		ofSetMouseValues(activeWindow->mouseX, activeWindow->mouseY, activeWindow->previousMouseX, activeWindow->previousMouseY);
		windows[i]->draw();
	}

	ofNotifyDraw();
}

ofPtr<ofWindow> ofWindowManager::getLastCreatedWindow() {
	return windows.back();
}


ofPtr<ofWindow> ofWindowManager::getMainWindow() {
	return mainWindow;
}

void ofWindowManager::setFrameRate(float targetRate) {
	// given this FPS, what is the amount of millis per frame
	// that should elapse?

	// --- > f / s

	if(targetRate == 0) {
		bFrameRateSet = false;
		return;
	}

	bFrameRateSet = true;
	float durationOfFrame = 1.0f / (float)targetRate;
	millisForFrame = (int)(1000.0f * durationOfFrame);

	frameRate = targetRate;

}

float ofWindowManager::getFrameRate() {
	return frameRate;
}

ofPoint ofWindowManager::getWindowPosition() {
	return activeWindow->getWindowPosition();
}

ofPoint ofWindowManager::getWindowSize() {
	return activeWindow->getWindowSize();
}

void ofWindowManager::setOrientation(ofOrientation orientation){
	activeWindow->setOrientation(orientation);
}

ofOrientation ofWindowManager::getOrientation(){
	return activeWindow->getOrientation();
}

void ofWindowManager::hideCursor(){
	activeWindow->hideCursor();
};

void ofWindowManager::showCursor(){
	activeWindow->showCursor();
};

void ofWindowManager::setWindowPosition(int x, int y){
	activeWindow->setWindowPosition(x,y);
}

void ofWindowManager::setWindowShape(int w, int h){
	activeWindow->setWindowShape(w,h);
}

void ofWindowManager::setVerticalSync(bool vSync){
	activeWindow->setVerticalSync(vSync);
}

int ofWindowManager::getWidth() {
	return activeWindow->getWidth();
}

int ofWindowManager::getHeight() {
	return activeWindow->getHeight();
}


int ofWindowManager::getFrameNum() {
	return nFrameCount;
}

double  ofWindowManager::getLastFrameTime(){
	return lastFrameTime;
}

void ofWindowManager::setFullscreen(bool fullscreen) {
	activeWindow->setFullscreen(fullscreen);
}

void ofWindowManager::toggleFullscreen() {
	activeWindow->toggleFullscreen();
}

void ofWindowManager::exit(ofEventArgs& e) {
	glfwTerminate();
}
