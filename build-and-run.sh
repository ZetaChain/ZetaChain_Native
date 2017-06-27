#!/usr/bin/env bash
sudo ./gradlew clean -POS=linux
sudo ./gradlew build -POS=linux
# gdb r build\exe\testApp\TestApp.exe
build\exe\testApp\TestApp.exe