#pragma once

#include <ofBaseApp.h>
#include "ofAppBaseWindow.h"
#include "ofTypes.h"
#include <GL/glfw3.h>
#include <map>

#define OF_MAX_NUM_KEYS 512


class ofWindow  {
	public:
		~ofWindow();

		static ofPtr<ofWindow> createWindow(int w, int h, ofWindowMode windowMode = OF_WINDOW);
		static ofPtr<ofWindow> createWindow(int x, int y, int width, int height, ofWindowMode windowMode = OF_WINDOW);

		void enableContext();
		void initializeWindow(ofWindowMode mode = OF_WINDOW);

		void setWindowPositionAndShape(ofRectangle rect);
		void setWindowPositionAndShape(int x, int y, int width, int height);

		void addListener(ofBaseApp * app);

		void setup();

		void update();
		void draw();

		ofPoint getWindowPosition();
		void setWindowPosition(int x, int y);
		void setWindowPosition(ofPoint pos);

		void setOrientation(ofOrientation);
		ofOrientation getOrientation();

		ofPoint getWindowSize();
		void setWindowShape(int x, int y);
		void setWidth(int w);
		void setHeight(int h);

		void hideCursor();
		void showCursor();

		void setVerticalSync(bool vSync);
		void setFullscreen(bool fullscreen);
		void toggleFullscreen();
		bool isFullscreen();

		int getWidth();
		int getHeight();

		int getX();
		int getY();
		int setX(int x);
		int setY(int y);

		bool isKeyPressed(int key);
		bool isClosed();

		void setTitle(string title);
		string getTitle();

		int getID();

		ofWindowEvents events;

		static int lastWindowID;

		int mouseX, mouseY;
		int previousMouseX, previousMouseY;

	private:
		ofWindow();
		GLFWwindow* getGlfwWindow();

		void destroy();

		void mouseMoved(int mouseX, int mouseY);
		void mouseDragged(int mouseX, int mouseY, int button);
		void mousePressed(int button);
		void mousePressed(int mouseX, int mouseY, int button);
		void mouseReleased(int button);
		void mouseReleased(int mouseX, int mouseY, int button);

		void keyPressed(int key);
		void keyReleased(int key);

		void scrolled(float deltaX, float deltaY);

		void windowMoved(int x, int y);
		void windowFocused();
		void windowUnfocused();
		void windowClosed();
		void windowResized(int w, int h);
		
		void updateMouse(int x, int y);

		ofRectangle previousShape;
		bool isFocused;
		string title;
		GLFWwindow* window;
		ofPtr<ofProgrammableGLRenderer> renderer;

		bool isInitialized;
		ofWindowMode windowMode;
		int id;
		int width;
		int height;
		int x;
		int y;

		bool keyState[OF_MAX_NUM_KEYS];
		ofOrientation orientation;


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


		friend class ofWindowManager;
};
