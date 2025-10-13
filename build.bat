REM This is optional bat file to rebuild both DLL and EXE
REM You can run it from command line or integrate into VSCode tasks

g++ -o GomiSaver.exe GomiSaver.c -lgdi32 -mwindows
