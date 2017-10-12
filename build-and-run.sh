#!/usr/bin/env bash 
rm -f TestApp
# sudo ./setup-dependancies.sh 
sudo ./gradlew clean -POS=linux 
sudo ./gradlew build -POS=linux 
cp -f build/testApp/TestApp TestApp 
# gdb r build\testApp\TestApp 
# TestApp | output.log 
TestApp --noOpenCL | outputnoCL.log
# TestApp --useJSONFormat | outputJSON.log
# TestApp --noSHA256 | outputSHA256.log