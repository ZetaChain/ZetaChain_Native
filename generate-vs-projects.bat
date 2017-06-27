@echo off

call gradlew cleanVisualStudio -POS=windows
call gradlew BlockchainCppVisualStudio -POS=windows
call gradlew TestAppVisualStudio -POS=windows
