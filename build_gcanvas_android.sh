#!/bin/sh

cd third_party/GCanvas
echo $PWD

gradlew clean :android:gcanvas_library:assembleStandalone