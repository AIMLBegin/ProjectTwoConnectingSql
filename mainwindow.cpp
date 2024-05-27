#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include "mainpage.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    //database connection
    DBConnection = QSqlDatabase::addDatabase("QSQLITE");

   //DBConnection.setDatabaseName(QCoreApplication::applicationDirPath() + "/home/vision/ex1.db");
     DBConnection.setDatabaseName("/home/vision/ex1.db");

    if(DBConnection.open())
    {
        qDebug()<<"database connected";
    }
    else {
        qDebug() <<"database not connected";
    }

    // Connect signals and slots
    connect(ui->pushButton_Login, &QPushButton::clicked, this, &MainWindow::on_pushButton_Login_clicked);
    connect(ui->pushButton_Cancel, &QPushButton::clicked, this, &MainWindow::on_pushButton_Cancel_clicked);
}
MainWindow::~MainWindow()
{
    delete ui;

}
void MainWindow::on_pushButton_Login_clicked()
{
    QString UserName = ui->lineEdit_UserName->text();
    QString Password = ui->lineEdit_Password->text();
    if (UserName == "testuser" && Password == "password123")
    {
        QMessageBox::information(this, "QT with me", "login success");
        this ->hide();
        MainPage *mainpage =new MainPage(this);
        mainpage->show();
    }
    else //to check when other users tries to login
    {
        QSqlQuery QueryGetUser(DBConnection);
        QueryGetUser.prepare("select *from user where UserName='"+UserName +"' AND password'"+ Password +"'");
        if(QueryGetUser.exec())
        {
           int UserFindCount = 0;
           while (QueryGetUser.next())
           {
               UserFindCount = UserFindCount + 1; //if credentials are correct

           }
           if(UserFindCount ==1)
           {
               QMessageBox::information(this, "QT with me", "login success");
               this ->hide();
               MainPage *mainpage =new MainPage(this);
               mainpage->show();
           }
           else if (UserFindCount ==0) //if credentials are not correct
           {
               QMessageBox::information(this, "QT with me", "plz check the credentials");
           }
        }
    }
    //else
    //{
   //     QMessageBox::warning(this, "QT with me", "login failed");
   // }
}
void MainWindow::on_pushButton_Cancel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "QT with me", "do you want to close the application?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}
