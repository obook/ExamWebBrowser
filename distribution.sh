rm -R ./bin/linux
mkdir -p ./bin/linux
cp ./build/Linux-Release/ExamWebBrowser ./bin/linux/
cp ./media/ExamWebBrowser.png ./bin/linux/
zip -r ExamWebBrowser_Linux.zip ./bin/linux
# X:\Qt\6.7.0\msvc2019_64\bin\windeployqt X:\ExamWebBrowser\bin\ExamWebBrowser
