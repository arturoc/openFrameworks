#include "ofxSlider.h"
#include "ofGraphics.h"
using namespace std;

namespace{
	template<typename Type>
	std::string toString(Type t){
		return ofToString(t);
	}

	template<>
	std::string toString(uint8_t t){
		return ofToString((int) t);
	}

	template<>
	std::string toString(int8_t t){
		return ofToString((int) t);
	}
}

template<typename Type>
ofxSlider<Type>::ofxSlider(){
}

template<typename Type>
ofxSlider<Type>::~ofxSlider(){
	value.removeListener(this,&ofxSlider::valueChanged);
}

template<typename Type>
ofxSlider<Type>::ofxSlider(ofParameter<Type> _val, float width, float height){
	setup(_val,width,height);
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(ofParameter<Type> _val, float width, float height){
	listener = input.leftFocus.newListener([this]{
		state = Slider;
		if(!input.containsValidValue()){
			errorTime = ofGetElapsedTimef();
		}else{
			errorTime = 0;
		}
	});
	bUpdateOnReleaseOnly = false;
	value.makeReferenceTo(_val);
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	setNeedsRedraw();

	value.addListener(this,&ofxSlider::valueChanged);
	registerMouseEvents();

	input.setup(_val, width, height);
	return this;
}

template<typename Type>
ofxSlider<Type>* ofxSlider<Type>::setup(const std::string& sliderName, Type _val, Type _min, Type _max, float width, float height){
	value.set(sliderName,_val,_min,_max);
	return setup(value,width,height);
}

template<typename Type>
void ofxSlider<Type>::setMin(Type min){
    value.setMin(min);
}

template<typename Type>
Type ofxSlider<Type>::getMin(){
    return value.getMin();
}

template<typename Type>
void ofxSlider<Type>::setMax(Type max){
    value.setMax(max);
}

template<typename Type>
Type ofxSlider<Type>::getMax(){
    return value.getMax();
}

template<typename Type>
bool ofxSlider<Type>::mouseMoved(ofMouseEventArgs & mouse){
	bool mouseInside = isGuiDrawing() && b.inside(mouse);
	if(mouseInside != this->mouseInside && overlappingLabel){
		setNeedsRedraw();
	}
	this->mouseInside = mouseInside;

	if(usingTimeline()){
		auto overTLIcon = getTLIconBox(b.position.xy()).inside(mouse);
		if(overTLIcon != this->overTLIcon){
			setNeedsRedraw();
		}
		this->overTLIcon = overTLIcon;
	}
	return mouseInside;
}

template<typename Type>
bool ofxSlider<Type>::mousePressed(ofMouseEventArgs & mouse){
	if(state==Slider){
		if(mouse.button == OF_MOUSE_BUTTON_RIGHT){
			if(b.inside(mouse)){
				state = Input;
				auto mouseLeft = mouse;
				mouseLeft.button = OF_MOUSE_BUTTON_LEFT;
				input.mousePressed(mouseLeft);
				return true;
			}else{
				return false;
			}
		}else{
			if(bUpdateOnReleaseOnly){
				value.disableEvents();
			}

#if OFX_TIMELINE
			if(usingTimeline() && isGuiDrawing() && overTLIcon){
				return bGuiActive = pressedOnTLIcon = true;
			}
#endif
			if(setValue(mouse.x, mouse.y, true)){
				return true;
			}else{
				return false;
			}
		}
	}else{
		return isGuiDrawing() && input.mousePressed(mouse);
	}
}

template<typename Type>
bool ofxSlider<Type>::mouseDragged(ofMouseEventArgs & mouse){
	mouseMoved(mouse);
	pressedOnTLIcon = false;
	if(state==Slider){
		mouseDragging = setValue(mouse.x, mouse.y, false);
		return mouseDragging;
	}else{
		return isGuiDrawing() && input.mouseDragged(mouse);
	}
}

template<typename Type>
bool ofxSlider<Type>::mouseReleased(ofMouseEventArgs & mouse){
	if(state==Slider){
		if(bUpdateOnReleaseOnly){
			value.enableEvents();
		}

#if OFX_TIMELINE
		if(usingTimeline() && isGuiDrawing() && pressedOnTLIcon && overTLIcon){
			bGuiActive = pressedOnTLIcon = false;
			setTimelined(timeline, !timelined);
			return true;
		}
#endif

		bool attended = setValue(mouse.x, mouse.y, false);
		mouseDragging = false;
		bGuiActive = false;
		if(attended){
			return true;
		}else{
			return false;
		}
	}else{
		return isGuiDrawing() && input.mouseReleased(mouse);
	}
}

#if OFX_TIMELINE
template<typename Type>
void ofxSlider<Type>::setTimelined(ofxTimeline * timeline, bool timelined){
	this->timeline = timeline;
	if(timeline==nullptr){
		this->timelined = false;
		return;
	}
	this->timelined = timelined;
	if(timelined){
		tlCurves = timeline->addCurves(value);
	}else{
		timeline->remove(value);
		tlCurves = nullptr;
	}
	setNeedsRedraw();
	timeline->setOffset(glm::vec2(0, ofGetHeight() - timeline->getHeight()));
}

template<typename Type>
bool ofxSlider<Type>::refreshTimelined(ofxTimeline * timeline){
	this->timeline = timeline;
	if(timeline->getTrack(value)){
		this->timelined = true;
		tlCurves = timeline->linkCurves(value);
		return true;
	}else{
		this->timelined = false;
		return false;
	}
}
#endif

template<typename Type>
typename std::enable_if<std::is_integral<Type>::value, Type>::type
getRange(Type min, Type max, float width){
	double range = max - min;
	range /= width*4;
	return std::max(range,1.0);
}

template<typename Type>
typename std::enable_if<std::is_floating_point<Type>::value, Type>::type
getRange(Type min, Type max, float width){
	double range = max - min;
	range /= width*4;
	return range;
}

template<typename Type>
bool ofxSlider<Type>::mouseScrolled(ofMouseEventArgs & args){
	if(state==Slider){
		if(mouseInside){
			if(args.scrollY>0 || args.scrollY<0){
				double range = getRange(value.getMin(),value.getMax(),b.width);
				Type newValue = value + ofMap(args.scrollY,-1,1,-range, range);
				newValue = ofClamp(newValue,value.getMin(),value.getMax());
				value = newValue;
			}
			return true;
		}else{
			return false;
		}
	}else{
		return isGuiDrawing() && input.mouseScrolled(args);
	}
}


template<typename Type>
double ofxSlider<Type>::operator=(Type v){
	value = v;
	return v;
}

template<typename Type>
ofxSlider<Type>::operator const Type & (){
	return value;
}

template<typename Type>
void ofxSlider<Type>::generateDraw(){
	bg.clear();
	bar.clear();

	bg.setFillColor(timelined ? thisTimelinedBgColor : thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b);

	float valAsPct = ofClamp(value.getPctScaled(),0,1);
#if OFX_TIMELINE
	if(timelined && mouseDragging){
		auto kf = tlCurves->getNearestKeyframe(timeline->getCurrentTime());
		if(kf){
			valAsPct = kf->value;
		}
	}
#endif

	bar.setFillColor(timelined ? thisTimelinedColor : thisFillColor);
	bar.setFilled(true);
	bar.rectangle(b.x+1, b.y+1, valAsPct * (b.width - 2), b.height-2);

	generateText();
	input.generateDraw();
}

template<typename Type>
void ofxSlider<Type>::generateText(){
	string valStr = toString(value.get());

#if OFX_TIMELINE
	if(timelined && mouseDragging){
		auto kf = tlCurves->getNearestKeyframe(timeline->getCurrentTime());
		if(kf){
			ofParameter<Type> scale{"",value.get(),value.getMin(),value.getMax(),value.getScale()};
			scale.setPctScaled(kf->value);
			valStr = toString(scale.get());
		}
	}
#endif

	auto inputWidth = getTextBoundingBox(valStr,0,0).width;
	auto label = getTextBoundingBox(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	auto value = getTextBoundingBox(valStr, b.x + b.width - textPadding - inputWidth, b.y + b.height / 2 + 4);
	overlappingLabel = label.getMaxX() > value.x;

	textMesh.clear();

	auto showName = true;
	if(overTLIcon && !mouseDragging){
		showName = false;
	}
	if(overlappingLabel && mouseInside){
		showName = false;
	}
	if(showName){
		std::string name;
		if(overlappingLabel){
			for(auto c: ofUTF8Iterator(getName())){
				auto next = name;
				ofUTF8Append(next, c);
				if(getTextBoundingBox(next,0,0).width > b.width-textPadding*2){
					break;
				}else{
					name = next;
				}
			}
		}else{
			name = getName();
		}
		textMesh.append(getTextMesh(name, b.x + textPadding, b.y + b.height / 2 + 4));
	}

	auto showValue = true;
	if(overlappingLabel && !mouseInside){
		showValue = false;
	}
	if(showValue){
		textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, b.y + b.height / 2 + 4));
	}
}

template<typename Type>
void ofxSlider<Type>::render(){
	if(state==Slider){
		ofColor c = ofGetStyle().color;
		if(errorTime>0 && !input.containsValidValue()){
			auto now = ofGetElapsedTimef();
			auto pct = (now - errorTime) / 0.5;
			if(pct<1){
				bg.setFillColor(ofColor::darkRed.getLerped(thisBackgroundColor, pct));
				bar.setFillColor(ofColor::red.getLerped(thisFillColor, pct));
			}else{
				bg.setFillColor(thisBackgroundColor);
				bar.setFillColor(thisFillColor);
				errorTime = 0;
			}
		}

		bg.draw();
		bar.draw();

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
		if(ofxBaseGui::usingTimeline() && ((overTLIcon && !mouseDragging) || (overlappingLabel && mouseInside))){
			tlIcon.setStrokeColor(thisTextColor);
			tlIcon.draw(b.x, b.y);
		}
	}else{
		input.draw();
	}
}


template<typename Type>
bool ofxSlider<Type>::setValue(float mx, float my, bool bCheck){
	if( !isGuiDrawing() ){
		bGuiActive = false;
		return false;
	}
	if( bCheck ){
		if( b.inside(mx, my) ){
			bGuiActive = true;
		}else{
			bGuiActive = false;
		}
	}
	if( bGuiActive ){
		value.setPctScaled(ofMap(mx, b.x, b.x + b.width, 0, 1, true));
		return true;
	}
	return false;
}

template<typename Type>
ofAbstractParameter & ofxSlider<Type>::getParameter(){
	return value;
}


template<typename Type>
void ofxSlider<Type>::setUpdateOnReleaseOnly(bool _bUpdateOnReleaseOnly){
	bUpdateOnReleaseOnly = _bUpdateOnReleaseOnly;
}

template<typename Type>
void ofxSlider<Type>::valueChanged(Type & value){
    setNeedsRedraw();
}

template<typename Type>
void ofxSlider<Type>::setPosition(const ofPoint & p){
	ofxBaseGui::setPosition(p);
	input.setPosition(p);
}

template<typename Type>
void ofxSlider<Type>::setPosition(float x, float y){
	ofxBaseGui::setPosition(x, y);
	input.setPosition(x, y);
}

template<typename Type>
void ofxSlider<Type>::setSize(float w, float h){
	ofxBaseGui::setSize(w, h);
	input.setSize(w, h);
}

template<typename Type>
void ofxSlider<Type>::setShape(ofRectangle r){
	ofxBaseGui::setShape(r);
	input.setShape(r);
}

template<typename Type>
void ofxSlider<Type>::setShape(float x, float y, float w, float h){
	ofxBaseGui::setShape(x, y, w, h);
	input.setShape(x, y, w, h);
}

template class ofxSlider<int8_t>;
template class ofxSlider<uint8_t>;
template class ofxSlider<int16_t>;
template class ofxSlider<uint16_t>;
template class ofxSlider<int32_t>;
template class ofxSlider<uint32_t>;
template class ofxSlider<int64_t>;
template class ofxSlider<uint64_t>;
template class ofxSlider<float>;
template class ofxSlider<double>;

template class ofxSlider<typename std::conditional<std::is_same<uint32_t, size_t>::value || std::is_same<uint64_t, size_t>::value, bool, size_t>::type>;
