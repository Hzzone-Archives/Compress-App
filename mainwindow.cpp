#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "component.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

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


//普通压缩按钮槽函数
void MainWindow::on_pushButton_clicked()
{
    QMessageBox chooseFileBox;
    chooseFileBox.setText("请选择需要压缩的文件!");
    chooseFileBox.exec();
    //选择待压缩文件窗口
    QString input_filename = QFileDialog::getOpenFileName(this,
        tr("Choose text file"), tr("Text files (*.txt)"));
    //QString转化为char*
    if(input_filename!=NULL){
        QByteArray b = input_filename.toLatin1();
        char* f1 = b.data();
        QMessageBox chooseCompressFileBox;
        chooseCompressFileBox.setText("请选择存放压缩文件的路径!");
        chooseCompressFileBox.exec();
        //压缩文件后缀名为.zip
        QString output_dir = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), QDir::currentPath());
        bool ok;
        QString text = QInputDialog::getText(this, tr("请输入压缩文件名!"),
                                             tr("File name:"), QLineEdit::Normal,
                                                 QDir::home().dirName(), &ok);
        QString output_filename = output_dir + "/" + text + ".zip";
        QByteArray aaaa = output_filename.toLatin1();
        char* f2 = aaaa.data();
       //分割压缩文件路径，获得待压缩文件的格式
        QStringList list1 = input_filename.split("/");
        QString temp1= list1.at(list1.size()-1);
        QStringList list2 = temp1.split(".");
        QString temp2= list2.at(list2.size()-1);
        const char* format = temp2.toStdString().c_str();
        char p[] = "";
        char* x = p;
        encode(f1, f2, x, format);
    }
}

//解压按钮
void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox chooseFileBox;
    chooseFileBox.setText("请选择需要解压的文件!");
    chooseFileBox.exec();
    //选择待压缩文件窗口
    QString input_filename = QFileDialog::getOpenFileName(this,
        tr("Choose text file"), tr("Text files (*.txt)"));
    if(input_filename!=NULL){
        //QString转化为char*
        QByteArray b = input_filename.toLatin1();
        char* f1 = b.data();
        if(isCompressFile(f1)){
            const char* secretcode = isSecretFile(f1);
            //判断是否是加密文件
            if(strlen(secretcode)==0){
                QMessageBox chooseDirBox;
                chooseDirBox.setText("请选择解压文件存放的路径!");
                chooseDirBox.exec();
                QString output_dir = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), QDir::currentPath());
                bool ok;
                QString text = QInputDialog::getText(this, tr("请输入解压文件名!"),
                                                     tr("File name:"), QLineEdit::Normal,
                                                         QDir::home().dirName(), &ok);
                QString output_filename = output_dir + "/" + text +"." + fileFormat(f1);
                QByteArray a = output_filename.toLatin1();
                char* f2 = a.data();
                decode(f1, f2);
            }else{
                //如果加密了，则弹出输入框输入密码
                bool ok;
                QString text = QInputDialog::getText(this, tr("请输入压缩密码"),
                                                     tr("compressing file secret code:"), QLineEdit::Password,
<<<<<<< HEAD
=======
               //选择解压文件路径,文件名比如test1.txt在前面加上decompress
>>>>>>> c8e53e6cebb25bd81c804c3626305eefe06d7f69
                                                         QDir::home().dirName(), &ok);
                //QString转化为char*
                const char* p = (text.toStdString()).c_str();
                cout<<"p"<<p<<endl;
                if(strcmp(p, secretcode)==0){
                    QMessageBox chooseDirBox;
                    chooseDirBox.setText("请选择解压文件存放的路径!");
                    chooseDirBox.exec();
                    QString output_dir = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), QDir::currentPath());
                    bool ok1;
                    QString text1 = QInputDialog::getText(this, tr("请输入解压文件名!"),
                                                         tr("File name:"), QLineEdit::Normal,
                                                             QDir::home().dirName(), &ok1);
                    QString output_filename = output_dir + "/" + text1 +"." + fileFormat(f1);
                    QByteArray a = output_filename.toLatin1();
                    char* f2 = a.data();
                    decode(f1, f2);
                }else{
                    QMessageBox warningBox;
                    warningBox.setText("加密压缩文件密码错误!");
                    warningBox.exec();
                }

            }
        }else{
            QMessageBox msgBox;
            msgBox.setText("请选择压缩格式的文件!");
            msgBox.exec();
        }
    }
}

//加密压缩按钮
//点击之后会弹出一个框输入压缩密码
void MainWindow::on_pushButton_2_clicked()
{
    //选择待压缩文件窗口
    QString input_filename = QFileDialog::getOpenFileName(this,
        tr("Choose text file"), tr("Text files (*.txt)"));
    //QString转化为char*
    if(input_filename!=NULL){
        QByteArray b = input_filename.toLatin1();
        char* f1 = b.data();
        QString output_dir = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), QDir::currentPath());
        bool ok1;
        QString text = QInputDialog::getText(this, tr("请输入解压文件名!"),
                                             tr("File name:"), QLineEdit::Normal,
                                                 QDir::home().dirName(), &ok1);
        QString output_filename = output_dir + "/" + text +"." + "zip";
        QByteArray d = output_filename.toLatin1();
        char* f2 = d.data();
        //输入密码
        bool ok2;
        QString text1 = QInputDialog::getText(this, tr("请输入密码!"),
                                             tr("secret code:"), QLineEdit::Password,
                                                 QDir::home().dirName(), &ok2);
        QByteArray c = text1.toLatin1();
        char* x = c.data();
        QStringList list1 = input_filename.split("/");
        QString temp1= list1.at(list1.size()-1);
        QStringList list2 = temp1.split(".");
        QString temp2= list2.at(list2.size()-1);
        const char* format = temp2.toStdString().c_str();
        encode(f1, f2, x, format);
    }
}
