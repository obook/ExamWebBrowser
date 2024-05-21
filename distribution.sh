rm -R ./bin/linux
mkdir -p ./bin/linux/images
cp ./src/build/Desktop_Qt_6_5_3_GCC_64bit-Release/ExamWebBrowser ./bin/linux/
cp ./media/ExamWebBrowser.png ./bin/linux/images
cp ./media/home.svg ./bin/linux/images
zip -r ExamWebBrowser_Linux.zip ./bin/linux
# X:\Qt\6.7.0\msvc2019_64\bin\windeployqt X:\ExamWebBrowser\bin\ExamWebBrowser
