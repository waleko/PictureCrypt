cd src/app

# app
qmake.exe C:\projects\picturecrypt\src\app\app.pro -spec win32-g++
C:/Qt/Tools/mingw530_32/bin/mingw32-make.exe all
C:/Qt/Tools/mingw530_32/bin/mingw32-make.exe clean -j8

cd ..
mkdir deploy
cd deploy
mkdir debug
mkdir release
copy "C:\projects\picturecrypt\src\app\build\Debug\PictureCrypt.exe" "C:\projects\picturecrypt\src\deploy\debug\PictureCrypt.exe"
copy "C:\projects\picturecrypt\src\app\build\Release\PictureCrypt.exe" "C:\projects\picturecrypt\src\deploy\release\PictureCrypt.exe"
cd debug
windeployqt.exe --debug PictureCrypt.exe
cd ../release
windeployqt.exe --release PictureCrypt.exe
copy "$env:QTDIR\bin\libstdc++-6.dll" .
copy "$env:QTDIR\bin\libwinpthread-1.dll" .
cd ../..

# Console
cd console
qmake.exe C:\projects\picturecrypt\src\console\console.pro -spec win32-g++
C:/Qt/Tools/mingw530_32/bin/mingw32-make.exe
cd ../deploy
mkdir console
copy "C:\projects\picturecrypt\src\console\release\picturecrypt.exe" "C:\projects\picturecrypt\src\deploy\console\picturecrypt.exe"
cd console
windeployqt.exe --release picturecrypt.exe
copy "$env:QTDIR\bin\libstdc++-6.dll" .
copy "$env:QTDIR\bin\libwinpthread-1.dll" .
cd ../..

# Tests

cd tests
qmake.exe C:\projects\picturecrypt\src\tests\tests.pro -spec win32-g++
C:/Qt/Tools/mingw530_32/bin/mingw32-make.exe
cd release
windeployqt.exe --release tests.exe
copy "$env:QTDIR\bin\libstdc++-6.dll" .
copy "$env:QTDIR\bin\libwinpthread-1.dll" .

cd ../../..