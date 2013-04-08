#pragma once

#include "ofWindow.h"
#include "ofBaseTypes.h"

typedef ofPtr <ofWindow> ofWindowPtr;

class ofWindowManager : public ofAppBaseWindow {
	public:
		static ofPtr<ofWindowManager> getWindowManager();
		~ofWindowManager();


		void initializeWindow();
		void setupOpenGL(int w, int h, ofWindowMode screenMode);
		void setupOpenGL(int w, int h, ofWindowMode screenMode, int monitor);
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
		void addWindow(ofPtr<ofWindow> win);
		void removeWindow(int id);
		void exit(ofEventArgs& e);


		typedef vector <ofWindowPtr> ofWindowList;
		ofWindowList windows;
		ofPtr<ofWindow> mainWindow;
		ofPtr<ofWindow> activeWindow;

		friend class ofWindow;
};


/* OF C-STYLE FUNCTIONS */
ofPtr<ofWindow> ofCreateWindow(int x, int y, int width, int height);
ofPtr<ofWindow> ofCreateWindow(int width, int height);
ofPtr<ofWindow> ofGetMainWindow();
ofPtr<ofWindow> ofGetLastCreatedWindow();
ofPtr<ofWindow> ofGetCurrentWindow();

