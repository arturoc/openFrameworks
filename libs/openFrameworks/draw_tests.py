ofBackground(0,0,0)
ofSetColor(255,255,255)


#self.video.update()
#self.video.draw(0,0)

class particle:
    def __init__(self):
        self.pos = ofPoint(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()))
        self.frc = ofPoint(0,0,0)
    
    def update(self,frc_):
        self.frc = frc_
        self.pos *= self.frc

    def draw(self):
        ofCircle(self.pos,10);

def getPixelCoords(pixelNum):
    pixelY = int(pixelNum/ofGetWidth())
    pixelX = pixelNum-pixelY*ofGetWidth()
    return [pixelX,pixelY]

pixelNum = ofGetFrameNum()%(ofGetWidth()*ofGetHeight())

global particles
particles=[particle() for i in range(0,100)]
#particles[i] = particle()
#part.update(ofPoint(0,9,0));
#part.draw()

"""for p in particles:
    ofDrawBitmapString( str(p.pos.__getattr__(x))), 20,40)
    for pp in particles:
        distx = p.pos.x - pp.pos.x
        disty = p.pos.y - pp.pos.y
        sqdist = distx*distx + disty*disty
        p.update((p.pos-pp.pos).normalize()*sqdist)"""

"""for p in particles:
    p.draw()"""

"""
for i in range(256)
    buff[
"""

ofSetColor(255,255,255)
"""if buff!=0:
    for i in range(256):
        ofLine(i,100,i,100+floatArray_getitem(buff,i*2+1)*100.)
"""


frameCycle = (ofGetFrameNum()%600)/600.
#ofDrawBitmapString(str(frameCycle),20,20)

ofPushMatrix()
ofTranslate(ofGetWidth()/2.,ofGetHeight()/2)
ofRotate((ofGetFrameNum()%1200.)/1200.*360.,0,1,0);
ofTranslate(-ofGetWidth()/2.,-ofGetHeight()/2)


"""
for j in range(0,8):
    for i in range(0,1000):
        ofSetColor(int(255.*(i/1000.)),int(255.*(i/1000.)),int(255.*(i/1000.)))
        ofLine(i,j*100+40+30*(math.sin((i/1000.+(ofGetFrameNum()%60)/60.)*2*math.pi)+ofNoise(i/1000.+(ofGetFrameNum()%60)/60.)),i,j*100+45+30*(math.sin((i/1000.+(ofGetFrameNum()%60)/60.)*2*math.pi)+ofNoise(i/1000.-(ofGetFrameNum()%60)/60.)))
"""


for i in range(0,ofGetWidth()):
    ofNoFill()
    ofSetColor(int(255.*(i/float(ofGetWidth()))),int(255.*(i/float(ofGetWidth()))),int(255.*(i/float(ofGetWidth()))))
    posInBuffer = int(i/(ofGetWidth()/256.))*2+1
    sample = floatArray_getitem(buff,posInBuffer)
    noise = ofNoise(i/float(ofGetWidth())*frameCycle)
    abssample = abs(sample)
    width = abssample*100.;
    colorMultiplier=10
    gray=100+int(200.*abssample*colorMultiplier)
    sine = math.sin(i/1000.*frameCycle*16.*math.pi)
    ofSetColor(gray,gray,gray)
    ofPushMatrix()
    ofTranslate(i,200)
    ofRotate(90,0,1,0)
    ofTranslate(-i,-200)
    #ofTranslate(0,100.*sine,-100.*sine)
    #ofLine(i,200-width,i,200+width)
    ofCircle(i,200,width)
    ofPopMatrix()


ofPopMatrix()

ofSetColor(255,255,255)
#ofDrawBitmapString(str(ofGetFrameRate()),20,20)

"""
for i in range(100):
    pixelNumI = pixelNum*(i+100) % (ofGetWidth()*ofGetHeight())
    pixel = getPixelCoords(pixelNumI)
    ofRect(pixel[0],pixel[1],2,2)
    #ofDrawBitmapString(str(pixelNumI),20+50*i,20)
"""

#ofSetColor(0,0,0,10)
#ofRect(0,0,ofGetWidth(),ofGetHeight())

showCode = 1
codeX = 20
codeY = 200
codeColor = ofColor(100,100,100)









