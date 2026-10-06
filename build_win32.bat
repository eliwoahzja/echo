@echo off
@setlocal

rem Build for Release x64 with Visual Studio toolchain.
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 >nul
    goto :vs_env_ready
)

if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64 >nul
    goto :vs_env_ready
)

echo Nie znaleziono VS2019 Dev Environment. Ustaw VS Developer Command Prompt recznie.
exit /b 1

:vs_env_ready

msbuild example_win32_directx11.sln /m /t:Build /p:Configuration=Release /p:Platform=x64