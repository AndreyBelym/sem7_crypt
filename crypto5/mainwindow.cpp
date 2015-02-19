#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <cstdio>
#include <fstream>
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

void MainWindow::on_pushButton_4_clicked()
{
    ui->lineEdit_2->setText(rand64());
}
using namespace std;
void MainWindow::on_pushButton_5_clicked()
{
    if(!parseHex(ui->lineEdit->text(),key))
        QMessageBox::warning(this,"","Неправильный ключ!");
    if(!parseHex(ui->lineEdit_2->text(),IV))
        QMessageBox::warning(this,"","Неправильное значение инициализации!");
    if(infile.endsWith(".enc"))
        outfile=infile.left(infile.length()-4);
    else
        outfile=infile+".enc";
    for(int i=0;i<8;i++){
        cout<<setw(2)<<setfill('0')<<hex<<(int)key[i];
    }
    cout<<endl;
    for(int i=0;i<8;i++){
        cout<<setw(2)<<setfill('0')<<hex<<(int)IV[i];
    }
    cout<<endl;
    std::fstream ifile(infile.toUtf8().constData(),std::ios::in|std::ios::binary),
            ofile(outfile.toUtf8().constData(),std::ios::out|std::ios::binary);

    des_ofb_crypt(ifile,key,IV,ofile);
    ifile.close(); ofile.close();
;}
