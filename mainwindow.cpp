#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Kuznyechik.hpp"
#include "mycrypto.hpp"
#include "argvparser.h"
using namespace CommandLineProcessing;

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <stdexcept>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(sendrand(QString)),this, SLOT(on_lineEdit_textChanged(QString)));  //iv
    connect(this, SIGNAL(sendkey(QString)),this, SLOT(on_lineEdit_2_textChanged(QString))); //key
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool logic_xor3(bool x, bool y, bool z) {
    return (!x && !y && z) || (!x && y && !z) || (x && !y && !z);
}
    // файл шифрования/расшифровывания
void MainWindow::on_pushButton_clicked()
{
    QString open_file =  QFileDialog::getOpenFileName(this, "Выбрать файл","","TXT(*.txt)");
    ui->label->setText(open_file);
}

    //ключ шифрования
void MainWindow::on_pushButton_2_clicked()
{
    QString key_file = QFileDialog::getOpenFileName(this, "Выбрать файл","","TXT(*.txt)");
    QFile file(key_file);                       // создаем объект класса QFile
    QByteArray data;                            // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (!file.open(QIODevice::ReadOnly))        // Проверяем, возможно ли открыть наш файл для чтения
        return;                                 // если это сделать невозможно, то завершаем функцию
    data = file.readLine();                     //считываем все данные с файла в объект data
    data = data.simplified();
    ui->lineEdit_2->setText(data);

    emit sendkey(data);
}

    //каталог нового файла
void MainWindow::on_pushButton_3_clicked()
{
    QString save_file = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),"output.txt", tr("TXT(*.txt)"));
    if (save_file.isEmpty() == false)
    {
        QFile file;                             // Создать файловый объект
        file.setFileName(save_file);            // Имя связанного файла
        if (file.open(QIODevice::WriteOnly))    // Открыть файл, только запись
        {
            QTextStream out(&file);             // поток записываемых данных направляем в файл
            out << ui->label_4->text() << endl; // считываем зашифрованный/шифрованный текст из label
        }
    }
    ui->label_3->setText(save_file);            // покажем куда сохранили наш output.txt
}

    //зашифровать
void MainWindow::on_pushButton_4_clicked()
{
    //connect(this, SIGNAL(sendSrc_filename(QString)),this, SLOT(on_pushButton_4_clicked(QString)));
    QString open_file = ui->label->text();
    QString src_filename = open_file;
    std::string utf8_src_filename = src_filename.toUtf8().constData(); //src_file - там содержится исходные данные

    QString rkey = ui->lineEdit_2->text();
    std::string std_recieve_key = rkey.toStdString();       //key

    QString riv = ui->lineEdit->text();
    std::string std_recieve_iv = riv.toUtf8().constData();    //iv

    QFile file(open_file);
    QByteArray data;                        // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
        return;                         // если это сделать невозможно, то завершаем функцию
    data = file.readLine();             //считываем все данные с файла в объект data
    data = data.simplified();                                   //message
    std::string std_recieve_message = data.toStdString();

    ByteBlock key = hex_to_bytes(std_recieve_key);
    ByteBlock iv = hex_to_bytes(std_recieve_iv);
    ByteBlock message = hex_to_bytes(std_recieve_message);
    ByteBlock output;

    CFB_Mode<Kuznyechik> encryptor(Kuznyechik(key), iv);

    encryptor.encrypt(message, output); //шифровка

    //ui->label_4->setText(hex_representation(output));
    ui->label_4->setText(QString::fromStdString(hex_representation(output))); //передаем сигнал в label

}

    // РАСШИФРОВКА
void MainWindow::on_pushButton_5_clicked()
{
    QString open_file = ui->label->text();
    QString src_filename = open_file;
    std::string utf8_src_filename = src_filename.toUtf8().constData(); //src_file

    QString rkey = ui->lineEdit_2->text();
    std::string std_recieve_kkey = rkey.toStdString(); //key


    QString riv = ui->lineEdit->text();
    std::string std_recieve_iv = riv.toUtf8().constData();  //iv

    QFile file(open_file);
    QByteArray data;                     // Создаем объект класса QByteArray, куда мы будем считывать данные
    if (!file.open(QIODevice::ReadOnly)) // Проверяем, возможно ли открыть наш файл для чтения
        return;                         // если это сделать невозможно, то завершаем функцию
    data = file.readLine();            //считываем все данные с файла в объект data
    data = data.simplified();
    std::string std_recieve_message = data.toStdString(); //message

    ByteBlock key = hex_to_bytes(std_recieve_kkey);
    ByteBlock iv = hex_to_bytes(std_recieve_iv);
    ByteBlock message = hex_to_bytes(std_recieve_message);
    ByteBlock output;

    CFB_Mode<Kuznyechik> encryptor(Kuznyechik(key), iv);

    encryptor.decrypt(message, output); //расшифровка

    ui->label_4->setText(QString::fromStdString(hex_representation(output))); //передаем сообщение в label
}


    // IV СИХНРОПОСЫЛКА
void MainWindow::on_pushButton_6_clicked()
{
    int n = 32;
    int array[n];
    //char str [6] = {'a','b','c','d','e','f'}; //попытка добавить буквы в IV
    QString arrayStr;
    for(int i = 0; i < n; ++i)
    {
        array[i] = rand() % 10;
        arrayStr += QString::number(array[i]);
    }

    emit sendrand(arrayStr); //посылаем сигнал на слот line edit
}

    // IV VALUE
void MainWindow::on_lineEdit_textChanged(QString recieve)
{
    ui->lineEdit->setText(recieve); //принимаем сигнал с синхропосылки
    ui->lineEdit->setMaxLength(32);
}

    // ENCRYPT/DECRYPT VALUE
void MainWindow::on_lineEdit_2_textChanged(QString recieve)
{
    ui->lineEdit_2->setText(recieve); //принимаем сигнал с ключа шифрования
    ui->lineEdit_2->setMaxLength(64);
}

