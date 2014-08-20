@ECHO OFF
REM
REM Copyright (c) 2013 The ANGLE Project Authors. All rights reserved.
REM Use of this source code is governed by a BSD-style license that can be
REM found in the LICENSE file.
REM

PATH %PATH%;%ProgramFiles(x86)%\Windows Kits\8.0\bin\x86;%DXSDK_DIR%\Utilities\bin\x86

setlocal
set errorCount=0
set successCount=0
set debug=0

if "%1" == "debug" (
    set debug=1
)
if "%1" == "release" (
    set debug=0
)

::              | Input file          | Entry point           | Type | Output file                        | Debug |
call:BuildShader Passthrough2D11.hlsl VS_Passthrough2D         vs_4_0_level_9_1 compiled\winrt\passthrough2d11vs.h         %debug%
REM call:BuildShader Passthrough2D11.hlsl PS_PassthroughDepth2D    ps_4_0_level_9_1 compiled\winrt\passthroughdepth2d11ps.h    %debug%
call:BuildShader Passthrough2D11.hlsl PS_PassthroughRGBA2D     ps_4_0_level_9_1 compiled\winrt\passthroughrgba2d11ps.h     %debug%
REM call:BuildShader Passthrough2D11.hlsl PS_PassthroughRGBA2DUI   ps_4_0_level_9_1 compiled\winrt\passthroughrgba2dui11ps.h   %debug%
REM call:BuildShader Passthrough2D11.hlsl PS_PassthroughRGBA2DI    ps_4_0_level_9_1 compiled\winrt\passthroughrgba2di11ps.h    %debug%
call:BuildShader Passthrough2D11.hlsl PS_PassthroughRGB2D      ps_4_0_level_9_1 compiled\winrt\passthroughrgb2d11ps.h      %debug%
REM call:BuildShader Passthrough2D11.hlsl PS_PassthroughRGB2DUI    ps_4_0_level_9_1 compiled\winrt\passthroughrgb2dui11ps.h    %debug%
REM call:BuildShader Passthrough2D11.hlsl PS_PassthroughRGB2DI     ps_4_0_level_9_1 compiled\winrt\passthroughrgb2di11ps.h     %debug%
call:BuildShader Passthrough2D11.hlsl PS_PassthroughRG2D       ps_4_0_level_9_1 compiled\winrt\passthroughrg2d11ps.h       %debug%
REM call:BuildShader Passthrough2D11.hlsl PS_PassthroughRG2DUI     ps_4_0_level_9_1 compiled\winrt\passthroughrg2dui11ps.h     %debug%
REM call:BuildShader Passthrough2D11.hlsl PS_PassthroughRG2DI      ps_4_0_level_9_1 compiled\winrt\passthroughrg2di11ps.h      %debug%
call:BuildShader Passthrough2D11.hlsl PS_PassthroughR2D        ps_4_0_level_9_1 compiled\winrt\passthroughr2d11ps.h        %debug%
REM call:BuildShader Passthrough2D11.hlsl PS_PassthroughR2DUI      ps_4_0_level_9_1 compiled\winrt\passthroughr2dui11ps.h      %debug%
REM call:BuildShader Passthrough2D11.hlsl PS_PassthroughR2DI       ps_4_0_level_9_1 compiled\winrt\passthroughr2di11ps.h       %debug%
call:BuildShader Passthrough2D11.hlsl PS_PassthroughLum2D      ps_4_0_level_9_1 compiled\winrt\passthroughlum2d11ps.h      %debug%
call:BuildShader Passthrough2D11.hlsl PS_PassthroughLumAlpha2D ps_4_0_level_9_1 compiled\winrt\passthroughlumalpha2d11ps.h %debug%


call:BuildShader Swizzle11.hlsl       PS_SwizzleF2D            ps_4_0_level_9_1 compiled\winrt\swizzlef2dps.h              %debug%
REM call:BuildShader Swizzle11.hlsl       PS_SwizzleI2D            ps_4_0_level_9_1 compiled\winrt\swizzlei2dps.h              %debug%
REM call:BuildShader Swizzle11.hlsl       PS_SwizzleUI2D           ps_4_0_level_9_1 compiled\winrt\swizzleui2dps.h             %debug%

REM call:BuildShader Swizzle11.hlsl       PS_SwizzleF3D            ps_4_0_level_9_1 compiled\winrt\swizzlef3dps.h              %debug%
REM call:BuildShader Swizzle11.hlsl       PS_SwizzleI3D            ps_4_0_level_9_1 compiled\winrt\swizzlei3dps.h              %debug%
REM call:BuildShader Swizzle11.hlsl       PS_SwizzleUI3D           ps_4_0_level_9_1 compiled\winrt\swizzleui3dps.h             %debug%

REM call:BuildShader Swizzle11.hlsl       PS_SwizzleF2DArray       ps_4_0_level_9_1 compiled\winrt\swizzlef2darrayps.h         %debug%
REM call:BuildShader Swizzle11.hlsl       PS_SwizzleI2DArray       ps_4_0_level_9_1 compiled\winrt\swizzlei2darrayps.h         %debug%
REM call:BuildShader Swizzle11.hlsl       PS_SwizzleUI2DArray      ps_4_0_level_9_1 compiled\winrt\swizzleui2darrayps.h        %debug%

call:BuildShader Clear11.hlsl         VS_ClearFloat            vs_4_0_level_9_1 compiled\winrt\clearfloat11vs.h            %debug%
call:BuildShader Clear11.hlsl         PS_ClearFloat            ps_4_0_level_9_1 compiled\winrt\clearfloat11ps.h            %debug%

REM call:BuildShader Clear11.hlsl         VS_ClearUint             vs_4_0_level_9_1 compiled\winrt\clearuint11vs.h             %debug%
REM call:BuildShader Clear11.hlsl         PS_ClearUint             ps_4_0_level_9_1 compiled\winrt\clearuint11ps.h             %debug%

REM call:BuildShader Clear11.hlsl         VS_ClearSint             vs_4_0_level_9_1 compiled\winrt\clearsint11vs.h             %debug%
REM call:BuildShader Clear11.hlsl         PS_ClearSint             ps_4_0_level_9_1 compiled\winrt\clearsint11ps.h             %debug%

REM call:BuildShader BufferToTexture11.hlsl  VS_BufferToTexture        vs_4_0_level_9_1 compiled\winrt/buffertotexture11_vs.h      %debug%
REM call:BuildShader BufferToTexture11.hlsl  GS_BufferToTexture        gs_4_0 compiled\winrt/buffertotexture11_gs.h      %debug%
REM call:BuildShader BufferToTexture11.hlsl  PS_BufferToTexture_4F     ps_4_0_level_9_1 compiled\winrt/buffertotexture11_ps_4f.h   %debug%
REM call:BuildShader BufferToTexture11.hlsl  PS_BufferToTexture_4I     ps_4_0_level_9_1 compiled\winrt/buffertotexture11_ps_4i.h   %debug%
REM call:BuildShader BufferToTexture11.hlsl  PS_BufferToTexture_4UI    ps_4_0_level_9_1 compiled\winrt/buffertotexture11_ps_4ui.h  %debug%

echo.

if %successCount% GTR 0 (
   echo %successCount% shaders compiled successfully.
)
if %errorCount% GTR 0 (
   echo There were %errorCount% shader compilation errors.
)

endlocal
exit /b

:BuildShader
set input=%~1
set entry=%~2
set type=%~3
set output=%~4
set debug=%~5

if %debug% == 0 (
    set "buildCMD=fxc /nologo /E %entry% /T %type% /Fh %output% %input%"
) else (
    set "buildCMD=fxc /nologo /Zi /Od /E %entry% /T %type% /Fh %output% %input%"
)

set error=0
%buildCMD% || set error=1

if %error% == 0 (
    set /a successCount=%successCount%+1
) else (
    set /a errorCount=%errorCount%+1
)

exit /b
