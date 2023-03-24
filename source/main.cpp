/*
 * Compilation of standalone application:
 * In PowerShell: .\Qt\6.4.2\mingw_64\bin> .\windeployqt.exe "D:\Qt\client_tcp\build-client_tcp-Desktop_Qt_6_4_2_MinGW_64_bit-Release\release\client_tcp.exe"
 * Copy libgcc_s_seh-1.dll, libstdc++-6, libwinpthread-1 to application folder
*/

#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    // Setting up aplication window
    QApplication a(argc, argv);
    window w;
    w.show();
    return a.exec();
}
