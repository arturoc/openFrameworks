#pragma once

#include "ofWindow.h"
#include "ofBaseTypes.h"

typedef ofPtr <ofWindow> ofWindowPtr;
typedef vector <ofWindowPtr> ofWindowList;

class ofWindowManager : public ofAppBaseWindow {
	public:
		ofWindowManager();
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

		void setFrameRate(float targetRate);
		float getFrameRate();
		int getFrameNum();
		void setActiveWindow(ofPtr<ofWindow> win);

		ofPoint getWindowPosition();
		ofPoint getWindowSize();

		int     getWidth();
		int     getHeight();

		void toggleFullscreen();
		void setFullscreen(bool fullscreen);

	private:
		ofPtr<ofWindow> getWindowByGlfw(GLFWwindow* win);
		void initializeGLFW();
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
		static void glfwMousePosCallback(GLFWwindow * glfwWin, int x, int y);
		static void glfwCursorEnterCallback(GLFWwindow * glfwWin, int action);
		static void glfwScrollCallback(GLFWwindow * glfwWin, double deltaX, double deltaY);
		static void glfwKeyCallback(GLFWwindow * glfwWin, int key, int action);
		static void glfwCharCallback(GLFWwindow * glfwWin, unsigned int character);

		ofWindowList windows;
		ofPtr<ofWindow> mainWindow;
		ofPtr<ofWindow> activeWindow;
};


/* OF C-STYLE FUNCTIONS */
ofPtr<ofWindowManager> ofGetWindowManager();
ofPtr<ofWindow> ofCreateWindow(int x, int y, int width, int height);
ofPtr<ofWindow> ofCreateWindow(int width = 800, int height = 600);
ofPtr<ofWindow> ofGetMainWindow();
ofPtr<ofWindow> ofGetLastCreatedWindow();
