#include "window.h"
#include <iostream>
#include <sstream>
#include <string.h>
#include <fstream>
#include <cstring>

using namespace std;

window::window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::window)
{
    // Setting up the UI and buttons
    ui->setupUi(this);
    text = this->ui->output;
    this->ui->disconnectButton->setEnabled(false);
    this->ui->procedureButton->setEnabled(false);
    this->ui->comboBox->setEnabled(false);
    this->ui->parameterBox->setEnabled(false);
}

window::~window()
{
    // UI destructor on closing window
    window::on_disconnectButton_clicked();
    delete ui;
}


void window::on_connectButton_clicked()
{
    // Reading and converting IP and port values
    QString qip = this->ui->ipBox->text();
    QByteArray ipba = qip.toLocal8Bit();
    const char *ip = ipba.data();
    QString qport = this->ui->portBox->text();
    QByteArray portba = qport.toLocal8Bit();
    const char *port = portba.data();


    // Socket initialization
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        cerr << "WSAStartup failed: " << iResult << endl;
    }

    sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        cerr << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
    }

    // Connecting to the server
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ip);
    servAddr.sin_port = htons(atoi(port));
    iResult = ::connect(sock, (SOCKADDR *) &servAddr, sizeof(servAddr));
    if (iResult == SOCKET_ERROR) {
        cerr << "Error connecting to server: " << WSAGetLastError() << endl;
        text->document()->setPlainText("Unable to connect to server with given parameters.");
        closesocket(sock);
        WSACleanup();
    } else {
        // Setting up variables and buttons
        isListSent = false;
        this->ui->connectButton->setEnabled(false);
        this->ui->procedureButton->setEnabled(true);
        this->ui->disconnectButton->setEnabled(true);
        this->ui->comboBox->setEnabled(true);
        this->ui->parameterBox->setEnabled(true);
        text->document()->setPlainText("Connected to server.");

        // Getting procedure list
        if(!isListSent) {
            char recvBuf[2048];
            iResult = recv(sock, recvBuf, 2048, 0);
            if (iResult > 0) {
                string tmpstr(recvBuf, 2048);
                istringstream  is(tmpstr);
                string line;
                QString templine;

                // Adding procedures to drop-down list
                while (getline(is,line)) {
                    templine = QString::fromStdString(line);
                    this->ui->comboBox->addItem(templine);
                }
                int lastItem = this->ui->comboBox->count() - 1;
                this->ui->comboBox->removeItem(lastItem);
                isListSent = true;
            }
            else if (iResult == 0) {
                cout << "Connection closed" << endl;
            }
            else {
                cerr << "Error receiving message: " << WSAGetLastError() << endl;
            }
        }
    }

}


void window::on_disconnectButton_clicked()
{
    // Closing the socket
    const char *msg = "disconnect";
    int iResult = send(sock, msg, strlen(msg), 0);
    if (iResult == SOCKET_ERROR) {
        cerr << "Error sending message: " << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
    }

    iResult = closesocket(sock);
    if (iResult == SOCKET_ERROR) {
        cerr << "Error closing socket: " << WSAGetLastError() << endl;
    }
    WSACleanup();

    // Setting up variables and buttons, clearing textbox, displaying disconnect message
    text->document()->setPlainText("Disconnected from the server.");
    isListSent = false;
    this->ui->comboBox->clear();
    this->ui->parameterBox->clear();
    this->ui->connectButton->setEnabled(true);
    this->ui->procedureButton->setEnabled(false);
    this->ui->disconnectButton->setEnabled(false);
    this->ui->comboBox->setEnabled(false);
    this->ui->parameterBox->setEnabled(false);
}


void window::on_procedureButton_clicked()
{
    // Reading value from drop-down list (procedure) and sending message
    QString qchoice= this->ui->comboBox->currentText();
    QByteArray choiceba = qchoice.toLocal8Bit();
    char *choice = choiceba.data();

    QString qparameter = this->ui->parameterBox->text();
    QByteArray parameterba = qparameter.toLocal8Bit();
    char *parameter = parameterba.data();

    char *input = new char[strlen(parameter) + strlen(choice) + 1];
    strcpy(input, choice);
    strcat(input, "$");
    strcat(input, parameter);

    // Sending chosen procedure
    int iResult = send(sock, input, strlen(input), 0);
    if (iResult == SOCKET_ERROR) {
        //cerr << "Error sending message: " << WSAGetLastError() << endl;
        //closesocket(sock);
        //WSACleanup();
        window::on_disconnectButton_clicked();
    }

    // Receiving output
    char recvBuf[2048];
    iResult = recv(sock, recvBuf, 2048, 0);
    if (iResult > 0) {
        text->document()->setPlainText(recvBuf);
    }
}

