@echo off
call gradlew clean -POS=windows
call gradlew build -POS=windows
call build\exe\testApp\TestApp.exe > output.log
rem call build\exe\testApp\TestApp.exe --nosha256 > outputnoSha.log