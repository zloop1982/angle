@echo off

@echo off

echo./*
echo. * Check VC++ environment...
echo. */
echo.

set FOUND_VC=0

if defined VS120COMNTOOLS (
    set VSTOOLS="%VS120COMNTOOLS%"
    set VC_VER=120
    set FOUND_VC=1
) 

set VSTOOLS=%VSTOOLS:"=%
set "VSTOOLS=%VSTOOLS:\=/%"

set VSVARS="%VSTOOLS%vsvars32.bat"

if not defined VSVARS (
    echo Can't find VC2013 installed!
    goto ERROR
)

rmdir ..\projects\src\ARM /s/q
rmdir ..\projects\src\Debug /s/q
rmdir ..\projects\src\Release /s/q
rmdir .\Redist /s/q
echo./*
echo. * Building libraries...
echo. */
echo.

call %VSVARS%
if %FOUND_VC%==1 (
    msbuild  ..\projects\src\angle_UA.sln /p:Configuration="Debug" /p:Platform="Win32" /t:Clean,Build
    msbuild  ..\projects\src\angle_UA.sln /p:Configuration="Release" /p:Platform="Win32" /t:Clean,Build
    msbuild  ..\projects\src\angle_UA.sln /p:Configuration="Debug" /p:Platform="ARM" /t:Clean,Build
    msbuild  ..\projects\src\angle_UA.sln /p:Configuration="Release" /p:Platform="ARM" /t:Clean,Build
) else (
    echo Script error.
    goto ERROR
)

echo. * Copying winrt libraries...
set IN_DIR=.\..\projects\src\Release
set OUT_DIR=.\Redist\winrt\win32\
xcopy "%IN_DIR%\libEGL_UA.Windows\libEGL.dll" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libEGL_UA.Windows\libEGL.lib" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libGLESv2_UA.Windows\libGLESv2.lib" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libGLESv2_UA.Windows\libGLESv2.dll" "%OUT_DIR%"  /iycq

set IN_DIR=.\..\projects\src\ARM\Release
set OUT_DIR=.\Redist\winrt\arm\
xcopy "%IN_DIR%\libEGL_UA.Windows\libEGL.dll" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libEGL_UA.Windows\libEGL.lib" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libGLESv2_UA.Windows\libGLESv2.lib" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libGLESv2_UA.Windows\libGLESv2.dll" "%OUT_DIR%"  /iycq

echo. * Copying wp8_1 libraries...
set IN_DIR=.\..\projects\src\Release
set OUT_DIR=.\Redist\wp8_1\win32\
xcopy "%IN_DIR%\libEGL_UA.WindowsPhone\libEGL.dll" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libEGL_UA.WindowsPhone\libEGL.lib" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libGLESv2_UA.WindowsPhone\libGLESv2.lib" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libGLESv2_UA.WindowsPhone\libGLESv2.dll" "%OUT_DIR%"  /iycq

set IN_DIR=.\..\projects\src\ARM\Release
set OUT_DIR=.\Redist\wp8_1\arm\
xcopy "%IN_DIR%\libEGL_UA.WindowsPhone\libEGL.dll" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libEGL_UA.WindowsPhone\libEGL.lib" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libGLESv2_UA.WindowsPhone\libGLESv2.lib" "%OUT_DIR%"  /iycq
xcopy "%IN_DIR%\libGLESv2_UA.WindowsPhone\libGLESv2.dll" "%OUT_DIR%"  /iycq

echo. * Copying include files...
set OUT_DIR=.\Redist\include\
xcopy ".\..\include" "%OUT_DIR%" /eiycq
xcopy ".\..\src\common\winrtangle.h" "%OUT_DIR%" /iycq
xcopy ".\..\samples\gles2_book\Common\esUtil.h" "%OUT_DIR%" /iycq

echo. *Build complete.

goto EOF

:ERROR
pause

:EOF