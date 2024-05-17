mkdir .\bin\Windows\
del .\bin\Windows\ExamWebBrowser.exe
xcopy .\src\build\Desktop_Qt_6_7_0_MSVC2019_64bit-Release\ExamWebBrowser.exe .\bin\Windows\
xcopy .\media\ExamWebBrowser.png .\bin\Windows\
X:\Qt\6.7.0\msvc2019_64\bin\windeployqt X:\ExamWebBrowser\bin\Windows\ExamWebBrowser.exe
tar.exe -a -cf ExamWebBrowser.zip bin
"X:\Program Files (x86)\NSIS\makensis.exe" ExamWebBrowser.nsi
