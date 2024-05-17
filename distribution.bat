del .\bin\ExamWebBrowser.exe
xcopy .\build\ExamWebBrowser.exe .\bin\
X:\Qt\6.7.0\msvc2019_64\bin\windeployqt X:\ExamWebBrowser\bin\ExamWebBrowser.exe
tar.exe -a -cf ExamWebBrowser.zip bin
"X:\Program Files (x86)\NSIS\makensis.exe" ExamWebBrowser.nsi
