from openframeworks import *
import math
import os, sys, subprocess


showCode = 1
codeX = 20
codeY = 380
codeColor = ofColor(100,100,100)
buff = 0;


def easeIn(t,b,c,d):
    s = 1.70158
    t = t/float(d)
    postFix = t
    return c*(postFix)*t*((s+1)*t - s) + b

class testApp(ofBaseApp):
    def setup(self):
		self.time = 0
		self.beginning = 100
		self.change = 600
		self.duration = 1000
		self.counter = 0.0
		ofSetVerticalSync(1)
		ofSetBackgroundAuto(0)
		ofSoundStreamSetup(0,2,self,44100,256,2);
		self.video = ofVideoPlayer()
		self.video.loadMovie("fingers.mov")
		self.video.setLoopState(OF_LOOP_NORMAL)
		self.video.play()

    def update(self):
        self.counter += 0.1
        if self.time < self.duration: self.time = self.time + 1
        else:
            self.change = -600
    def draw(self):
		file = open(os.getcwd() + '/draw.py', 'r')
		source = file.read()
		file.close()
		ofPushMatrix()
		try:
			exec(source)
			ofPopMatrix()
		except:
			ofPopMatrix()
			ofSetColor(255,255,255)
			ofDrawBitmapString(str(sys.exc_info()),20,20)
			
		if showCode:
			ofSetColor(codeColor)
			ofDrawBitmapString(source,codeX,codeY)
		            
    def mousePressed(self,x,y,b): self.time=0

    def audioReceived(self,buffer,bufferSize,nChannels):
        global buff;
        buff = buffer
        #print buffer
        #print floatArray_getitem(buffer,10)
        
ofSetupOpenGL(800,700, OF_WINDOW)
ofRunApp(testApp())
