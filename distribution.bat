del .\bin\ExamWebBrowser.exe
xcopy .\build\ExamWebBrowser.exe .\bin\
REM X:\Qt\6.7.0\msvc2019_64\bin\windeployqt --libdir bin/lib X:\ExamWebBrowser\bin\ExamWebBrowser.exe
X:\Qt\6.7.0\msvc2019_64\bin\windeployqt X:\ExamWebBrowser\bin\ExamWebBrowser.exe
tar.exe -a -cf ExamWebBrowser.zip bin
