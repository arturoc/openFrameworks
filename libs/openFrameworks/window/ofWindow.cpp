/*
 *  ofWindow.cpp
 *  openFrameworksLib
 *
 *  Created by Philip Whitfield on 3/8/12.
 *  Copyright 2012 undef.ch. All rights reserved.
 *
 */
#include <ofGraphics.h>
#include "ofEvents.h"
#include "ofWindow.h"
#include "ofWindowManager.h"
#include "ofProgrammableGLRenderer.h"
#include "ofAppRunner.h"

//Philip - Now, I don't know if this is the proper way to expose the native glfw funcitons, but it seems to work.-..

#ifdef TARGET_LINUX
#define GLFW_EXPOSE_NATIVE_X11_GLX 1
#include "glfw3native.h"
#endif

static ofWindow * toOf(GLFWwindow * glfwWin){
	return ((ofWindow*)glfwGetWindowUserPointer(glfwWin));
}


/********** ofWindowDefinitions *****************/
int ofWindow::lastWindowID = 0;

//--------------------------------------------------------------
ofWindow::ofWindow() : mouseX(0),
	mouseY(0),
	previousMouseX(0),
	previousMouseY(0),
	isFocused(false),
	title("ofTestApp"),
	isInitialized(false),
	windowMode(OF_WINDOW) {
	id = lastWindowID;
	lastWindowID++;
	width = 800;
	height = 600;
	x = 0;
	y = 0;
	orientation = OF_ORIENTATION_DEFAULT;
	window = NULL;
	for(unsigned int i = 0; i < OF_MAX_NUM_KEYS; i++) {
		keyState[i] = false;
	}
}

//--------------------------------------------------------------
ofWindow::~ofWindow() {
}

//--------------------------------------------------------------
ofPtr<ofWindow> ofWindow::createWindow(int x, int y, int width, int height){
	ofPtr<ofWindow> win(new ofWindow());

	win->setWindowPositionAndShape(x, y, width, height);
	ofWindowManager::getWindowManager()->addWindow(win);
	win->initializeWindow(OF_WINDOW);
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


//--------------------------------------------------------------
ofPtr<ofWindow> ofWindow::createWindow(int w, int h, ofWindowMode windowMode, int monitor){
	ofPtr<ofWindow> win(new ofWindow());

	win->setWindowShape(w,h);
	ofWindowManager::getWindowManager()->addWindow(win);
	win->initializeWindow(windowMode,monitor);
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

//--------------------------------------------------------------
void ofWindow::initializeWindow(ofWindowMode wm, int monitor) {
	ofLogNotice("CREATING WINDOW AT " + ofToString(x) + "/" + ofToString(y) + " SIZE " + ofToString(width) + " x " + ofToString(height));

	windowMode = wm;

	/*Find out if we can share the context */
	GLFWwindow* win = NULL;
	ofPtr<ofWindow> mainWindow = ofGetMainWindow();
	if(mainWindow != NULL){
		win = mainWindow->getGlfwWindow();
	}

	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	if(wm==OF_GAME_MODE){
		if(count>monitor){
			window = glfwCreateWindow(width, height, title.c_str(), monitors[monitor], win);
		}else if(count>0){
			ofLogWarning() << "can't find specified monitor for fullscreen, using default monitor";
			window = glfwCreateWindow(width, height, title.c_str(), monitors[0], win);
		}else{
			ofLogError() << "can't find any monitor";
			return;
		}
	}else{
		window = glfwCreateWindow(width, height, title.c_str(), NULL, win);
		if(monitor<count){
			int monitorX,monitorY;
			glfwGetMonitorPos(monitors[monitor],&monitorX,&monitorY);
			setWindowPosition(monitorX,monitorY);
		}
		if(wm==OF_FULLSCREEN){
			setFullscreen(true);
		}
	}
	if(window == NULL){
		ofLogError("Could not initialize window");
		return;
	}

	glfwSetWindowUserPointer(window,this);

	glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );

	isInitialized = true;
	renderer = ofPtr<ofProgrammableGLRenderer>(new ofProgrammableGLRenderer);


}

//--------------------------------------------------------------
void ofWindow::addListener(ofBaseApp * app) {
	enableContext();
	app->setup();

	ofAddListener(events.update, app, &ofBaseApp::update);
	ofAddListener(events.draw, app, &ofBaseApp::draw);

	ofAddListener(events.mouseMoved, app, &ofBaseApp::mouseMoved);
	ofAddListener(events.mousePressed, app, &ofBaseApp::mousePressed);
	ofAddListener(events.mouseDragged, app, &ofBaseApp::mouseDragged);
	ofAddListener(events.mouseReleased, app, &ofBaseApp::mouseReleased);

	ofAddListener(events.keyPressed, app, &ofBaseApp::keyPressed);
	ofAddListener(events.keyReleased, app, &ofBaseApp::keyReleased);

	ofAddListener(events.windowResized, app, &ofBaseApp::windowResized);
	ofAddListener(events.windowMoved, app, &ofBaseApp::windowMoved);
	ofAddListener(events.windowClosed, app, &ofBaseApp::windowClosed);
}

//--------------------------------------------------------------
GLFWwindow* ofWindow::getGlfwWindow() {
	return window;
}

//--------------------------------------------------------------
void ofWindow::enableContext() {
	glfwMakeContextCurrent(window);
	ofSetCurrentRenderer(renderer);
}

//--------------------------------------------------------------
void ofWindow::setup() {
	glfwMakeContextCurrent(window);
	renderer->setup();
	renderer->setupGraphicDefaults();
	ofSetCurrentRenderer(renderer);
	ofSetStyle(ofStyle());
    ofSetOrientation(OF_ORIENTATION_DEFAULT,true);
	glClearColor(.55, .55, .55, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	ofWindowEventArgs e;
	e.window = this;
	ofNotifyEvent(events.setup, e);
}

//--------------------------------------------------------------
void ofWindow::update() {
	enableContext();
	ofWindowEventArgs e;
	e.window = this;
	ofNotifyEvent(events.update, e);
}

//--------------------------------------------------------------
void ofWindow::draw() {
	enableContext();

	if(renderer->getType()==ofProgrammableGLRenderer::TYPE){
		renderer->startRender();
	}

	renderer->viewport();     // used to be glViewport( 0, 0, width, height );
	renderer->clear(renderer->getBgColor().r*255,renderer->getBgColor().g*255,renderer->getBgColor().b*255,renderer->getBgColor().a*255);
	renderer->setupScreenPerspective(width, height);


	ofWindowEventArgs e;
	e.window = this;
	ofNotifyEvent(events.draw, e);

	if(renderer->getType()==ofProgrammableGLRenderer::TYPE){
		renderer->finishRender();
	}
	glfwSwapBuffers(window);
}

//--------------------------------------------------------------
void ofWindow::destroy() {
	//getWindowPosition();
	glfwDestroyWindow(window);
	window = NULL;
}


//USER INTERACTION EVENTS
//--------------------------------------------------------------
void ofWindow::mouseMoved(int mX, int mY) {
	updateMouse(mX, mY);
	ofNotifyMouseMoved(mouseX, mouseY);

	ofMouseEventArgs e;
	e.x = mX;
	e.y = mY;
	e.window = this;
	ofNotifyEvent(events.mouseMoved, e);
}

//--------------------------------------------------------------
void ofWindow::mousePressed(int button) {
	mousePressed(mouseX, mouseY, button);
}

//--------------------------------------------------------------
void ofWindow::mousePressed(int mX, int mY, int button) {
	updateMouse(mX, mY);
	ofNotifyMousePressed(mouseX, mouseY, button);

	ofMouseEventArgs e;
	e.x = mX;
	e.y = mY;
	e.button = button;
	e.window = this;
	ofNotifyEvent(events.mousePressed, e);
}

//--------------------------------------------------------------
void ofWindow::mouseReleased(int button) {
	mouseReleased(mouseX, mouseY, button);
}

//--------------------------------------------------------------
void ofWindow::mouseReleased(int mX, int mY, int button) {
	updateMouse(mX, mY);
	ofNotifyMouseReleased(mouseX, mouseY, button);

	ofMouseEventArgs e;
	e.x = mX;
	e.y = mY;
	e.button = button;
	e.window = this;
	ofNotifyEvent(events.mouseReleased, e);
}

//--------------------------------------------------------------
void ofWindow::mouseDragged(int mX, int mY, int button) {
	updateMouse(mX, mY);

	ofNotifyMouseDragged(mouseX, mouseY, button);

	ofMouseEventArgs e;
	e.x = mouseX;
	e.y = mouseY;
	e.button = button;
	e.window = this;
	ofNotifyEvent(events.mouseDragged, e);
}

//--------------------------------------------------------------
void ofWindow::scrolled(float deltaX, float deltaY) {
#ifndef TARGET_OSX
	deltaX *= -1;
#endif

	ofNotifyScrolled(deltaX, deltaY);
	ofScrollEventArgs e;
	e.deltaX = deltaX;
	e.deltaY = deltaY;
	e.window = this;
	ofNotifyEvent(events.scrolled, e);
}

//--------------------------------------------------------------
void ofWindow::keyPressed(int key) {
	if(key < OF_MAX_NUM_KEYS) {
		keyState[key] = true;
	}
	ofNotifyKeyPressed(key);

	ofKeyEventArgs e;
	e.key = key;
	e.window = this;
	ofNotifyEvent(events.keyPressed, e);
}

//--------------------------------------------------------------
void ofWindow::keyReleased(int key) {
	if(key < OF_MAX_NUM_KEYS) {
		keyState[key] = false;
	}
	ofNotifyKeyReleased(key);

	ofKeyEventArgs e;
	e.key = key;
	e.window = this;
	ofNotifyEvent(events.keyReleased, e);
}

//--------------------------------------------------------------
bool ofWindow::isKeyPressed(int key) {
	return keyState[key];
}


//WINDOW SPECIFIC EVENT
//--------------------------------------------------------------
void ofWindow::windowFocused() {
	isFocused = true;
}

//--------------------------------------------------------------
void ofWindow::windowUnfocused() {
	isFocused = false;
}

//--------------------------------------------------------------
void ofWindow::windowClosed() {
	ofWindowEventArgs e;
	e.window = this;
	ofNotifyEvent(events.windowClosed, e);
	ofWindowManager::getWindowManager()->removeWindow(id);

	glfwDestroyWindow(window);
	window = NULL;
}

//--------------------------------------------------------------
ofPoint ofWindow::getWindowPosition() {
	if(window != NULL) {
		glfwGetWindowPos(window, &x, &y);
	}
	return ofPoint(x, y);
}

//--------------------------------------------------------------
ofPoint ofWindow::getWindowSize() {
	return ofPoint(width, height);
}

//--------------------------------------------------------------
int ofWindow::getWidth() {
	return width;
}

//--------------------------------------------------------------
int ofWindow::getHeight() {
	return height;
}

//--------------------------------------------------------------
int ofWindow::getX() {
	return getWindowPosition().x;
}

//--------------------------------------------------------------
int ofWindow::getY() {
	return getWindowPosition().y;
}

//--------------------------------------------------------------
int ofWindow::setX(int _x) {
	setWindowPosition(_x, y);
	return _x; //TODO: this should return the actual window position
}

//--------------------------------------------------------------
int ofWindow::setY(int _y) {
	setWindowPosition(x, _y);
	return _y; //TODO: this should return the actual window position
}

//--------------------------------------------------------------
void ofWindow::setWindowPositionAndShape(int _x, int _y, int w, int h) {
	if(!isInitialized) {
		x = _x;
		y = _y;
		width = w;
		height = h;
	} else {
		setWindowPosition(_x, _y);
		setWindowShape(w, h);
	}
}

//--------------------------------------------------------------
void ofWindow::setWindowPosition(int x, int y) {
	glfwSetWindowPos(window, x, y);
}

//--------------------------------------------------------------
void ofWindow::setWindowPosition(ofPoint pos) {
	setWindowPosition(pos.x, pos.y);
}

//--------------------------------------------------------------
void ofWindow::setOrientation(ofOrientation _orientation){
	orientation = _orientation;
}

//--------------------------------------------------------------
ofOrientation ofWindow::getOrientation(){
	return orientation;
}

//--------------------------------------------------------------
void ofWindow::setWindowShape(int w, int h) {
	previousShape.width = width;
	previousShape.height = height;
	width = w;
	height = h;
	if(window)
		glfwSetWindowSize(window, w, h);
}

//--------------------------------------------------------------
void ofWindow::setWidth(int w) {
	previousShape.width = width;
	width = w;
	glfwSetWindowSize(window, w, height);
}

//--------------------------------------------------------------
void ofWindow::setHeight(int h) {
	previousShape.height = height;
	height = h;
	glfwSetWindowSize(window, width, h);
}

//--------------------------------------------------------------
void ofWindow::hideCursor(){
	glfwSetInputMode(window, GLFW_CURSOR_MODE, GLFW_CURSOR_HIDDEN);
}

//--------------------------------------------------------------
void ofWindow::showCursor(){
	glfwSetInputMode(window, GLFW_CURSOR_MODE, GLFW_CURSOR_NORMAL);
}

//--------------------------------------------------------------
void ofWindow::windowResized(int w, int h) {
	if(width == w && height == h) {
		return;
	}

	previousShape.width = width;
	previousShape.height = height;
	width = w;
	height = h;
	ofNotifyWindowResized(width, height);

	ofResizeEventArgs e;
	e.width = width;
	e.height = height;
	ofNotifyEvent(events.windowResized, e);
	draw();
}

//--------------------------------------------------------------
void ofWindow::windowMoved(int _x, int _y) {
	if(x == _x && y == _y) {
		return;
	}
	previousShape.x = x;
	previousShape.y = y;
	x = _x;
	y = _y;

	ofMoveEventArgs e;
	e.x = x;
	e.y = y;
	ofNotifyEvent(events.windowMoved, e);
}

//--------------------------------------------------------------
void ofWindow::setTitle(string t) {
	if(isInitialized) {
		glfwSetWindowTitle(window, t.c_str());
	}
	title = t;
}

//--------------------------------------------------------------
string ofWindow::getTitle() {
	return title;
}

//--------------------------------------------------------------
void ofWindow::setVerticalSync(bool vSync){
	if(vSync) glfwSwapInterval( 1 );
	else  glfwSwapInterval( 0 );
}

//--------------------------------------------------------------
int ofWindow::getID(){
	return id;
}

//--------------------------------------------------------------
bool ofWindow::isClosed() {
	return window == NULL;
}

//--------------------------------------------------------------
void ofWindow::updateMouse(int x, int y) {
	previousMouseX = mouseX;
	previousMouseY = mouseY;
	mouseX = x;
	mouseY = y;
}

//--------------------------------------------------------------
bool ofWindow::isFullscreen() {
	if(windowMode  == OF_FULLSCREEN)
		return true;
	if(windowMode == OF_GAME_MODE)
		return true;
	return false;
}

//--------------------------------------------------------------
void ofWindow::setFullscreen(bool fullscreen) {
	cout << "FULLSCREEN " << fullscreen <<endl;
#ifdef TARGET_LINUX
#include <X11/Xatom.h>

	Window nativeWin = glfwGetX11Window(window);
	Display* display = glfwGetX11Display();


	Atom m_net_state= XInternAtom(display, "_NET_WM_STATE", false);
	Atom m_net_fullscreen= XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", false);

	XEvent xev;

	xev.xclient.type = ClientMessage;
	xev.xclient.serial = 0;
	xev.xclient.send_event = True;
	xev.xclient.window = nativeWin;
	xev.xclient.message_type = m_net_state;
	xev.xclient.format = 32;

	if (fullscreen)
		xev.xclient.data.l[0] = 1;
	else
		xev.xclient.data.l[0] = 0;

	xev.xclient.data.l[1] = m_net_fullscreen;
	xev.xclient.data.l[2] = 0;
	xev.xclient.data.l[3] = 0;
	xev.xclient.data.l[4] = 0;
	XSendEvent(display, RootWindow(display, DefaultScreen(display)),
	           False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);

#endif

	if (fullscreen)
		windowMode = OF_FULLSCREEN;
	else
		windowMode = OF_WINDOW;
}

//--------------------------------------------------------------
void ofWindow::setWindowPositionAndShape(ofRectangle rect) {
}

//--------------------------------------------------------------
void ofWindow::toggleFullscreen() {
	if(windowMode == OF_WINDOW)
		setFullscreen(true);
	else if(windowMode == OF_FULLSCREEN)
		setFullscreen(false);
}




////GLFW CALLBACKS
//--------------------------------------------------------------
void ofWindow::glfwErrorCallback(int type, const char * err) {
	ofLog(OF_LOG_ERROR, err);
}

//--------------------------------------------------------------
void ofWindow::glfwWindowSizeCallback(GLFWwindow * glfwWin, int w, int h) {
	toOf(glfwWin)->windowResized(w, h);
}

//--------------------------------------------------------------
void ofWindow::glfwWindowCloseCallback(GLFWwindow * glfwWin) {
	toOf(glfwWin)->windowClosed();
}

//--------------------------------------------------------------
void ofWindow::glfwWindowRefreshCallback(GLFWwindow * glfwWin) {
	//toOf(glfwWin)->glfwWindowRefresh(glfwWin);
}

//--------------------------------------------------------------
void ofWindow::glfwWindowFocusCallback(GLFWwindow * glfwWin, int action) {
	//toOf(glfwWin)->glfwWindowFocus(glfwWin, action);
}

//--------------------------------------------------------------
void ofWindow::glfwWindowIconifyCallback(GLFWwindow * glfwWin, int action) {
	//toOf(glfwWin)->glfwWindowIconify(glfwWin, action);
}

//--------------------------------------------------------------
void ofWindow::glfwMouseButtonCallback(GLFWwindow * glfwWin, int button, int action) {
	if(action == GLFW_PRESS) {
		toOf(glfwWin)->mousePressed(button);
	} else {
		toOf(glfwWin)->mouseReleased(button);
	}
}

//--------------------------------------------------------------
void ofWindow::glfwMousePosCallback(GLFWwindow * glfwWin, double x, double y) {
	toOf(glfwWin)->mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofWindow::glfwCursorEnterCallback(GLFWwindow * glfwWin, int action) {
	//toOf(glfwWin)->glfwCursorEnter(glfwWin, action);
}

//--------------------------------------------------------------
void ofWindow::glfwScrollCallback(GLFWwindow * glfwWin, double deltaX, double deltaY) {
	toOf(glfwWin)->scrolled(-deltaX, -deltaY);
}

//--------------------------------------------------------------
void ofWindow::glfwKeyCallback(GLFWwindow * glfwWin, int key, int action) {
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
		toOf(glfwWin)->keyReleased(key);
	} else {
		toOf(glfwWin)->keyPressed(key);
	}
}

//--------------------------------------------------------------
void ofWindow::glfwCharCallback(GLFWwindow * glfwWin, unsigned int character) {
	//toOf(glfwWin)->glfwChar(glfwWin, character);
}


