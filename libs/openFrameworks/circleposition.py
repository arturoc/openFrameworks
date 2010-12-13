intFrameCycle = (ofGetFrameNum()%16)/8.
if intFrameCycle==1:
    ofFill()
else:
    ofNoFill()

ofCircle(640,750,20)
