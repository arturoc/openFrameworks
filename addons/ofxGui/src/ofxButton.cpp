#include "ofxButton.h"
using namespace std;

ofxButton::ofxButton(){
	value.setSerializable(false);
}

ofxButton::ofxButton(ofParameter<void> _bVal, float width, float height){
	setup(_bVal, width, height);
}

ofxButton::~ofxButton(){
	//
}

ofxButton* ofxButton::setup(ofParameter<void> _bVal, float width, float height){
	parameter = _bVal;
	value = false;
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	registerMouseEvents();

	value.addListener(this,&ofxButton::valueChanged);

	return this;
}

ofxButton* ofxButton::setup(const std::string& toggleName, float width, float height){
	setName(toggleName);
	value = false;
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	checkboxRect.set(1, 1, b.height - 2, b.height - 2);

	registerMouseEvents();

	value.addListener(this,&ofxButton::valueChanged);

	return this;
}

bool ofxButton::mouseReleased(ofMouseEventArgs & args){
	bool attended = setValue(args.x, args.y, false);
	bGuiActive = false;
	if(attended){
		return true;
	}else{
		return false;
	}
}

#if OFX_TIMELINE
void ofxButton::setTimelined(ofxTimeline * timeline, bool timelined){
	this->timeline = timeline;
	if(timeline==nullptr){
		this->timelined = false;
		return;
	}
	this->timelined = timelined;
	if(timelined){
		timeline->addBangs(parameter);
	}else{
		timeline->remove(parameter);
	}
	setNeedsRedraw();
	timeline->setOffset(glm::vec2(0, ofGetHeight() - timeline->getHeight()));
}

bool ofxButton::refreshTimelined(ofxTimeline * timeline){
	this->timeline = timeline;
	if(timeline->getTrack(parameter)){
		this->timelined = true;
		timeline->linkBangs(parameter);
		return true;
	}else{
		this->timelined = false;
		return false;
	}
}
#endif

bool ofxButton::mouseMoved(ofMouseEventArgs & args){
	return ofxToggle::mouseMoved(args);
}

bool ofxButton::mousePressed(ofMouseEventArgs & args){
	return ofxToggle::mousePressed(args);
}

bool ofxButton::mouseDragged(ofMouseEventArgs & args){
	return ofxToggle::mouseDragged(args);
}

ofAbstractParameter & ofxButton::getParameter(){
	return parameter;
}

void ofxButton::valueChanged(bool & v){
	if(!v){
		parameter.trigger();
	}
}
