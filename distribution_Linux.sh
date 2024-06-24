rm -R ./bin/ExamWebBrowser_linux
rm -R ./distribution/
mkdir -p ./bin/ExamWebBrowser_linux/images
mkdir -p ./bin/ExamWebBrowser_linux/images
mkdir -p ./distribution
cp ./src/build/Desktop_Qt_6_5_3_GCC_64bit-Release/ExamWebBrowser ./bin/ExamWebBrowser_linux/
cp ./images/ExamWebBrowser.png ./bin/ExamWebBrowser_linux/images
cp ./images/home.svg ./bin/ExamWebBrowser_linux/images
cd ./bin
zip -r ./../distribution/ExamWebBrowser_Linux.zip ./ExamWebBrowser_linux
