#include "ofxToggle.h"
#include "ofGraphics.h"
using namespace std;

ofxToggle::ofxToggle(ofParameter<bool> _bVal, float width, float height){
	setup(_bVal,width,height);
}

ofxToggle::~ofxToggle(){
	value.removeListener(this,&ofxToggle::valueChanged);
}

ofxToggle * ofxToggle::setup(ofParameter<bool> _bVal, float width, float height){
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	value.makeReferenceTo(_bVal);

	value.addListener(this,&ofxToggle::valueChanged);
	registerMouseEvents();
	setNeedsRedraw();

	return this;

}

ofxToggle * ofxToggle::setup(const std::string& toggleName, bool _bVal, float width, float height){
	value.set(toggleName,_bVal);
	return setup(value,width,height);
}


bool ofxToggle::mouseMoved(ofMouseEventArgs & args){
	if(isGuiDrawing() && ofxBaseGui::usingTimeline()){
		auto tlBox = getTLIconBox(b.position.xy());
		overTLIcon = tlBox.inside(args);
	}

	if(isGuiDrawing() && b.inside(ofPoint(args.x,args.y))){
		mouseOver = true;
		return true;
	}else{
		mouseOver = false;
		return false;
	}
}

bool ofxToggle::mousePressed(ofMouseEventArgs & args){
#ifdef OFX_TIMELINE
	if(isGuiDrawing() && overTLIcon){
		setTimelined(timeline, !timelined);
		return true;
	}
#endif

	if(setValue(args.x, args.y, true)){
		return true;
	}else{
		return false;
	}
}

bool ofxToggle::mouseDragged(ofMouseEventArgs & args){
	if(bGuiActive && b.inside(ofPoint(args.x,args.y))){
		return true;
	}else{
		return false;
	}
}

bool ofxToggle::mouseReleased(ofMouseEventArgs & args){
	bool wasGuiActive = bGuiActive;
	bGuiActive = false;
	if(wasGuiActive && b.inside(ofPoint(args.x,args.y))){
		return true;
	}else{
		return false;
	}
}

void ofxToggle::generateDraw(){
	bg.clear();
	bg.setFillColor(timelined ? thisTimelinedBgColor : thisBackgroundColor);
	bg.rectangle(b);

	auto side = b.height - 2;
	checkboxRect.set(b.width - side - 1, 1, side, side);
	fg.clear();
	if(value){
		fg.setFilled(true);
		fg.setFillColor(timelined ? thisTimelinedColor : thisFillColor);
	}else{
		fg.setFilled(false);
		fg.setStrokeWidth(1);
		fg.setStrokeColor(timelined ? thisTimelinedColor : thisFillColor);
	}
	fg.rectangle(b.getPosition()+checkboxRect.getTopLeft(),checkboxRect.width,checkboxRect.height);

	cross.clear();
	cross.setStrokeColor(thisTextColor);
	cross.setStrokeWidth(1);
	cross.setFilled(false);
	cross.moveTo(b.getPosition()+checkboxRect.getTopLeft());
	cross.lineTo(b.getPosition()+checkboxRect.getBottomRight());
	cross.moveTo(b.getPosition()+checkboxRect.getTopRight());
	cross.lineTo(b.getPosition()+checkboxRect.getBottomLeft());

	std::string name;
	auto textX = b.x + textPadding;
	if(getTextBoundingBox(getName(), textX, 0).getMaxX() > b.getMaxX() - textPadding){
		for(auto c: ofUTF8Iterator(getName())){
			auto next = name;
			ofUTF8Append(next, c);
			if(getTextBoundingBox(next,textX,0).getMaxX() > b.getMaxX() - textPadding){
				break;
			}else{
				name = next;
			}
		}
	}else{
		name = getName();
	}

	textMesh = getTextMesh(name, textX, b.y+b.height / 2 + 4);
}

void ofxToggle::render(){
	bg.draw();
	fg.draw();

	if( value ){
		cross.draw();
	}

	if(overTLIcon){
		tlIcon.setColor(thisTextColor);
		tlIcon.draw(b.x, b.y);
	}else{
		ofColor c = ofGetStyle().color;
		ofBlendMode blendMode = ofGetStyle().blendingMode;
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableAlphaBlending();
		}
		ofSetColor(thisTextColor);

		bindFontTexture();
		textMesh.draw();
		unbindFontTexture();

		ofSetColor(c);
		if(blendMode!=OF_BLENDMODE_ALPHA){
			ofEnableBlendMode(blendMode);
		}
	}
}

bool ofxToggle::operator=(bool v){
	value = v;
	return v;
}

ofxToggle::operator const bool & (){
	return value;
}

bool ofxToggle::setValue(float mx, float my, bool bCheck){

	if( !isGuiDrawing() ){
		bGuiActive = false;
		return false;
	}
	if( bCheck ){
		ofRectangle checkRect = checkboxRect;
		checkRect.x += b.x;
		checkRect.y += b.y;

		if( checkRect.inside(mx, my) ){
			bGuiActive = true;
		}else{
			bGuiActive = false;

		}
	}
	if( bGuiActive ){
		value = !value;
		return true;
	}
	return false;
}

ofAbstractParameter & ofxToggle::getParameter(){
	return value;
}

#if OFX_TIMELINE
void ofxToggle::setTimelined(ofxTimeline * timeline, bool timelined){
	this->timeline = timeline;
	if(timeline==nullptr){
		this->timelined = false;
		return;
	}
	this->timelined = timelined;
	if(timelined){
		timeline->addSwitches(value);
	}else{
		timeline->remove(value);
	}
	setNeedsRedraw();
	timeline->setOffset(glm::vec2(0, ofGetHeight() - timeline->getHeight()));
}

bool ofxToggle::refreshTimelined(ofxTimeline * timeline){
	if(timeline->getTrack(value)){
		this->timeline = timeline;
		this->timelined = true;
		timeline->linkSwitches(value);
		return true;
	}else{
		return false;
	}
}
#endif

void ofxToggle::valueChanged(bool & value){
    setNeedsRedraw();
}
