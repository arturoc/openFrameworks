ofBackground(0,0,0)


ofPushMatrix()

intFrameCycle = (ofGetFrameNum()%(30*8))/8
frameCycle = (ofGetFrameNum()%1200)/1200.

ofPushMatrix()
ofSetLineWidth(2)
ofNoFill()
ofTranslate(640,750,0)
ofRotate(frameCycle*360,0,1,0)
ofTranslate(-640,-750,0)
avgBuff=0
for i in range(-128,128):
    posInBuffer = int((i+128)/(ofGetWidth()/256.))*2+1
    sample = floatArray_getitem(buff,posInBuffer)
    sine = 200.*math.sin(i/float(ofGetWidth())*256*math.pi)
    avgBuff+=sample*sample
    """if ofRandom(0,1)>0.5:
        ofCircle(640+i*8,750+sine,sample*100)
    else:
        ofTriangle(640+i*8,750+sine,620+i*8,780+sine,660+i*8,780+sine)"""

    circleY = 750+sine
    ofPushMatrix()
    ofTranslate(640+i*4,circleY,0)
    ofRotate(90,0,1,0)
    ofTranslate(-640+i*4,-circleY,0)
    ofCircle(640+i*4,750+sine,sample*50)
    #ofTriangle(640+i*8,750+sine,620+i*8,780+sine,660+i*8,780+sine)
    ofPopMatrix()
ofPopMatrix()

avgBuff/=256.

ofSetLineWidth(4)

if(avgBuff>0.3):
    ofFill()
else:
    ofNoFill()

for y in range(5):
    for x in range(6):
        """if intFrameCycle==y*6+x:
            ofFill()
        else:
            ofNoFill()"""
        #ofNoFill()
        ofRect(340+105*x,390+50*y,110,50)

ofRect(340+105*x,390+50*y,110,50)

ofPopMatrix()







































































showCode = 1
codeX = 80
codeY = 0
codeColor = ofColor(200,200,200)
