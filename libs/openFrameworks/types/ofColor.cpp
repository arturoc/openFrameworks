#include "ofColor.h"
#include "ofConstants.h"
#include <limits>

template<typename PixelType>
const ofColor_<PixelType> ofColor_<PixelType>::gray = ofColor( 128,128,128 );

template<typename PixelType>
const ofColor_<PixelType> ofColor_<PixelType>::white = ofColor( 255,255,255 );

template<typename PixelType>
const ofColor_<PixelType> ofColor_<PixelType>::red = ofColor( 255,0,0 );

template<typename PixelType>
const ofColor_<PixelType> ofColor_<PixelType>::green = ofColor( 0,255,0 );

template<typename PixelType>
const ofColor_<PixelType> ofColor_<PixelType>::blue = ofColor( 0,0,255 );

template<typename PixelType>
const ofColor_<PixelType> ofColor_<PixelType>::cyan = ofColor( 0,255,255 );

template<typename PixelType>
const ofColor_<PixelType> ofColor_<PixelType>::magenta = ofColor( 255,0,255 );

template<typename PixelType>
const ofColor_<PixelType> ofColor_<PixelType>::yellow = ofColor( 255,255,0 );

template<typename PixelType>
const ofColor_<PixelType> ofColor_<PixelType>::black = ofColor( 0,0,0 );

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (){
	r = 255.0f;
	g = 255.0f;
	b = 255.0f;
	a = 255.0f;
}

template<typename PixelType>
ofColor_<PixelType>::~ofColor_ (){}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (float _r, float _g, float _b, float _a){
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (const ofColor_<PixelType> & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (const ofColor_<PixelType> & color, float _a){
	r = color.r;
	g = color.g;
	b = color.b;
	a = _a;
}

template<typename PixelType>
ofColor_<PixelType>::ofColor_ (float gray, float _a){
	r = g = b = gray;
	a = _a;
}

template<typename PixelType>
PixelType ofColor_<PixelType>::getMaxValue(){
	return numeric_limits<PixelType>::max();
}

template<>
float ofColor_<float>::getMaxValue(){
	return 1;
}

template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::fromHsb (float hue, float saturation, float brightness, float alpha) {
	ofColor_<PixelType> cur;
	cur.setHsb(hue, saturation, brightness, alpha);
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::fromHex(int hexColor, float alpha) {
	ofColor_<PixelType> cur;
	cur.setHex(hexColor, alpha);
	return cur;
}


template<typename PixelType>
void ofColor_<PixelType>::set (float _r, float _g, float _b, float _a){
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}


template<typename PixelType>
void ofColor_<PixelType>::set (float _gray, float _a){
	r = g = b = _gray;
	a = _a;
}


template<typename PixelType>
void ofColor_<PixelType>::set (ofColor_<PixelType> const & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
}


template<typename PixelType>
void ofColor_<PixelType>::setHex (int hexColor, float alpha){
	ofColor c;
	c.r = (hexColor >> 16) & 0xff;
	c.g = (hexColor >> 8) & 0xff;
	c.b = (hexColor >> 0) & 0xff;
	c.a = alpha;
	(*this) = c;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::clamp (){
	r = CLAMP(r, 0.0f, getMaxValue());
	g = CLAMP(g, 0.0f, getMaxValue());
	b = CLAMP(b, 0.0f, getMaxValue());
	a = CLAMP(a, 0.0f, getMaxValue());
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::invert() {
	r = getMaxValue() - r;
	g = getMaxValue() - g;
	b = getMaxValue() - b;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::normalize() {
	float brightness = getBrightness();
	r = getMaxValue() * (r / brightness);
	g = getMaxValue() * (g / brightness);
	b = getMaxValue() * (b / brightness);
	return *this;
}


template<typename PixelType>
ofColor_<PixelType>& ofColor_<PixelType>::lerp(const ofColor_<PixelType>& target, float amount) {
	float invAmount = 1.f - amount;
	r = invAmount * r + amount * target.r;
	g = invAmount * g + amount * target.g;
	b = invAmount * b + amount * target.b;
	a = invAmount * a + amount * target.a;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getClamped () const {
	ofColor_<PixelType> cur(*this);
	cur.clamp();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getInverted () const {
	ofColor_<PixelType> cur(*this);
	cur.invert();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getNormalized () const {
	ofColor_<PixelType> cur(*this);
	cur.normalize();
	return cur;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::getLerped(const ofColor_<PixelType>& target, float amount) const {
	ofColor_<PixelType> cur(*this);
	cur.lerp(target, amount);
	return cur;
}



template<typename PixelType>
float ofColor_<PixelType>::getHue() const {
	float hue, saturation, brightness;
	getHsb(hue, saturation, brightness);
	return hue;
}


template<typename PixelType>
float ofColor_<PixelType>::getSaturation() const {
	float hue, saturation, brightness;
	getHsb(hue, saturation, brightness);
	return saturation;
}

/*
	Brightness is simply the maximum of the three color components.
	This is used by Photoshop (HSB) and Processing (HSB).
	Brightness is also called "value".
*/

template<typename PixelType>
float ofColor_<PixelType>::getBrightness() const {
	float max = r;
	if(g > max) {
		max = g;
	}
	if(b > max) {
		max = b;
	}
	return max;
}

/*
	Lightness is the average of the three color components.
	This is used by the Lab and HSL color spaces.
*/

template<typename PixelType>
float ofColor_<PixelType>::getLightness() const {
	return (r + g + b) / 3.f;
}



template<typename PixelType>
void ofColor_<PixelType>::getHsb(float& hue, float& saturation, float& brightness) const {
	float max = getBrightness();
	
	float min = r;
	if(g < min) {
		min = g;
	}
	if(b < min) {
		min = b;
	}
	
	if(max == min) { // grays
		hue = 0.f;
		saturation = 0.f;
		brightness = getMaxValue() * max;
		return;
	}
	
	float hueSixth;
	if(r == max) {
		hueSixth = (g - b) / (max - min);
		if(hueSixth < 0.f)
			hueSixth += 6.f;
	} else if (g == max) {
		hueSixth = 2.f + (b - r) / (max - min);
	} else {
		hueSixth = 4.f + (r - g) / (max - min);
	}
	hue = getMaxValue() * hueSixth / 6.f;
	saturation = getMaxValue() * (max - min) / max;
	brightness = max;
}


template<typename PixelType>
void ofColor_<PixelType>::setHue (float hue) {
	float oldHue, saturation, brightness;
	getHsb(oldHue, saturation, brightness);
	setHsb(hue, saturation, brightness);
}


template<typename PixelType>
void ofColor_<PixelType>::setSaturation (float saturation) {
	float hue, oldSaturation, brightness;
	getHsb(hue, oldSaturation, brightness);
	setHsb(hue, saturation, brightness);
}


template<typename PixelType>
void ofColor_<PixelType>::setBrightness (float brightness) {
	float hue, saturation, oldBrightness;
	getHsb(hue, saturation, oldBrightness);
	setHsb(hue, saturation, brightness);
}


template<typename PixelType>
void ofColor_<PixelType>::setHsb(float hue, float saturation, float brightness, float alpha) {
	a = alpha;
	setHsb(hue, saturation, brightness);
}

/*
	setHsb() breaks the "always override alpha" model because it's needed by setHue(),
	setSaturation(), and setBrightness() -- which shouldn't modify the alpha.
*/

template<typename PixelType>
void ofColor_<PixelType>::setHsb(float hue, float saturation, float brightness) {
	if(brightness == 0) { // black
		set(0);
	} else if(saturation == 0) { // grays
		set(brightness);
	} else {
		float hueSix = hue * 6. / float(getMaxValue());
		float saturationNorm = saturation / float(getMaxValue());
		int hueSixCategory = (int) floorf(hueSix);
		float hueSixRemainder = hueSix - hueSixCategory;
		unsigned char pv = (unsigned char) ((1.f - saturationNorm) * brightness);
		unsigned char qv = (unsigned char) ((1.f - saturationNorm * hueSixRemainder) * brightness);
		unsigned char tv = (unsigned char) ((1.f - saturationNorm * (1.f - hueSixRemainder)) * brightness);
		switch(hueSixCategory) {
			case 0: // r
				r = brightness;
				g = tv;
				b = pv;
				break;
			case 1: // g
				r = qv;
				g = brightness;
				b = pv;
				break;
			case 2:
				r = pv;
				g = brightness;
				b = tv;
				break;
			case 3: // b
				r = pv;
				g = qv;
				b = brightness;
				break;
			case 4:
				r = tv;
				g = pv;
				b = brightness;
				break;
			case 5: // back to r
				r = brightness;
				g = pv;
				b = qv;
				break;
		}
	}
}


template<typename PixelType>
int ofColor_<PixelType>::getHex () const {
	ofColor c = *this;
	return
		((0xff & (unsigned char) c.r) << 16) |
		((0xff & (unsigned char) c.g) << 8) |
		((0xff & (unsigned char) c.b));
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator = (const ofColor_<PixelType> & color){
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator = (float const & val){
	r = val;
	g = val;
	b = val;
	a = getMaxValue();
	return *this;
}


template<typename PixelType>
bool ofColor_<PixelType>::operator == (const ofColor_<PixelType> & color){
	return (r == color.r) && (g == color.g) && (b == color.b);
}


template<typename PixelType>
bool ofColor_<PixelType>::operator != (const ofColor_<PixelType> & color){
	return (r != color.r) || (g != color.g) || (b != color.b);
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator + (const ofColor_<PixelType> & color) const{
	return ofColor_<PixelType>( CLAMP(float(r)+float(color.r),0,getMaxValue()),
								CLAMP(float(g)+float(color.g),0,getMaxValue()),
								CLAMP(float(b)+float(color.b),0,getMaxValue()));
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator + (float const & val) const{
	return ofColor_<PixelType>( CLAMP(float(r)+val,0,getMaxValue()),
								CLAMP(float(g)+val,0,getMaxValue()),
								CLAMP(float(b)+val,0,getMaxValue()));
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator += (const ofColor_<PixelType> & color){
	*this = *this + color;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator += (float const & val){
	*this = *this + val;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator - (const ofColor_<PixelType> & color) const{
	return ofColor_<PixelType>( CLAMP(float(r)-float(color.r),0,getMaxValue()),
								CLAMP(float(g)-float(color.g),0,getMaxValue()),
								CLAMP(float(b)-float(color.b),0,getMaxValue()));
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator - (float const & val) const{
	return ofColor_<PixelType>( CLAMP(float(r)-val,0,getMaxValue()),
								CLAMP(float(g)-val,0,getMaxValue()),
								CLAMP(float(b)-val,0,getMaxValue()));
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator -= (const ofColor_<PixelType> & color){
	*this = *this - color;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator -= (float const & val){
	*this = *this - val;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator * (const ofColor_<PixelType> & color) const{
	return ofColor_<PixelType>( r*float(color.r/float(getMaxValue())),
								g*float(color.g/float(getMaxValue())),
								b*float(color.b/float(getMaxValue())));
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator * (float const & val) const{
	float v = CLAMP(val,0.0,1.0);
	return ofColor_<PixelType>( r*v/float(getMaxValue()),
								g*v/float(getMaxValue()),
								b*v/float(getMaxValue()) );
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator *= (ofColor_<PixelType> const & color){
	*this = *this * color;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator *= (float const & val){
	*this = *this * val;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator / (ofColor_<PixelType> const & color) const{
	return ofColor_<PixelType>( color.r!=0.0f ? r/float(color.r/float(getMaxValue())) : r ,
								color.g!=0.0f ? g/float(color.g/float(getMaxValue())) : g,
								color.b!=0.0f ? b/float(color.b/float(getMaxValue())) : b );
}


template<typename PixelType>
ofColor_<PixelType> ofColor_<PixelType>::operator / (float const & val) const{
	if( val != 0.0f)
		return *this * 1./val;
	else
		return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator /= (ofColor_<PixelType> const & color){
	*this = *this / color;
	return *this;
}


template<typename PixelType>
ofColor_<PixelType> & ofColor_<PixelType>::operator /= (float const & val){
	*this = *this / val;
	return *this;
}


template<typename PixelType>
const PixelType & ofColor_<PixelType>::operator [] (int n) const{
	switch( n ){
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3: 
			return a;
		default:
			return r;
			break;
	}
}

template<typename PixelType>
PixelType & ofColor_<PixelType>::operator [] (int n){
	switch( n ){
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		case 3:
			return a;
		default:
			return r;
			break;
	}
}

template class ofColor_<unsigned char>;
template class ofColor_<float>;
template class ofColor_<unsigned short>;
