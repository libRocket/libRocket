#!/bin/bash


function build_universal()
{
	xcodebuild -configuration Release -sdk iphoneos5.0 -alltargets clean
	xcodebuild -configuration Release -sdk iphoneos5.0 -target $1 build
	cp build/Release-iphoneos/lib$1.a ./lib$1-arm.a
	
	xcodebuild -configuration Release -sdk iphonesimulator5.0 -alltargets clean
	xcodebuild -configuration Release -sdk iphonesimulator5.0 -target $1 build
	cp build/Release-iphonesimulator/lib$1.a ./lib$1-sim.a
	
	lipo -output ../bin/lib$1.a -create lib$1-arm.a -arch i386 lib$1-sim.a
        rm lib$1-{arm,sim}.a
}


build_universal "RocketCoreiOS"
build_universal "RocketControlsiOS"
build_universal "RocketFreeTypeiOS"
build_universal "RocketDebuggeriOS"
