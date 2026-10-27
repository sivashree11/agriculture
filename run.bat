@echo off
REM ==============================================================================
REM Batch script to Compile and Launch the Smart Agricultural By-Product Marketplace
REM Platform: Windows (MinGW / MSYS2 g++)
REM ==============================================================================

echo ==========================================================
echo  Compiling Smart Agricultural By-Product Marketplace...
echo ==========================================================

g++ -std=c++17 main.cpp -o marketplace.exe -lws2_32

if %ERRORLEVEL% EQU 0 (
    echo Compilation successful!
    echo Starting C++ Server on http://localhost:8080 ...
    marketplace.exe
) else (
    echo Compilation failed! Please verify g++ is in your PATH.
    pause
)
