#include "ofSoundStream.h"
#include "portaudio.h"
#include "ofUtils.h"

//----------------------------------- static variables:
static ofBaseApp 	* 		OFSAptr;
PaStream 			*		stream;
int 						nInputChannels;
int 						nOutputChannels;
ofAudioEventArgs 			audioEventArgs;


int receiveAudioBufferAndCallSimpleApp(const void *inputBuffer,
        void *outputBuffer,
        unsigned long bufferSize,
        const PaStreamCallbackTimeInfo* streamTime,
        PaStreamCallbackFlags status,
        void *data );
//(void *outputBuffer, void *inputBuffer, unsigned int bufferSize,
//           double streamTime, RtAudioStreamStatus status, void *data);


//------------------------------------------------------------------------------
int receiveAudioBufferAndCallSimpleApp(const void *inputBuffer,
        void *outputBuffer,
        unsigned long bufferSize,
        const PaStreamCallbackTimeInfo* streamTime,
        PaStreamCallbackFlags status,
        void *data ){


	if ( status ) std::cout << "Stream over/underflow detected." << std::endl;

	// 	rtAudio uses a system by which the audio
	// 	can be of different formats
	// 	char, float, etc.
	// 	we choose float
	float * fPtrOut = (float *)outputBuffer;
	float * fPtrIn = (float *)inputBuffer;
	// [zach] memset output to zero before output call
	// this is because of how rtAudio works: duplex w/ one callback
	// you need to cut in the middle. if the simpleApp
	// doesn't produce audio, we pass silence instead of duplex...


	if (nInputChannels > 0){
		if(OFSAptr)OFSAptr->audioReceived(fPtrIn, bufferSize, nInputChannels);
		memset(fPtrIn, 0, bufferSize * nInputChannels * sizeof(float));
		#ifdef OF_USING_POCO
			audioEventArgs.buffer = fPtrIn;
			audioEventArgs.bufferSize = bufferSize;
			audioEventArgs.nChannels = nInputChannels;
			ofNotifyEvent( ofEvents.audioReceived, audioEventArgs );
		#endif
	}


	if (nOutputChannels > 0) {
		if(OFSAptr)OFSAptr->audioRequested(fPtrOut, bufferSize, nOutputChannels);
		#ifdef OF_USING_POCO
			audioEventArgs.buffer = fPtrOut;
			audioEventArgs.bufferSize = bufferSize;
			audioEventArgs.nChannels = nOutputChannels;
			ofNotifyEvent( ofEvents.audioRequested, audioEventArgs );
		#endif
	}

	return 0;
}
//---------------------------------------------------------
void ofSoundStreamSetup(int nOutputs, int nInputs, ofBaseApp * OFSA){
	ofSoundStreamSetup(nOutputs, nInputs, OFSA, 44100, 256, 4);
}

//---------------------------------------------------------
void ofSoundStreamSetup(int nOutputs, int nInputs, int sampleRate, int bufferSize, int nBuffers){
	ofSoundStreamSetup(nOutputs, nInputs, NULL, sampleRate, bufferSize, nBuffers);
}

//---------------------------------------------------------
void ofSoundStreamSetup(int nOutputs, int nInputs, ofBaseApp * OFSA, int sampleRate, int bufferSize, int nBuffers){

	nInputChannels 		=  nInputs;
	nOutputChannels 	=  nOutputs;
	OFSAptr 			=  OFSA;
	bufferSize = ofNextPow2(bufferSize);	// must be pow2


	PaError err;
	err = Pa_Initialize();
	if( err != paNoError )
		ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));


	err = Pa_OpenDefaultStream( &stream,
									nInputChannels,          /* no input channels */
									nOutputChannels,          /* stereo output */
									paFloat32,  /* 64 bit floating point output */
									sampleRate,
									bufferSize,        /* frames per buffer, i.e. the number
													   of sample frames that PortAudio will
													   request from the callback. Many apps
													   may want to use
													   paFramesPerBufferUnspecified, which
													   tells PortAudio to pick the best,
													   possibly changing, buffer size.*/
									&receiveAudioBufferAndCallSimpleApp, /* this is your callback function */
									NULL ); /*This is a pointer that will be passed to
													   your callback*/

	err = Pa_StartStream( stream );
    if( err != paNoError )
    	ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));


}

//---------------------------------------------------------
void ofSoundStreamStop(){
	int err = Pa_StopStream(stream);
    if( err != paNoError )
    	ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));
}



//---------------------------------------------------------
void ofSoundStreamStart(){
	int err = Pa_StartStream( stream );
    if( err != paNoError )
    	ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));
}


//---------------------------------------------------------
void ofSoundStreamClose(){
	int err = Pa_Terminate();
	if( err != paNoError )
    	ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( err ));
}


//---------------------------------------------------------
void ofSoundStreamListDevices(){
	int numDevices;

	numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
    	ofLog(OF_LOG_ERROR,"PortAudio error: %s\n",Pa_GetErrorText( numDevices ));
    	return;
	}
	const   PaDeviceInfo *deviceInfo;

	for( int i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		cout << deviceInfo->name << endl;
		cout << "api: " << deviceInfo->hostApi << endl;
		cout << "max in channels" << deviceInfo->maxInputChannels << endl;
		cout << "max out channels" << deviceInfo->maxOutputChannels << endl;
		cout << "default sample rate:" << deviceInfo->defaultSampleRate << endl;
	}

}
