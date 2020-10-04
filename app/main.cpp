#include "mainwindow.h"
using namespace std;
#include "Img.h"


bool check(int argc, char *argv[]) {
    if (argc != 2)
        return false;
    if (QDir(QString::fromUtf8(argv[1])).exists() && QDir(QString::fromUtf8(argv[1])).isReadable())
        return true;
    return false;
}

QString corPath(char *str) {
    if(str == NULL) 
        return "";
    if(str[strlen(str) - 1] == '/')
        return QString::fromUtf8(str);
    return QString::fromUtf8(str) + "/";
}

int main(int argc, char *argv[])
{
    bool flag = check(argc, argv);
    QApplication a(argc, argv);
    MainWindow window(corPath(argv[1]), flag);
    window.show();
    int i = a.exec();
    return i;
    //ImageFile::getImageFile("/Users/olytvynenk/Desktop/2.png")->setArt("/Users/olytvynenk/Desktop/ОльгаЮкина.mp3");
}