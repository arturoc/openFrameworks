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

/********** GLFW CALLBACKS **********************************************************/

void ofWindowManager::glfwErrorCallback(int type, const char * err) {
	ofLog(OF_LOG_ERROR, err);
}

void ofWindowManager::glfwWindowSizeCallback(GLFWwindow * glfwWin, int w, int h) {
	ofWindowManager::getWindowManager()->glfwWindowSize(glfwWin, w, h);
}

void ofWindowManager::glfwWindowCloseCallback(GLFWwindow * glfwWin) {
	ofWindowManager::getWindowManager()->glfwWindowClose(glfwWin);
}

void ofWindowManager::glfwWindowRefreshCallback(GLFWwindow * glfwWin) {
	ofWindowManager::getWindowManager()->glfwWindowRefresh(glfwWin);
}

void ofWindowManager::glfwWindowFocusCallback(GLFWwindow * glfwWin, int action) {
	ofWindowManager::getWindowManager()->glfwWindowFocus(glfwWin, action);
}

void ofWindowManager::glfwWindowIconifyCallback(GLFWwindow * glfwWin, int action) {
	ofWindowManager::getWindowManager()->glfwWindowIconify(glfwWin, action);
}

void ofWindowManager::glfwMouseButtonCallback(GLFWwindow * glfwWin, int button, int action) {
	ofWindowManager::getWindowManager()->glfwMouseButton(glfwWin, button, action);
}

void ofWindowManager::glfwMousePosCallback(GLFWwindow * glfwWin, double x, double y) {
	ofWindowManager::getWindowManager()->glfwMousePos(glfwWin, x, y);
}

void ofWindowManager::glfwCursorEnterCallback(GLFWwindow * glfwWin, int action) {
	ofWindowManager::getWindowManager()->glfwCursorEnter(glfwWin, action);
}

void ofWindowManager::glfwScrollCallback(GLFWwindow * glfwWin, double deltaX, double deltaY) {
	ofWindowManager::getWindowManager()->glfwScroll(glfwWin, deltaX, deltaY);
}

void ofWindowManager::glfwKeyCallback(GLFWwindow * glfwWin, int key, int action) {
	ofWindowManager::getWindowManager()->glfwKey(glfwWin, key, action);
}

void ofWindowManager::glfwCharCallback(GLFWwindow * glfwWin, unsigned int character) {
	ofWindowManager::getWindowManager()->glfwChar(glfwWin, character);
}



/********* C STYLE FUNCTION DEFINTIONS ************************************************/



ofPtr<ofWindowManager> ofWindowManager::getWindowManager() {
	static ofPtr<ofWindowManager> windowManager(new ofWindowManager());
	return windowManager;
}

ofPtr<ofWindow> ofCreateWindow(int x, int y, int width, int height) {
	return ofWindowManager::getWindowManager()->createWindow(x, y, width, height);
}

ofPtr<ofWindow> ofCreateWindow(int width, int height) {
	return ofCreateWindow(0, 0, width, height);
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

ofPtr<ofWindow> ofWindowManager::createWindow(int w, int h, ofWindowMode windowMode) {
	return createWindow(0, 0, w, h, windowMode);
}

ofPtr<ofWindow> ofWindowManager::createWindow(int x, int y, int width, int height, ofWindowMode windowMode) {
	ofPtr<ofWindow> win(new ofWindow());

	win->setWindowPositionAndShape(x, y, width, height);
	addWindow(win);
	win->initializeWindow(windowMode);
	if(win->getGlfwWindow() != NULL) {
		glfwSetWindowSizeCallback(win->getGlfwWindow(), &glfwWindowSizeCallback);
		glfwSetWindowCloseCallback(win->getGlfwWindow(),&glfwWindowCloseCallback);
		glfwSetWindowRefreshCallback(win->getGlfwWindow(),&glfwWindowRefreshCallback);
		glfwSetWindowFocusCallback(win->getGlfwWindow(),&glfwWindowFocusCallback);
		glfwSetWindowIconifyCallback(win->getGlfwWindow(),&glfwWindowIconifyCallback);
		glfwSetMouseButtonCallback(win->getGlfwWindow(),&glfwMouseButtonCallback);
		glfwSetCursorPosCallback(win->getGlfwWindow(),&glfwMousePosCallback);
		glfwSetCursorEnterCallback(win->getGlfwWindow(),&glfwCursorEnterCallback);
		glfwSetScrollCallback(win->getGlfwWindow(),&glfwScrollCallback);
		glfwSetKeyCallback(win->getGlfwWindow(),&glfwKeyCallback);
		glfwSetCharCallback(win->getGlfwWindow(),&glfwCharCallback);
	}

	return win;
}

void ofWindowManager::addWindow(ofPtr<ofWindow> win) {
	windows.push_back(win);
}

void ofWindowManager::deleteWindow(ofPtr<ofWindow> win) {
	win->destroy();
}

void ofWindowManager::removeWindow(ofPtr<ofWindow> win) {

	ofWindowList::iterator it = windows.begin();
	while(it != windows.end()) {
		if(*it == win) {
			win->windowClosed();
			windows.erase(it);
			break;
		}
		++it;
	}
	if(windows.size() == 0) {
		ofExit();
	}
}

void ofWindowManager::deleteWindow(int id) {
	deleteWindow(getWindowById(id));
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


ofPtr<ofWindow> ofWindowManager::getWindowByGlfw(GLFWwindow * win) {
	ofWindowList::iterator it = windows.begin();
	while(it != windows.end()) {
		if((*it)->getGlfwWindow() == win) {
			return *it;
		}
		++it;
	}
	ofLogWarning("ofWindowManager::getWindowById - GLFWwindow was not found, returning mainWindow instead");
	return mainWindow;
}

void ofWindowManager::setupOpenGL(int w, int h, int screenMode) {
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
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	ofWindowMode windowMode = OF_WINDOW;
	if(screenMode == OF_GAME_MODE)
		windowMode = OF_GAME_MODE;

	mainWindow = createWindow(w, h, windowMode);
	activeWindow = mainWindow;

	glfwMakeContextCurrent(mainWindow->getGlfwWindow());

	glfwSwapInterval( 0 );

	glfwSetTime( 0.0 );


	ofAddListener(ofEvents().exit, this, &ofWindowManager::exit);
}

void ofWindowManager::initializeWindow() {
	//define all callbacks now, so there won't happen anything before OF is fully initialized
	glfwSetErrorCallback(&glfwErrorCallback);

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

int ofWindowManager::getWidth() {
	return activeWindow->getWidth();
}

int ofWindowManager::getHeight() {
	return activeWindow->getHeight();
}

////GLFW CALLBACKS
void ofWindowManager::glfwWindowFocus(GLFWwindow * glfwWin, int action) {
	//ofWindow * win = getWindowByGlfw(glfwWin);
}

void ofWindowManager::glfwWindowSize(GLFWwindow * glfwWin, int w, int h) {
	ofPtr<ofWindow> win = getWindowByGlfw(glfwWin);
	win->windowResized(w, h);
}

int ofWindowManager::glfwWindowClose(GLFWwindow * glfwWin) {
	ofPtr<ofWindow> win = getWindowByGlfw(glfwWin);
	removeWindow(win);
	return 1;
}

void ofWindowManager::glfwWindowRefresh(GLFWwindow * glfwWin) {
	//ofWindow * win = getWindowByGlfw(glfwWin);
}

void ofWindowManager::glfwWindowIconify(GLFWwindow * glfwWin, int action) {
	//ofWindow * win = getWindowByGlfw(glfwWin);
}

void ofWindowManager::glfwMouseButton(GLFWwindow * glfwWin, int button, int action) {
	ofPtr<ofWindow> win = getWindowByGlfw(glfwWin);
	if(action == GLFW_PRESS) {
		win->mousePressed(button);
	} else {
		win->mouseReleased(button);
	}
}

void ofWindowManager::glfwMousePos(GLFWwindow * glfwWin, int mouseX, int mouseY) {
	ofPtr<ofWindow> win = getWindowByGlfw(glfwWin);
	win->mouseMoved(mouseX, mouseY);
}

void ofWindowManager::glfwCursorEnter(GLFWwindow * glfwWin, int action) {
	//ofWindow * win = getWindowByGlfw(glfwWin);
}

void ofWindowManager::glfwScroll(GLFWwindow * glfwWin, float deltaX, float deltaY) {
	ofPtr<ofWindow> win = getWindowByGlfw(glfwWin);
	win->scrolled(-deltaX, -deltaY);
}


void ofWindowManager::glfwKey(GLFWwindow * glfwWin, int key, int action) {
	ofPtr<ofWindow> win = getWindowByGlfw(glfwWin);

	//TODO: I'm sure there is some way to do this more efficient than a case statement (Philip)
	switch(key) {
	case GLFW_KEY_ENTER:
		key = OF_KEY_RETURN;
		break;

	case GLFW_KEY_ESC:
		key = OF_KEY_ESC;
		break;

	case GLFW_KEY_LEFT_CONTROL:
	case GLFW_KEY_RIGHT_CONTROL:
		key = OF_KEY_CTRL;
		break;

	case GLFW_KEY_RIGHT_ALT:
	case GLFW_KEY_LEFT_ALT:
		key = OF_KEY_ALT;
		break;

	case GLFW_KEY_RIGHT_SHIFT:
	case GLFW_KEY_LEFT_SHIFT:
		key = OF_KEY_SHIFT;
		break;

	case GLFW_KEY_BACKSPACE:
		key = OF_KEY_BACKSPACE;
		break;

	case GLFW_KEY_DELETE:
		key = OF_KEY_DEL;
		break;

	case GLFW_KEY_F1:
		key = OF_KEY_F1;
		break;

	case GLFW_KEY_F2:
		key = OF_KEY_F2;
		break;

	case GLFW_KEY_F3:
		key = OF_KEY_F3;
		break;

	case GLFW_KEY_F4:
		key = OF_KEY_F4;
		break;

	case GLFW_KEY_F5:
		key = OF_KEY_F5;
		break;

	case GLFW_KEY_F6:
		key = OF_KEY_F6;
		break;

	case GLFW_KEY_F7:
		key = OF_KEY_F7;
		break;

	case GLFW_KEY_F8:
		key = OF_KEY_F8;
		break;

	case GLFW_KEY_F9:
		key = OF_KEY_F9;
		break;

	case GLFW_KEY_F10:
		key = OF_KEY_F10;
		break;

	case GLFW_KEY_F11:
		key = OF_KEY_F11;
		break;

	case GLFW_KEY_F12:
		key = OF_KEY_F12;
		break;

	case GLFW_KEY_LEFT:
		key = OF_KEY_LEFT;
		break;

	case GLFW_KEY_RIGHT:
		key = OF_KEY_RIGHT;
		break;

	case GLFW_KEY_DOWN:
		key = OF_KEY_DOWN;
		break;

	case GLFW_KEY_UP:
		key = OF_KEY_UP;
		break;

	case GLFW_KEY_PAGEUP:
		key = OF_KEY_PAGE_UP;
		break;

	case GLFW_KEY_PAGEDOWN:
		key = OF_KEY_PAGE_DOWN;
		break;

	case GLFW_KEY_HOME:
		key = OF_KEY_HOME;
		break;

	case GLFW_KEY_END:
		key = OF_KEY_END;
		break;

	case GLFW_KEY_INSERT:
		key = OF_KEY_INSERT;
		break;
	}

	if(action == GLFW_RELEASE) {
		win->keyReleased(key);
	} else {
		win->keyPressed(key);
	}


}
void ofWindowManager::glfwChar(GLFWwindow * glfwWin, int key) {
	ofPtr<ofWindow> win = getWindowByGlfw(glfwWin);
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
