@echo off
call gradlew clean
call gradlew build
rem call gdb r build\exe\testApp\TestApp.exe
call build\exe\testApp\TestApp.exe