@echo off
if exist "%1" (
    copy "%1" "D:\Program Files\Guild Wars 2\Guild Wars 2\addons\" /Y
    echo DLL copied successfully.
) else (
    echo DLL not found.
)