#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <winsock2.h>
#include "ui_window.h"

QT_BEGIN_NAMESPACE
namespace Ui { class window; }
QT_END_NAMESPACE

class window : public QMainWindow
{
    Q_OBJECT

public:
    window(QWidget *parent = nullptr);
    ~window();

private slots:
    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

    void on_procedureButton_clicked();

private:
    // Setting up UI variable and variables needed for few functions
    Ui::window *ui;
    SOCKET sock;
    QPlainTextEdit *text;
    bool isListSent;
};

#endif // WINDOW_H
