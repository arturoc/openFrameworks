#pragma once

#include "ofWindow.h"
#include "ofBaseTypes.h"

typedef ofPtr <ofWindow> ofWindowPtr;

class ofWindowManager : public ofAppBaseWindow {
	public:
		static ofPtr<ofWindowManager> getWindowManager();
		~ofWindowManager();

		ofPtr<ofWindow> createWindow(int w = 800, int h = 600, ofWindowMode windowMode = OF_WINDOW);
		ofPtr<ofWindow> createWindow(int x, int y, int width, int height, ofWindowMode windowMode = OF_WINDOW);
		void deleteWindow(ofPtr<ofWindow> win);
		void deleteWindow(int id);

		void initializeWindow();
		void setupOpenGL(int w, int h, int screenMode);
		void runAppViaInfiniteLoop(ofBaseApp * appPtr);

		void update();
		void draw();

		ofPtr<ofWindow> getLastCreatedWindow();
		ofPtr<ofWindow> getMainWindow();
		ofPtr<ofWindow> getWindowById(int id);
		ofPtr<ofWindow> getCurrentWindow();

		void setOrientation(ofOrientation);
		ofOrientation getOrientation();
		bool doesHWOrientation(){return false;};

		void hideCursor();
		void showCursor();

		void setWindowPosition(int x, int y);
		void setWindowShape(int w, int h);

		//TODO:


		ofPoint getScreenSize(){};

		void setWindowTitle(std::string title){};
		int getWindowMode(){};
		void enableSetupScreen(){};
		void disableSetupScreen(){};
		// end TODO

		void setVerticalSync(bool vSync);

		void setFrameRate(float targetRate);
		float getFrameRate();
		int getFrameNum();
		double  getLastFrameTime();

		ofPoint getWindowPosition();
		ofPoint getWindowSize();

		int     getWidth();
		int     getHeight();

		void toggleFullscreen();
		void setFullscreen(bool fullscreen);

	private:
		ofWindowManager();
		ofPtr<ofWindow> getWindowByGlfw(GLFWwindow* win);
		void addWindow(ofPtr<ofWindow> win);
		void removeWindow(ofPtr<ofWindow> win);
		void exit(ofEventArgs& e);

		//would like to have these functions private. but don't know how...
		void glfwWindowFocus(GLFWwindow * glfwWin, int action);
		void glfwWindowSize(GLFWwindow * glfwWin, int w, int h);
		int glfwWindowClose(GLFWwindow * glfwWin);
		void glfwWindowRefresh(GLFWwindow * glfwWin);
		void glfwWindowIconify(GLFWwindow * glfwWin, int action);
		void glfwMouseButton(GLFWwindow * glfwWin, int button, int action);
		void glfwMousePos(GLFWwindow * glfwWin, int mouseX, int mouseY);
		void glfwCursorEnter(GLFWwindow * glfwWin, int action);
		void glfwScroll(GLFWwindow * glfwWin, float deltaX, float deltaY);
		void glfwKey(GLFWwindow * glfwWin, int key, int action);
		void glfwChar(GLFWwindow * glfwWin, int key);

		static void glfwErrorCallback(int type, const char * err);
		static void glfwWindowSizeCallback(GLFWwindow * glfwWin, int w, int h);
		static void glfwWindowCloseCallback(GLFWwindow * glfwWin);
		static void glfwWindowRefreshCallback(GLFWwindow * glfwWin);
		static void glfwWindowFocusCallback(GLFWwindow * glfwWin, int action);
		static void glfwWindowIconifyCallback(GLFWwindow * glfwWin, int action);
		static void glfwMouseButtonCallback(GLFWwindow * glfwWin, int button, int action);
		static void glfwMousePosCallback(GLFWwindow * glfwWin, double x, double y);
		static void glfwCursorEnterCallback(GLFWwindow * glfwWin, int action);
		static void glfwScrollCallback(GLFWwindow * glfwWin, double deltaX, double deltaY);
		static void glfwKeyCallback(GLFWwindow * glfwWin, int key, int action);
		static void glfwCharCallback(GLFWwindow * glfwWin, unsigned int character);

		typedef vector <ofWindowPtr> ofWindowList;
		ofWindowList windows;
		ofPtr<ofWindow> mainWindow;
		ofPtr<ofWindow> activeWindow;
};


/* OF C-STYLE FUNCTIONS */
ofPtr<ofWindow> ofCreateWindow(int x, int y, int width, int height);
ofPtr<ofWindow> ofCreateWindow(int width, int height);
ofPtr<ofWindow> ofGetMainWindow();
ofPtr<ofWindow> ofGetLastCreatedWindow();
ofPtr<ofWindow> ofGetCurrentWindow();

