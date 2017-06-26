@echo off
call gradlew clean -POS=windows
call gradlew build -POS=windows
rem call gdb r build\exe\testApp\TestApp.exe
call build\exe\testApp\TestApp.exe