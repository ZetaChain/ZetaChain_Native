#!/usr/bin/env bash
rm -f TestApp
sudo ./gradlew clean -POS=linux
sudo ./gradlew build -POS=linux
cp -f build/testApp/TestApp TestApp
# gdb r build\testApp\TestApp
build/testApp/TestApp | output.log