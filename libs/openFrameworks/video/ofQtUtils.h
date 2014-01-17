#pragma once

#include "ofConstants.h"

#if defined (TARGET_WIN32) || defined (TARGET_OSX)

#ifndef MAC_OS_X_VERSION_10_7

//p2cstr depreciation fix - thanks pickard!
#ifdef TARGET_OSX
	#define p2cstr(aStr) CFStringGetCStringPtr(CFStringCreateWithPascalString(NULL, aStr, kCFStringEncodingMacRoman),kCFStringEncodingMacRoman)
#endif

//-------------------------- helpful for rgba->rgb conversion
typedef struct{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} pix24;

typedef unsigned char Boolean;
typedef signed short SInt16;
typedef SInt16 OSErr;
typedef long ComponentResult;
struct GrafPort;
typedef struct GrafPort                 GrafPort;
typedef GrafPort *                      GrafPtr;
typedef GrafPtr                         WindowPtr;
typedef WindowPtr                       DialogPtr;
struct EventRecord;
typedef struct EventRecord              EventRecord;
struct ComponentInstanceRecord;
typedef struct ComponentInstanceRecord  ComponentInstanceRecord;
typedef ComponentInstanceRecord *       ComponentInstance;
typedef ComponentInstance               MediaHandler;
typedef ComponentInstance               MovieController;
typedef long Fixed;
struct MediaRecord;
typedef struct MediaRecord MediaRecord;
typedef MediaRecord * Media;
struct MovieRecord;
typedef struct MovieRecord MovieRecord;
typedef MovieRecord * Movie;
struct CGrafPort;
typedef struct CGrafPort                CGrafPort;
typedef CGrafPort *                     CGrafPtr;
typedef CGrafPtr                        GWorldPtr;
typedef ComponentInstance               SeqGrabComponent;
typedef ComponentInstance               SGChannel;
struct Rect;
typedef struct Rect                     Rect;


//----------------------------------------

void 		initializeQuicktime();
void 		closeQuicktime();
void 		convertPixels(unsigned char * gWorldPixels, unsigned char * rgbPixels, int w, int h);
Boolean 	SeqGrabberModalFilterUPP(DialogPtr theDialog, const EventRecord *theEvent, short *itemHit, long refCon);
OSErr           IsMPEGMediaHandler(MediaHandler inMediaHandler, Boolean *outIsMPEG);
ComponentResult MPEGMediaGetStaticFrameRate(MediaHandler inMPEGMediaHandler, Fixed *outStaticFrameRate);
OSErr           MediaGetStaticFrameRate(Media inMovieMedia, double *outFPS);
void            MovieGetVideoMediaAndMediaHandler(Movie inMovie, Media *outMedia,
				MediaHandler *outMediaHandler);
void            MovieGetStaticFrameRate(Movie inMovie, double *outStaticFrameRate);

#ifdef TARGET_OSX
	OSErr	GetSettingsPreference(CFStringRef inKey, UserData *outUserData);
	OSErr	SaveSettingsPreference(CFStringRef inKey, UserData inUserData);
#endif

#endif //OS 10.7 guard

#endif
