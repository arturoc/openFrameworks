#pragma once
#include "ofxBaseGui.h"

#include "ofxSlider.h"
#include "ofxButton.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"

template<class VecType>
class ofxVecSlider_;
template<class ColorType>
class ofxColorSlider_;


class ofxGuiGroup : public ofxBaseGui {
	public:
		ofxGuiGroup();
		ofxGuiGroup(const ofParameterGroup & parameters, const std::string& _filename = "settings.xml", float x = 10, float y = 10);
		virtual ~ofxGuiGroup(){
		}
		ofxGuiGroup * setup(const std::string& collectionName = "", const std::string& filename = "settings.xml", float x = 10, float y = 10);
		ofxGuiGroup * setup(const ofParameterGroup & parameters, const std::string& filename = "settings.xml", float x = 10, float y = 10);

		void add(ofxBaseGui * element);
		void add(const ofParameterGroup & parameters);


		template<typename T>
		void add(ofParameter <T> & parameter){
			add(defaultControl(parameter), b.width);
		}


		static ofxGuiGroup * defaultControl(const ofParameterGroup & parameters);
		template<typename T>
		typename std::enable_if<std::is_arithmetic<T>::value, ofxSlider<T>*>::type defaultControl(ofParameter<T> & p){
			return new ofxSlider<T>(p);
		}
		static ofxButton * defaultControl(ofParameter <void> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxToggle * defaultControl(ofParameter <bool> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxTextField * defaultControl(ofParameter <std::string> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxVecSlider_<ofVec2f> * defaultControl(ofParameter <ofVec2f> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxVecSlider_<ofVec3f> *  defaultControl(ofParameter <ofVec3f> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxVecSlider_<ofVec4f> *  defaultControl(ofParameter <ofVec4f> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxVecSlider_<glm::vec2> * defaultControl(ofParameter <glm::vec2> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxVecSlider_<glm::vec3> * defaultControl(ofParameter <glm::vec3> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxVecSlider_<glm::vec4> * defaultControl(ofParameter <glm::vec4> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxColorSlider_<unsigned char> * defaultControl(ofParameter <ofColor> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxColorSlider_<unsigned short> * defaultControl(ofParameter <ofShortColor> & parameter, float width = defaultWidth, float height = defaultHeight);
		static ofxColorSlider_<float> * defaultControl(ofParameter <ofFloatColor> & parameter, float width = defaultWidth, float height = defaultHeight);

		void minimize();
		void maximize();
		void minimizeAll();
		void maximizeAll();
		bool isMinimized() const;

		void setWidthElements(float w);

		void clear();

		virtual void sizeChangedCB();

		virtual bool mouseMoved(ofMouseEventArgs & args);
		virtual bool mousePressed(ofMouseEventArgs & args);
		virtual bool mouseDragged(ofMouseEventArgs & args);
		virtual bool mouseReleased(ofMouseEventArgs & args);
		virtual bool mouseScrolled(ofMouseEventArgs & args);


		std::vector <std::string> getControlNames() const;
		std::size_t getNumControls() const;

		ofxIntSlider & getIntSlider(const std::string& name);
		ofxFloatSlider & getFloatSlider(const std::string& name);
		ofxToggle & getToggle(const std::string& name);
		ofxButton & getButton(const std::string& name);
		ofxGuiGroup & getGroup(const std::string& name);

		ofxBaseGui * getControl(const std::string& name);
		ofxBaseGui * getControl(std::size_t num);

		virtual ofAbstractParameter & getParameter();

		virtual void setPosition(const ofPoint& p);
		virtual void setPosition(float x, float y);

#if OFX_TIMELINE
		virtual void setTimeline(ofxTimeline * timeline);
		virtual void setTimelined(ofxTimeline * timeline, bool timelined);
		bool refreshTimelined(ofxTimeline * timeline);
#endif

	protected:

		void addWithoutParameter(ofxBaseGui * element);
		virtual void render();
		virtual bool setValue(float mx, float my, bool bCheck);
		virtual void onMinimize();
		virtual void onMaximize();

		float spacing, spacingNextElement;
		float header;

		template <class ControlType>
		ControlType & getControlType(const std::string& name);

		virtual void generateDraw();

		std::vector <ofxBaseGui *> collection;
		ofParameterGroup parameters;

		std::string filename;
		bool minimized = false;
		bool bGuiActive = false;
		bool overTLIcon = false;
		bool timelined = false;

		ofPath border, headerBg;
		ofVboMesh textMesh;
};

template <class ControlType>
ControlType & ofxGuiGroup::getControlType(const std::string& name){
	ControlType * control = dynamic_cast <ControlType *>(getControl(name));
	if(control){
		return *control;
	}else{
		ofLogWarning() << "getControlType " << name << " not found, creating new";
		control = new ControlType;
		control->setName(name);
		add(control);
		return *control;
	}
}
