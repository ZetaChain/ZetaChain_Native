@echo off
del /F TestApp.exe
call gradlew clean -POS=windows
call gradlew build -POS=windows
echo F | xcopy /Q /Y /F build\exe\testApp\TestApp.exe TestApp.exe
rem call TestApp.exe > output.log
call TestApp.exe --noOpenCL > outputnoCL.log 
rem call TestApp.exe --useJSONFormat > outputJSON.log
rem call TestApp.exe --nosha256 > outputnoSha.log