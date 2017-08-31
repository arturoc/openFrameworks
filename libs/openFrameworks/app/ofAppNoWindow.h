#pragma once

#include "ofConstants.h"
#include "ofAppBaseWindow.h"
#include "ofBaseTypes.h"
#include "of3dGraphics.h"
#include "ofPath.h"

class ofBaseApp;

class ofAppNoWindow : public ofAppBaseWindow {

public:
	ofAppNoWindow();
	~ofAppNoWindow(){}

	static bool doesLoop(){ return false; }
	static bool allowsMultiWindow(){ return false; }
	static void loop(){};
	static bool needsPolling(){ return false; }
	static void pollEvents(){};

	void run(ofBaseApp * appPtr);

	static void exitApp();
	void setup(const ofWindowSettings & settings);
	void update();
	void draw();

	glm::vec2	getWindowPosition();
	glm::vec2	getWindowSize();
	glm::vec2	getScreenSize();

	int			getWidth();
	int			getHeight();

	ofCoreEvents & events();
	std::shared_ptr<ofBaseRenderer> & renderer();

private:
	int width, height;

    ofBaseApp *		ofAppPtr;
    ofCoreEvents coreEvents;
    std::shared_ptr<ofBaseRenderer> currentRenderer;
};

class ofNoopRenderer: public ofBaseRenderer{
public:
	ofNoopRenderer():graphics3d(this){}
private:
	static const std::string TYPE;
	const std::string & getType(){return TYPE;}

	void startRender(){}
	void finishRender(){}
	void update(){}

	void draw(const ofPolyline_<ofVec3f> & poly) const{}
	void draw(const ofPolyline_<ofVec2f> & poly) const{}
	void draw(const ofPolyline_<glm::vec3> & poly) const{}
	void draw(const ofPolyline_<glm::vec2> & poly) const{}
	void draw(const ofPath & shape) const{}
	void draw(const of3dPrimitive&, ofPolyRenderMode) const{}
	void draw(const ofNode&) const{}
	void draw(const ofMesh_<ofVec3f,ofVec3f,ofFloatColor,ofVec2f> & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const{}
	void draw(const ofMesh_<glm::vec3, glm::vec3, ofFloatColor, glm::vec2> & vertexData, ofPolyRenderMode renderType, bool useColors, bool useTextures, bool useNormals) const{}
	void draw(const ofImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{}
	void draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{}
	void draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{}
	void draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const{}

	//--------------------------------------------
	// transformations
	void pushView(){};
	void popView(){};

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(ofRectangle viewport){};
	void viewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true){};
	void setupScreenPerspective(float width = 0, float height = 0, float fov = 60, float nearDist = 0, float farDist = 0){}
	void setupScreenOrtho(float width = 0, float height = 0, float nearDist = -1, float farDist = 1){};
	ofRectangle getCurrentViewport() const{return ofRectangle();};
	ofRectangle getNativeViewport() const{return ofRectangle();};
	int getViewportWidth() const{return 0;};
	int getViewportHeight() const{return 0;};

	void setCoordHandedness(ofHandednessType handedness){};
	ofHandednessType getCoordHandedness() const{return OF_LEFT_HANDED;};

	//our openGL wrappers
	void pushMatrix(){};
	void popMatrix(){};
	void translate(float x, float y, float z = 0){};
	void translate(const glm::vec3 & p){};
	void scale(float xAmnt, float yAmnt, float zAmnt = 1){};
	void rotateRad(float radians, float vecX, float vecY, float vecZ){};
	void rotateXRad(float radians){};
	void rotateYRad(float radians){};
	void rotateZRad(float radians){};
	void rotateRad(float radians){};

	// screen coordinate things / default gl values
	void setupGraphicDefaults(){};
	void setupScreen(){};

	void setOrientation(ofOrientation, bool){}
	bool isVFlipped() const{return true;}
	glm::mat4 getCurrentMatrix(ofMatrixMode) const{return glm::mat4(1.0);};
	glm::mat4 getCurrentOrientationMatrix() const{return glm::mat4(1.0);}
	void matrixMode(ofMatrixMode){}
	void loadIdentityMatrix(){}
	void loadMatrix(const glm::mat4&){}
	void loadMatrix(const float*){}
	void multMatrix(const glm::mat4&){}
	void multMatrix(const float*){}
	void loadViewMatrix(const glm::mat4&){}
	void multViewMatrix(const glm::mat4&){}
	glm::mat4 getCurrentViewMatrix() const{return glm::mat4(1.0);}
	glm::mat4 getCurrentNormalMatrix() const{return glm::mat4(1.0);}
	void enableAntiAliasing(){}
	void disableAntiAliasing(){}


	// drawing modes
	void setRectMode(ofRectMode mode){}
	ofRectMode getRectMode(){return OF_RECTMODE_CORNER;}
	void setFillMode(ofFillFlag fill){}
	ofFillFlag getFillMode(){return OF_FILLED;}
	void setLineWidth(float lineWidth){}
	void setBlendMode(ofBlendMode blendMode){}
	void setLineSmoothing(bool smooth){}
	void setCircleResolution(int res){};
	void enablePointSprites(){};
	void disablePointSprites(){};
	void setDepthTest(bool){};

	// color options
	void setColor(int r, int g, int b){}; // 0-255
	void setColor(int r, int g, int b, int a){}; // 0-255
	void setColor(const ofColor & color){};
	void setColor(const ofColor & color, int _a){};
	void setColor(int gray){}; // new set a color as grayscale with one argument
	void setHexColor( int hexColor ){}; // hex, like web 0xFF0033;

	// bg color
	ofColor getBackgroundColor(){return ofColor(200);}
	void setBackgroundColor(const ofColor & color){}
	bool getBackgroundAuto(){
		return true;
	}
	void background(const ofColor & c){};
	void background(float brightness){};
	void background(int hexColor, float _a=255.0f){};
	void background(int r, int g, int b, int a=255){};

	void setBackgroundAuto(bool bManual){};		// default is true

	void clear(){};
	void clear(float r, float g, float b, float a=0){};
	void clear(float brightness, float a=0){};
	void clearAlpha(){};

	// drawing
	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const{}
	void drawRectangle(float x, float y, float z, float w, float h) const{}
	void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const{}
	void drawCircle(float x, float y, float z, float radius) const{}
	void drawSphere(float x, float y, float z, float radius) const{}
	void drawEllipse(float x, float y, float z, float width, float height) const{}
	void drawString(std::string text, float x, float y, float z) const{}
	void drawString(const ofTrueTypeFont & font, std::string text, float x, float y) const{}

	void setBitmapTextMode(ofDrawBitmapMode mode){}
	ofStyle getStyle() const{ return ofStyle(); }
	void pushStyle(){}
	void popStyle(){}
	void setStyle(const ofStyle & style){}
	void setCurveResolution(int res){}
	void setPolyMode(ofPolyWindingMode){}

	// returns true if the renderer can render curves without decomposing them
	ofPath & getPath(){ return path; }
	void bind(const ofCamera & camera, const ofRectangle & viewport){};
	void unbind(const ofCamera & camera){};
	const of3dGraphics & get3dGraphics() const{
		return graphics3d;
	}

	of3dGraphics & get3dGraphics(){
		return graphics3d;
	}
	of3dGraphics graphics3d;
	ofPath path;
};

