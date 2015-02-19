#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <cstdio>
#include <fstream>
#include <vector>
#include <iomanip>
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

void MainWindow::on_pushButton_clicked()
{
    infile=QFileDialog::getOpenFileName(this);
    ui->label->setText(infile);
}

QString rand512(){
    QString t,t1;
    for(int i=0;i<512;i++)
        t+=t1.sprintf("%02x",rand()%256);
    return t;
}
bool parseHex(QString &str,unsigned char v[512]){
    if (str.length()!=1024)
        return false;
    bool ok;
    for(int i=0;i<512;i++){
        v[i]=str.mid(2*i,2).toInt(&ok,16);
        if(!ok)
            return false;
    }
    return true;
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->plainTextEdit->setPlainText(rand512());
}

void filemod(std::iostream& fs,unsigned char mod[512]){
    std::vector<unsigned char> buf;
    buf.insert(buf.begin(),mod,mod+512);
    unsigned char t;int i,j;
    fs>>std::noskipws;
    for(i=0;i<512&&fs>>t;i++){
        buf[i]^=t;
    }
    std::cout<<i<<" bytes read"<<std::endl;
    fs.clear();fs.seekp(0);
    for(j=0;j<i;j++){
        fs<<unsigned char(buf[j]);
    }
    std::cout<<j<<" bytes wrote"<<std::endl;

}
void MainWindow::on_pushButton_5_clicked()
{
    if(!parseHex(ui->plainTextEdit->toPlainText(),mod))
        QMessageBox::warning(this,"","Неправильный ключ!");

    std::fstream ifile(infile.toUtf8().constData(),std::ios::in|std::ios::out|std::ios::binary);
    filemod(ifile,mod);
;}
