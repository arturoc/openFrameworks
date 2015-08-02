#include "ofxLabel.h"
#include "ofGraphics.h"
using namespace std;

ofxLabel::ofxLabel()
:ofxBaseGui(Config()){

}

ofxLabel::ofxLabel(ofParameter<std::string> _label, const Config & config)
:ofxBaseGui(config){
    label.makeReferenceTo(_label);
    setNeedsRedraw();
    label.addListener(this,&ofxLabel::valueChanged);
}

ofxLabel::ofxLabel(ofParameter<string> _label, float width, float height)
:ofxBaseGui(Config()){
	setup(_label,width,height);
}

ofxLabel::~ofxLabel(){
    label.removeListener(this,&ofxLabel::valueChanged);
}

ofxLabel & ofxLabel::setup(ofParameter<string> _label, float width, float height) {
    label.makeReferenceTo(_label);
    b.width  = width;
    b.height = height;
    setNeedsRedraw();
    label.addListener(this,&ofxLabel::valueChanged);
    return *this;
}

ofxLabel & ofxLabel::setup(const std::string& labelName, string _label, float width, float height) {
    label.set(labelName,_label);
    return setup(label,width,height);
}

void ofxLabel::generateDraw(){
	bg.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
    bg.rectangle(b);

    if(bShowName){
        string name;
        if(!getName().empty()){
            name = getName() + ": ";
        }
        textMesh = getTextMesh(name + (string)label, b.x + textPadding, b.y + b.height / 2 + 4);
    }else {
        textMesh = getTextMesh((string)label, b.x + textPadding, b.y + b.height / 2 + 4);
    }
}

void ofxLabel::render() {
	ofColor c = ofGetStyle().color;

	bg.draw();

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
    ofSetColor(textColor);

    bindFontTexture();
    textMesh.draw();
    unbindFontTexture();

    ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

ofAbstractParameter & ofxLabel::getParameter(){
	return label;
}

void ofxLabel::valueChanged(string & value){
    setNeedsRedraw();
}
