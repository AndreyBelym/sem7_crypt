#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <cstdio>
#include <sstream>
#include "des.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString rand64(){
    QString t,t1;
    for(int i=0;i<8;i++)
        t+=t1.sprintf("%02x",rand()%256);
    return t;
}
bool parseHex(QString &str,unsigned char v[8]){
    if (str.length()!=16)
        return false;
    bool ok;
    for(int i=0;i<8;i++){
        v[i]=str.mid(2*i,2).toInt(&ok,16);
        if(!ok)
            return false;
    }
    return true;
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->lineEdit->setText(rand64());
}

void MainWindow::on_pushButton_5_clicked()
{
    if(!parseHex(ui->lineEdit->text(),key))
        QMessageBox::warning(this,"","Неправильный ключ!");

    create_key(key,keys);
    std::stringstream s;
    for(int i=0;i<16;i++){
        s<<i+1<<". ";
        print_block(s,keys[i],6,6);
        s<<std::endl;
    }
    ui->plainTextEdit->setPlainText(s.str().c_str());
;}
