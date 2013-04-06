#pragma once

#include "ofPoint.h"
#include "ofEvents.h"
#include "ofBaseTypes.h"

class ofBaseApp : public ofBaseSoundInput, public ofBaseSoundOutput{

	public:
        ofBaseApp() {
            mouseX = mouseY = 0;
        }

		virtual ~ofBaseApp(){}

		virtual void setup(){}
		virtual void update(){}
		virtual void draw(){}
		virtual void exit(){}

		virtual void windowResized(int w, int h){}

		virtual void keyPressed( int key ){}
		virtual void keyReleased( int key ){}

		virtual void mouseMoved( int x, int y ){}
		virtual void mouseDragged( int x, int y, int button ){}
		virtual void mousePressed( int x, int y, int button ){}
		virtual void mouseReleased(){}
		virtual void mouseReleased(int x, int y, int button ){}
		virtual void scrolled(float deltaX, float deltaY){}
		
		virtual void dragEvent(ofDragInfo dragInfo) { }
		virtual void gotMessage(ofMessage msg){ }
	
		virtual void windowEntry ( int state ) { }
		

		// window parameter methods
		virtual void setup(ofWindowEventArgs & f){
			setup();
		}
		virtual void update(ofWindowEventArgs & f){
			update();
		}
		virtual void draw(ofWindowEventArgs & f){
			draw();
		}

		//KEY
		virtual void keyReleased(ofKeyEventArgs & e){
			keyReleased(e.key);
		}
		virtual void keyPressed(ofKeyEventArgs & e){
			keyPressed(e.key);
		}

		//WINDOW
		virtual void windowMoved(ofMoveEventArgs & e){}
		virtual void windowResized(ofResizeEventArgs & e){
			windowResized(e.width, e.height);
		}
		virtual void windowClosed(ofWindowEventArgs & e){}

		//MOUSE
		virtual void mouseMoved(ofMouseEventArgs & e){
			mouseMoved(e.x, e.y);
		}
		virtual void mouseDragged(ofMouseEventArgs & e){
			mouseDragged(e.x, e.y, e.button);
		}
		virtual void mousePressed(ofMouseEventArgs & e){
			mousePressed(e.x, e.y, e.button);
		}
		virtual void mouseReleased(ofMouseEventArgs & e){
			mouseReleased(e.x, e.y, e.button);
		}

		virtual void scrolled(ofScrollEventArgs & e){
			scrolled(e.deltaX, e.deltaY);
		}


		int mouseX, mouseY;			// for processing heads
};



