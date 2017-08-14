#include "ofxGuiGroup.h"
#include "ofxPanel.h"
#include "ofxSliderGroup.h"
#include "ofGraphics.h"
#include "ofxLabel.h"
#include "ofxInputField.h"

using namespace std;

ofxGuiGroup::ofxGuiGroup(){
	minimized = false;
	spacing = 1;
	spacingNextElement = 3;
	header = defaultHeight;
	bGuiActive = false;
}

ofxGuiGroup::ofxGuiGroup(const ofParameterGroup & parameters, const std::string& filename, float x, float y){
	minimized = false;
	parent = nullptr;
	setup(parameters, filename, x, y);
}

ofxGuiGroup * ofxGuiGroup::setup(const std::string& collectionName, const std::string& filename, float x, float y){
	ofParameterGroup parameters(collectionName);
	return setup(parameters, filename, x, y);
}

ofxGuiGroup * ofxGuiGroup::setup(const ofParameterGroup & _parameters, const std::string& _filename, float x, float y){
	b.x = x;
	b.y = y;
	header = defaultHeight;
	spacing = 1;
	spacingNextElement = 3;
	if(parent != nullptr){
		b.width = parent->getWidth();
	}else{
		b.width = defaultWidth;
	}
	clear();
	filename = _filename;
	bGuiActive = false;
	parameters = _parameters;

	std::vector<ofAbstractParameter*> hidden;
	for(std::size_t i = 0; i < _parameters.size(); i++){
		string type = _parameters.getType(i);
		ofxBaseGui * control = nullptr;
		if(type == typeid(ofParameter <int32_t> ).name()){
			auto p = _parameters.getInt(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <uint32_t> ).name()){
			auto p = _parameters.get<uint32_t>(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <int64_t> ).name()){
			auto p = _parameters.get<int64_t>(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <uint64_t> ).name()){
			auto p = _parameters.get<uint64_t>(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <int8_t> ).name()){
			auto p = _parameters.get<int8_t>(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <uint8_t> ).name()){
			auto p = _parameters.get<uint8_t>(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <int16_t> ).name()){
			auto p = _parameters.get<int16_t>(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <uint16_t> ).name()){
			auto p = _parameters.get<uint16_t>(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <size_t> ).name()){
			auto p = _parameters.get<size_t>(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <float> ).name()){
			auto p = _parameters.getFloat(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <double> ).name()){
			auto p = _parameters.get<double>(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <bool> ).name()){
			auto p = _parameters.getBool(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <void> ).name()){
			auto p = _parameters.getVoid(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <ofDefaultVec2> ).name()){
			auto p = _parameters.getVec2f(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <ofDefaultVec3> ).name()){
			auto p = _parameters.getVec3f(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <ofDefaultVec4> ).name()){
			auto p = _parameters.getVec4f(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <ofColor> ).name()){
			auto p = _parameters.getColor(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <ofShortColor> ).name()){
			auto p = _parameters.getShortColor(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <ofFloatColor> ).name()){
			auto p = _parameters.getFloatColor(i);
			control = defaultControl(p);
		}else if(type == typeid(ofParameter <string> ).name()){
			auto p = _parameters.getString(i);
			control = defaultControl(p);
		}else if(typeid(string).name() == _parameters[i].valueType() && _parameters[i].isReadOnly()){
			auto p = _parameters.get(i).castReadOnly<string,void>();
			control = defaultControl(p);
		}else if(type == typeid(ofParameterGroup).name()){
			auto p = _parameters.getGroup(i);
			control = defaultControl(p);
		}else{
			ofLogWarning() << "ofxBaseGroup; no control for parameter of type " << type;
		}
		if(control){
			addWithoutParameter(control);
			auto hiddenParams = control->getHiddenParameters();
			if(hiddenParams){
				hidden.push_back(hiddenParams);
			}
		}
	}

	for(auto * p: hidden){
		parameters.add(*p);
	}

	registerMouseEvents();

	setNeedsRedraw();

	return this;
}

void ofxGuiGroup::setWidthElements(float w){
	for(std::size_t i = 0; i < collection.size(); i++){
		collection[i]->setSize(w, collection[i]->getHeight());
		collection[i]->setPosition(b.x + b.width - w, collection[i]->getPosition().y);
		ofxGuiGroup * subgroup = dynamic_cast<ofxGuiGroup*>(collection[i]);
		if(subgroup != nullptr){
			subgroup->setWidthElements(w * .98);
		}
	}
	sizeChangedCB();
	setNeedsRedraw();
}

void ofxGuiGroup::add(ofxBaseGui * element){
	addWithoutParameter(element);
	parameters.add(element->getParameter());
}

void ofxGuiGroup::addWithoutParameter(ofxBaseGui * element){
	collection.push_back(element);

	element->setPosition(b.x, b.y + b.height  + spacing);

	b.height += element->getHeight() + spacing;

	//if(b.width<element->getWidth()) b.width = element->getWidth();

	element->unregisterMouseEvents();

	element->setParent(this);

	ofxGuiGroup * subgroup = dynamic_cast <ofxGuiGroup *>(element);
	if(subgroup != nullptr){
		subgroup->filename = filename;
		subgroup->setWidthElements(b.width * .98);
	}else{
		if(parent != nullptr){
			element->setSize(b.width * .98, element->getHeight());
			element->setPosition(b.x + b.width - element->getWidth(), element->getPosition().y);
		}
	}
#if OFX_TIMELINE
	element->setTimeline(timeline);
#endif

	setNeedsRedraw();
}

void ofxGuiGroup::add(const ofParameterGroup & parameters){
	add(defaultControl(parameters));
}

ofxGuiGroup * ofxGuiGroup::defaultControl(const ofParameterGroup & parameters){
	return new ofxGuiGroup(parameters);
}

ofxButton * ofxGuiGroup::defaultControl(ofParameter <void> & parameter, float width, float height){
	return new ofxButton(parameter, width, height);
}

ofxToggle * ofxGuiGroup::defaultControl(ofParameter <bool> & parameter, float width, float height){
	return new ofxToggle(parameter, width, height);
}

ofxTextField * ofxGuiGroup::defaultControl(ofParameter <std::string> & parameter, float width, float height){
	return new ofxTextField(parameter, width, height);
}

ofxVecSlider_<ofVec2f> * ofxGuiGroup::defaultControl(ofParameter <ofVec2f> & parameter, float width, float height){
	return new ofxVecSlider_<ofVec2f>(parameter, width, height);
}

ofxVecSlider_<ofVec3f> *  ofxGuiGroup::defaultControl(ofParameter <ofVec3f> & parameter, float width, float height){
	return new ofxVecSlider_<ofVec3f>(parameter, width, height);
}

ofxVecSlider_<ofVec4f> *  ofxGuiGroup::defaultControl(ofParameter <ofVec4f> & parameter, float width, float height){
	return new ofxVecSlider_<ofVec4f>(parameter, width, height);
}

ofxVecSlider_<glm::vec2> * ofxGuiGroup::defaultControl(ofParameter <glm::vec2> & parameter, float width, float height){
	return new ofxVecSlider_<glm::vec2>(parameter, width, height);
}

ofxVecSlider_<glm::vec3> * ofxGuiGroup::defaultControl(ofParameter <glm::vec3> & parameter, float width, float height){
	return new ofxVecSlider_<glm::vec3>(parameter, width, height);
}

ofxVecSlider_<glm::vec4> * ofxGuiGroup::defaultControl(ofParameter <glm::vec4> & parameter, float width, float height){
	return new ofxVecSlider_<glm::vec4>(parameter, width, height);
}

ofxColorSlider_<unsigned char> * ofxGuiGroup::defaultControl(ofParameter <ofColor> & parameter, float width, float height){
	return new ofxColorSlider_<unsigned char>(parameter, width, height);
}

ofxColorSlider_<unsigned short> * ofxGuiGroup::defaultControl(ofParameter <ofShortColor> & parameter, float width, float height){
	return new ofxColorSlider_<unsigned short>(parameter, width, height);
}

ofxColorSlider_<float> * ofxGuiGroup::defaultControl(ofParameter <ofFloatColor> & parameter, float width, float height){
	return new ofxColorSlider_<float>(parameter, width, height);
}

void ofxGuiGroup::clear(){
	collection.clear();
	parameters.clear();
	b.height = header + spacing + spacingNextElement;
	sizeChangedCB();
}

bool ofxGuiGroup::mouseMoved(ofMouseEventArgs & args){
	if(usingTimeline()){
		auto overTLIcon = getTLIconBox({b.x, b.y + 1 + spacingNextElement}).inside(args);
		if(isGuiDrawing() && overTLIcon != this->overTLIcon){
			setNeedsRedraw();
		}
		this->overTLIcon = overTLIcon;
	}

	ofMouseEventArgs a = args;
	bool attended = false;
	for(std::size_t i = 0; i < collection.size(); i++){
		attended |= collection[i]->mouseMoved(a);
	}
	if(attended){
		return true;
	}
	if(isGuiDrawing() && b.inside(ofPoint(args.x, args.y))){
		return true;
	}else{
		return false;
	}
}

bool ofxGuiGroup::mousePressed(ofMouseEventArgs & args){
#if OFX_TIMELINE
	if(usingTimeline() && isGuiDrawing() && overTLIcon){
		setTimelined(timeline, !timelined);
		setNeedsRedraw();
		return true;
	}
#endif

	if(setValue(args.x, args.y, true)){
		return true;
	}
	auto attended = false;
	ofMouseEventArgs a = args;
	for(std::size_t i = 0; i < collection.size(); i++){
		if(collection[i]->mousePressed(a)){
			attended = true;
		}
	}
	return attended;
}

bool ofxGuiGroup::mouseDragged(ofMouseEventArgs & args){
	if(setValue(args.x, args.y, false)){
		return true;
	}
	if(bGuiActive){
		ofMouseEventArgs a = args;
		for(std::size_t i = 0; i < collection.size(); i++){
			if(collection[i]->mouseDragged(a)){
				return true;
			}
		}
	}
	return false;
}

bool ofxGuiGroup::mouseReleased(ofMouseEventArgs & args){
	bGuiActive = false;
	for(std::size_t k = 0; k < collection.size(); k++){
		ofMouseEventArgs a = args;
		if(collection[k]->mouseReleased(a)){
			return true;
		}
	}
	if(isGuiDrawing() && b.inside(ofPoint(args.x, args.y))){
		return true;
	}else{
		return false;
	}
}

bool ofxGuiGroup::mouseScrolled(ofMouseEventArgs & args){
	ofMouseEventArgs a = args;
	for(std::size_t i = 0; i < collection.size(); i++){
		if(collection[i]->mouseScrolled(a)){
			return true;
		}
	}
	if(isGuiDrawing() && b.inside(ofPoint(args.x, args.y))){
		return true;
	}else{
		return false;
	}
}

void ofxGuiGroup::generateDraw(){
	border.clear();
	border.setFillColor(ofColor(thisBorderColor, 180));
	border.setFilled(true);
	border.rectangle(b.x, b.y + spacingNextElement, b.width + 1, b.height);


	headerBg.clear();
	headerBg.setFillColor(thisHeaderBackgroundColor);
	headerBg.setFilled(true);
	headerBg.rectangle(b.x, b.y + 1 + spacingNextElement, b.width, header);

	textMesh.clear();
	if(!overTLIcon){
		textMesh.append(getTextMesh(getName(), textPadding + b.x, header / 2 + 4 + b.y + spacingNextElement));
	}
	if(minimized){
		textMesh.append(getTextMesh("+", b.width - textPadding - 8 + b.x, header / 2 + 4 + b.y + spacingNextElement));
	}else{
		textMesh.append(getTextMesh("-", b.width - textPadding - 8 + b.x, header / 2 + 4 + b.y + spacingNextElement));
	}
}

void ofxGuiGroup::render(){
	border.draw();
	headerBg.draw();

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofColor c = ofGetStyle().color;
	ofSetColor(thisTextColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	if(overTLIcon){
		tlIcon.setColor(thisTextColor);
		tlIcon.draw(b.x, b.y + 1 + spacingNextElement);
	}

	if(!minimized){
		for(std::size_t i = 0; i < collection.size(); i++){
			collection[i]->draw();
		}
	}

	ofSetColor(c);
	if(blendMode != OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}

vector <string> ofxGuiGroup::getControlNames() const{
	vector <string> names;
	for(std::size_t i = 0; i < collection.size(); i++){
		names.push_back(collection[i]->getName());
	}
	return names;
}

ofxIntSlider & ofxGuiGroup::getIntSlider(const std::string& name){
	return getControlType <ofxIntSlider>(name);
}

ofxFloatSlider & ofxGuiGroup::getFloatSlider(const std::string& name){
	return getControlType <ofxFloatSlider>(name);
}

ofxToggle & ofxGuiGroup::getToggle(const std::string& name){
	return getControlType <ofxToggle>(name);
}

ofxButton & ofxGuiGroup::getButton(const std::string& name){
	return getControlType <ofxButton>(name);
}

ofxGuiGroup & ofxGuiGroup::getGroup(const std::string& name){
	return getControlType <ofxGuiGroup>(name);
}

ofxBaseGui * ofxGuiGroup::getControl(const std::string& name){
    for(std::size_t i = 0; i < collection.size(); i++){
		if(collection[i]->getName() == name){
			return collection[i];
		}
	}
	return nullptr;
}

bool ofxGuiGroup::setValue(float mx, float my, bool bCheck){

	if(!isGuiDrawing()){
		bGuiActive = false;
		return false;
	}


	if(bCheck){
		if(b.inside(mx, my)){
			bGuiActive = true;

			ofRectangle minButton(b.x + b.width - textPadding * 3, b.y, textPadding * 3, header);
			if(minButton.inside(mx, my)){
				minimized = !minimized;
				if(minimized){
					minimize();
				}else{
					maximize();
				}
				return true;
			}
		}
	}

	return false;
}

void ofxGuiGroup::minimize(){
	minimized = true;
	b.height = header + spacing + spacingNextElement + 1 /*border*/;
	if(parent){
		parent->sizeChangedCB();
	}
	setNeedsRedraw();
	onMinimize();
}

void ofxGuiGroup::maximize(){
	minimized = false;
    for(std::size_t i = 0; i < collection.size(); i++){
		b.height += collection[i]->getHeight() + spacing;
	}
	if(parent){
		parent->sizeChangedCB();
	}
	setNeedsRedraw();
	onMaximize();
}

void ofxGuiGroup::minimizeAll(){
	for(std::size_t i = 0; i < collection.size(); i++){
		ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(collection[i]);
		if(group){
			group->minimize();
		}
	}
}

void ofxGuiGroup::maximizeAll(){
	for(std::size_t i = 0; i < collection.size(); i++){
		ofxGuiGroup * group = dynamic_cast <ofxGuiGroup *>(collection[i]);
		if(group){
			group->maximize();
		}
	}
}

bool ofxGuiGroup::isMinimized() const{
	return minimized;
}

void ofxGuiGroup::onMinimize(){

}

void ofxGuiGroup::onMaximize(){

}

void ofxGuiGroup::sizeChangedCB(){
	float y;
	if(parent){
		y = b.y  + header + spacing + spacingNextElement;
	}else{
		y = b.y  + header + spacing;
	}
	for(std::size_t i = 0; i < collection.size(); i++){
		collection[i]->setPosition(collection[i]->getPosition().x, y + spacing);
		y += collection[i]->getHeight() + spacing;
	}
	if(minimized){
		b.height = header + spacing + spacingNextElement + 1 /*border*/;
	}else{
		b.height = y - b.y;
	}
	if(parent){
		parent->sizeChangedCB();
	}
	setNeedsRedraw();
}


std::size_t ofxGuiGroup::getNumControls() const {
	return collection.size();
}

ofxBaseGui * ofxGuiGroup::getControl(std::size_t num){
	if(num < collection.size()){
		return collection[num];
	}else{
		return nullptr;
	}
}

ofAbstractParameter & ofxGuiGroup::getParameter(){
	return parameters;
}

void ofxGuiGroup::setPosition(const ofPoint& p){
	ofVec2f diff = p - b.getPosition();

	for(std::size_t i = 0; i < collection.size(); i++){
		collection[i]->setPosition(collection[i]->getPosition() + diff);
	}

	b.setPosition(p);
	setNeedsRedraw();
}

void ofxGuiGroup::setPosition(float x, float y){
	setPosition(ofVec2f(x, y));
}

#if OFX_TIMELINE
void ofxGuiGroup::setTimeline(ofxTimeline * timeline){
	ofxBaseGui::setTimeline(timeline);
	for(auto * control: collection){
		control->setTimeline(timeline);
	}
}

void ofxGuiGroup::setTimelined(ofxTimeline * timeline, bool timelined){
	if(timeline==nullptr){
		this->timeline = nullptr;
		this->timelined = false;
	}
	this->timelined = timelined;
	if(timelined){
		auto currentPage = timeline->getPage(timeline->getCurrentPageName());
		if(currentPage->getTracks().empty()){
			timeline->setPageName(parameters.getHierarchicName());
		}else{
			auto * page = timeline->getPage(parameters.getHierarchicName());
			if(!page){
				timeline->addPage(parameters.getHierarchicName());
			}
		}
		timeline->setCurrentPage(timeline->getCurrentPageName());
	}
	for(auto & c: collection){
		c->setTimelined(timeline, timelined);
	}
}

bool ofxGuiGroup::refreshTimelined(ofxTimeline * timeline){
	this->timeline = timeline;
	if(timeline->getPage(parameters.getHierarchicName())){
		this->timelined = true;
	}
	auto anyTimelined = false;
	for(auto * control: collection){
		anyTimelined |= control->refreshTimelined(timeline);
	}
	return this->timelined || anyTimelined;
}
#endif
