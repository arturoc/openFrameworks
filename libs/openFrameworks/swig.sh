swig -Wall -python -threads -c++ -Iapp -Icommunication -Ievents -Igraphics -Isound -Iutils -Ivideo -lcpointer.i -lcarrays.i openframeworks.i
mv openframeworks_wrap.cxx openframeworks_wrap.cpp
#-threads
