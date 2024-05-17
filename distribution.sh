mkdir -p ./bin/linux/
rm ./bin/linux/ExamWebBrowser
cp ./build/Linux-Release/ExamWebBrowser ./bin/linux/
cp ./media/ExamWebBrowser.png ./bin/linux/
# X:\Qt\6.7.0\msvc2019_64\bin\windeployqt X:\ExamWebBrowser\bin\ExamWebBrowser
# tar.exe -a -cf ExamWebBrowser.zip bin
# "X:\Program Files (x86)\NSIS\makensis.exe" ExamWebBrowser.nsi
