#pragma once
#include "ofBaseTypes.h"

class ofSoundObject{
public:
	ofSoundObject()
		:connectedTo(NULL)
		,input(NULL)
		,checkingLoop(false){}

	virtual ~ofSoundObject(){}

	bool connectTo(ofSoundObject & soundObject){
		disconnect();
		checkingLoop = true;
		soundObject.checkLoop();
		return soundObject.addInput(*this);
	}

	bool disconnect(){
		if(connectedTo){
			return connectedTo->removeInput(*this);
		}else{
			return true;
		}
	}

protected:
	ofSoundObject * getInput(){
		return input;
	}

private:
	bool addInput(ofSoundObject & soundObject){
		if(input == NULL){
			input = &soundObject;
			return true;
		}else{
			return false;
		}
	}

	bool removeInput(ofSoundObject & soundObject){
		if(input == &soundObject){
			input = NULL;
			return true;
		}else{
			return false;
		}
	}

	bool checkLoop(){
		if(checkingLoop) return true;
		checkingLoop = true;
		bool loop = false;
		if(connectedTo){
			loop = connectedTo->checkLoop();
		}else{
			loop = false;
		}
		checkingLoop = false;
		return loop;
	}
	ofSoundObject * connectedTo;
	ofSoundObject * input;
	bool checkingLoop;
};
