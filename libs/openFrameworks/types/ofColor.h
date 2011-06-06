#pragma once
#include "ofMath.h"

//----------------------------------------------------------
// ofColor
//----------------------------------------------------------

template<typename PixelType>
class ofColor_{
	public:
	
		ofColor_<PixelType> ();
		~ofColor_<PixelType> ();
		
		ofColor_<PixelType> (float _r, float _g, float _b, float _a = getMaxValue());
		ofColor_<PixelType> (const ofColor_<PixelType> & color);
		ofColor_<PixelType> (const ofColor_<PixelType> & color, float _a);
		ofColor_<PixelType> (float gray, float _a = getMaxValue());
		
		template<typename SrcType>
		ofColor_<PixelType> (const ofColor_<SrcType> & color);

		static ofColor_<PixelType> fromHsb (float hue, float saturation, float brightness, float alpha = getMaxValue());
		static ofColor_<PixelType> fromHex (int hexColor, float alpha = getMaxValue());
		static PixelType getMaxValue();
		
		static const ofColor_<PixelType> gray;
		static const ofColor_<PixelType> white;
		static const ofColor_<PixelType> red;
		static const ofColor_<PixelType> green;
		static const ofColor_<PixelType> blue;
		static const ofColor_<PixelType> cyan;
		static const ofColor_<PixelType> magenta;
		static const ofColor_<PixelType> yellow;
		static const ofColor_<PixelType> black;
	
		void set (float _r, float _g, float _b, float _a = getMaxValue());
		void set (float _gray, float _a = getMaxValue());
		void set (ofColor_<PixelType> const & color);

		void setHex (int hexColor, float alpha = getMaxValue());
		int getHex () const;
		
		ofColor_<PixelType>& clamp ();
		ofColor_<PixelType>& invert ();
		ofColor_<PixelType>& normalize ();
		ofColor_<PixelType>& lerp(const ofColor_<PixelType>& target, float amount);
		
		ofColor_<PixelType> getClamped () const;
		ofColor_<PixelType> getInverted () const;
		ofColor_<PixelType> getNormalized () const;
		ofColor_<PixelType> getLerped(const ofColor_<PixelType>& target, float amount) const;
		
		float getHue () const;
		float getSaturation () const;
		float getBrightness () const; // brightest component
		float getLightness () const; // average of the components
		void getHsb(float& hue, float& saturation, float& brightness) const;
		
		void setHue (float hue);
		void setSaturation (float saturation);
		void setBrightness (float brightness);
		void setHsb(float hue, float saturation, float brightness, float alpha);
		void setHsb(float hue, float saturation, float brightness);
		
		ofColor_<PixelType> & operator = (ofColor_<PixelType> const & color);

		template<typename SrcType>
		ofColor_<PixelType> & operator = (ofColor_<SrcType> const & color);

		ofColor_<PixelType> & operator = (float const & val);
		bool operator == (ofColor_<PixelType> const & color);
		bool operator != (ofColor_<PixelType> const & color);
		ofColor_<PixelType> operator + (ofColor_<PixelType> const & color) const;
		ofColor_<PixelType> operator + (float const & val) const;
		ofColor_<PixelType> & operator += (ofColor_<PixelType> const & color);
		ofColor_<PixelType> & operator += (float const & val);
		ofColor_<PixelType> operator - (ofColor_<PixelType> const & color) const;
		ofColor_<PixelType> operator - (float const & val) const;
		ofColor_<PixelType> & operator -= (ofColor_<PixelType> const & color);
		ofColor_<PixelType> & operator -= (float const & val);
		ofColor_<PixelType> operator * (ofColor_<PixelType> const & color) const;
		ofColor_<PixelType> operator * (float const & val) const;
		ofColor_<PixelType> & operator *= (ofColor_<PixelType> const & color);
		ofColor_<PixelType> & operator *= (float const & val);
		ofColor_<PixelType> operator / (ofColor_<PixelType> const & color) const;
		ofColor_<PixelType> operator / (float const & val) const;
		ofColor_<PixelType> & operator /= (ofColor_<PixelType> const & color);
		ofColor_<PixelType> & operator /= (float const & val);
		const PixelType & operator [] (int n) const;
		PixelType & operator [] (int n);
		
		union  {
			struct {
				PixelType r, g, b, a;
			};
			PixelType v[4];
		};


	private:
		template<typename SrcType>
		void copyFrom(const ofColor_<SrcType> & mom);
		void copyFrom(const ofColor_<float> & mom);
};


typedef ofColor_<unsigned char> ofColor;
typedef ofColor_<float> ofFloatColor;
typedef ofColor_<unsigned short> ofShortColor;

template<typename PixelType>
template<typename SrcType>
ofColor_<PixelType>::ofColor_(const ofColor_<SrcType> & mom){
	copyFrom(mom);
}

template<typename PixelType>
template<typename SrcType>
ofColor_<PixelType> & ofColor_<PixelType>::operator=(const ofColor_<SrcType> & mom){
	copyFrom(mom);
	return *this;
}

template<typename PixelType>
template<typename SrcType>
void ofColor_<PixelType>::copyFrom(const ofColor_<SrcType> & mom){
	const float srcMax = ofColor_<SrcType>::getMaxValue();
	const float dstMax = ofColor_<PixelType>::getMaxValue();
	const float factor = dstMax / srcMax;

	// everything else is a straight scaling
	for(int i = 0; i < 4; i++){
		v[i] = mom[i] * factor;
	}
}

template<typename PixelType>
void ofColor_<PixelType>::copyFrom(const ofColor_<float> & mom){
	const float factor =  ofColor_<PixelType>::getMaxValue();

	// converting from float we need clamping to 0..1
	for(int i = 0; i < 4; i++){
		v[i] = ofClamp(mom[i], 0, 1) * factor;
	}

}
