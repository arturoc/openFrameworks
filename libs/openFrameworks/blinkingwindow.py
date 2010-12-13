
for y in range(5):
    for x in range(6):
        if ofRandom(0,1)>0.5:
            ofFill()
        else:
            ofNoFill()
        ofRect(340+105*x,390+50*y,110,50)
