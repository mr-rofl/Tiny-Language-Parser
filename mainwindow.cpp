#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "source.h"
#include "QFileDialog"
#include "QMessageBox"
#include "QString"
#include "QFile"
#include "QTextStream"
#include "QDebug"
#include "QPlainTextEdit"

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
    QString filename=QFileDialog::getOpenFileName(this,tr("openfile"),"C://","text file(*.txt)") ;
    if(filename==""){return;}
    string error="";
    string path= (filename).toStdString();
    bool x=myinit(path,&error);

    if(!x){
        QMessageBox::warning(this,"ERROR",QString::fromStdString(error)) ;

    }

}

void MainWindow::on_pushButton_2_clicked()
{
    string error="";

    QString s4=ui->plainTextEdit->toPlainText();

    QString path = "script2.txt";


    qDebug()<<s4;

    write(s4.toStdString(),path.toStdString());
    bool x=myinit(path.toStdString(),&error);

    if(!x){
        QMessageBox::warning(this,"ERROR",QString::fromStdString(error)) ;

    }

}

