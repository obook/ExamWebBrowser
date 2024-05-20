rm -R ./bin/linux
mkdir -p ./bin/linux/images
cp ./build/Linux-Release/ExamWebBrowser ./bin/linux/
cp ./media/ExamWebBrowser.png ./bin/linux/images
cp ./media/home.svg ./bin/linux/images
zip -r ExamWebBrowser_Linux.zip ./bin/linux
# X:\Qt\6.7.0\msvc2019_64\bin\windeployqt X:\ExamWebBrowser\bin\ExamWebBrowser
