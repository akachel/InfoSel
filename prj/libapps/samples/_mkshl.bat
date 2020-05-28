@echo off

rem Makes an user application of the InfoSel++ library as a program with the programmatic shell user interface,
rem the target program is linked with the main algorithm packages assembly and a given user algorithm package

call "%VS80COMNTOOLS%vsvars32.bat"
cl.exe /O2 /Ob1 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /GF /FD /EHsc /MD /Gy /Fo"out\\" /Fd"out\\" /W0 /c "..\src\%1.cpp"
cl.exe /O2 /Ob1 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /GF /FD /EHsc /MD /Gy /Fo"out\\" /Fd"out\\" /W0 /c "..\src\_packuse.cxx"
cl.exe /O2 /Ob1 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /GF /FD /EHsc /MD /Gy /Fo"out\\" /Fd"out\\" /W0 /c "..\src\shlmain.cpp"
link.exe /OUT:"..\bin\%1.exe" /INCREMENTAL:NO /MANIFEST /MANIFESTFILE:"out\%1.exe.manifest" /SUBSYSTEM:CONSOLE /OPT:NOREF /MACHINE:I386 "out\shlmain.obj" "out\_packuse.obj" "out\%1.obj" "..\lib\iselpacks.lib" "..\lib\infosel++.lib" kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
mt.exe /outputresource:"..\bin\%1.exe;#1" /manifest "out\%1.exe.manifest"
