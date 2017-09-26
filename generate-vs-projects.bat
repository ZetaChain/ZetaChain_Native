@echo off

call gradlew cleanVisualStudio -POS=windows
call gradlew ZetaChain_NativeVisualStudio -POS=windows
call gradlew TestAppVisualStudio -POS=windows
