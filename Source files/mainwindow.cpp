#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "infowindow.h"

    MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_mw(new Ui::MainWindow)
{
    ui_mw->setupUi(this);

    MainWindow::setFixedSize(700,520);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui_mw->regButton->setGeometry(1, 207, 317, 33);
    ui_mw->rgsWindow->setVisible(false);
    ui_mw->registrationButton_2->setVisible(false);
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("log_info");
    db.setUserName("root");
    db.setPassword("qwerty12");
    if(!db.open())
        qDebug() << db.lastError().text();
    else
        qDebug() << "Successful connection to database";
}

MainWindow::~MainWindow()
{
    delete ui_mw;
}

void MainWindow::on_pushButton_clicked()
{
    QString login    = ui_mw->login->   text();
    QString password = ui_mw->password->text();
    hash = new QCryptographicHash(QCryptographicHash::Sha512);
    if (password != "") hash->addData(password.toUtf8());
    QSqlQuery query;

    query.exec("SELECT Login, Password, Email FROM login_information");
    while (query.next()){
        QString log = query.value(0).toString();
        QString pw  = query.value(1).toString();
        email       = query.value(2).toString();
        if(login == log && pw == hash->result().toHex()){
            qDebug() << "Hash result:" << hash->result().toHex();
            qDebug() << "Password:   " << pw;
            info = new infoWindow(db, email);
            connect(this, SIGNAL(sendData(QString)), info, SLOT(receiveData(QString)));
            connect(info, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
            ui_mw->statusBar->showMessage("You have succesfully authorized");
            connect(info, &infoWindow::infoWdw, this, &MainWindow::show);
            qDebug() << "Email before send:" << email;
            emit sendData(QString(email));
            QTimer::singleShot(1500, this, SLOT(hide_onSuccessfulAuth()));
            QTimer::singleShot(1700, this, SLOT(showInfo()));                 //showing the second window
            QTimer::singleShot(1700, this, SLOT(setupUi()));
            return;
        }
        else{
            ui_mw->statusBar->showMessage("You have not authorized");
            ui_mw->password ->setText("");
        }
    }
}

void MainWindow::on_registrationButton_clicked()
{
    temp++;
    QPropertyAnimation *animation = new QPropertyAnimation(ui_mw->groupBox, "geometry");
    animation->setDuration(1000);
    animation->setStartValue(QRect(ui_mw->groupBox->x(),      ui_mw->groupBox->y(),
                                   ui_mw->groupBox->height(), ui_mw->groupBox->width()));
    animation->setEndValue  (QRect(ui_mw->groupBox->x(),      ui_mw->groupBox->y()-500,
                                   ui_mw->groupBox->height(), ui_mw->groupBox->width()));
    animation->start();
    if(temp != 1){
        QPropertyAnimation *animation2 = new QPropertyAnimation(ui_mw->rgsWindow, "geometry");
        animation2->setDuration(1000);
        animation2->setStartValue(QRect(ui_mw->rgsWindow->x(), ui_mw->rgsWindow->y(),
                                       ui_mw->rgsWindow->height(), ui_mw->rgsWindow->width()));
        animation2->setEndValue  (QRect(ui_mw->rgsWindow->x(), ui_mw->rgsWindow->y()-500,
                                       ui_mw->rgsWindow->height(), ui_mw->rgsWindow->width()));
        animation2->start();
    }
    QTimer::singleShot(1000, this, SLOT(show_regWindow()));
    ui_mw->registrationButton_2->setVisible(true);
}

void MainWindow::show_regWindow(){
    ui_mw->rgsWindow->setVisible(1);
    ui_mw->groupBox->setVisible(0);
}

void MainWindow::hide_regWindow(){
    ui_mw->rgsWindow->setVisible(0);
    ui_mw->groupBox->setVisible(1);
}

void MainWindow::hide_onSuccessfulAuth(){
    MainWindow::hide();
}

void MainWindow::showInfo(){
    info->show();
}

void MainWindow::setupUi(){
    ui_mw->login->   setText("");
    ui_mw->password->setText("");
    ui_mw->e_mail->  setText("");
    ui_mw->pw2_edit->setText("");
    ui_mw->pw_edit-> setText("");
    ui_mw->log_edit->setText("");
    ui_mw->statusBar->clearMessage();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    (arg1 != Qt::Unchecked) ?
                ui_mw->password->setEchoMode(QLineEdit::Normal) :
                ui_mw->password->setEchoMode(QLineEdit::Password);
}

void MainWindow::on_regButton_clicked()
{
    bool crct_log = false;
    email          = ui_mw->e_mail  ->text();
    QString login  = ui_mw->log_edit->text();
    QString pw1    = ui_mw->pw_edit ->text();
    QString pw2    = ui_mw->pw2_edit->text();
    if (
            email.length() > 5 && email.length() < 35 &&
            email.contains("@")&& email.contains(".") &&
            login.length() > 3 && login.length() < 16 &&
            pw1.length()   > 7 && pw1.length()   < 30 &&
            (pw1 == pw2))
        crct_log = true;
    if (crct_log)
    {
        hash = new QCryptographicHash(QCryptographicHash::Sha512);
        qDebug() << "Password =" << pw1.toUtf8();
        hash->addData(pw1.toUtf8());
        qDebug() << "Hashed   =" << hash->result().toHex();
        QSqlQuery query;
        query.prepare("INSERT INTO login_information (Login, Password, Email) "
                      "VALUES (?, ?, ?)");
        query.addBindValue(login);
        query.addBindValue(hash->result().toHex());
        query.addBindValue(email);
        query.exec();
        query.clear();
        query.prepare("INSERT INTO student_main_info (Email) "
                      "VALUE (:id)");
        query.bindValue(":id", email);
        query.exec();
        query.clear();
        query.prepare("INSERT INTO student_fac_info (Faculty) VALUE ('N/A')");
        query.exec();
        query.clear();
        query.prepare("INSERT INTO hostel_existence (Hostel_existence) VALUE (0)");
        query.exec();
        query.clear();
        query.prepare("INSERT INTO hostel_info (Hostel_number) VALUE (0)");
        query.exec();
        query.clear();
        query.prepare("INSERT INTO logo_contact (Commandant_number) VALUE ('+380')");
        query.exec();
        query.clear();
        query.prepare("INSERT INTO logo_info (Fluorography, Military_registration, Oncology_inspection,"
                      " Payment, Consolidation) VALUES (0, 0, 0, 0, 0)");
        query.exec();
        query.clear();
        query.prepare("INSERT INTO logo_events (Dedication) VALUE ('0')");
        query.exec();
        query.clear();
        ui_mw->statusBar->showMessage("You have successfully registered!");

        info = new infoWindow(db, email);
        connect(this, SIGNAL(sendData(QString)), info, SLOT(receiveData(QString)));
        connect(info, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
        ui_mw->statusBar->showMessage("You have succesfully authorized");
        connect(info, &infoWindow::infoWdw, this, &MainWindow::show);
        qDebug() << "Email before send:" << email;
        emit sendData(QString(email));
        QTimer::singleShot(1500, this, SLOT(hide_onSuccessfulAuth()));
        QTimer::singleShot(1700, this, SLOT(showInfo()));                 //showing the second window
        QTimer::singleShot(1700, this, SLOT(setupUi()));

    }
    else
        ui_mw->statusBar->showMessage("An error has occured");
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1 != Qt::Unchecked)
    {
        ui_mw->pw_edit ->setEchoMode(QLineEdit::Normal);
        ui_mw->pw2_edit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui_mw->pw_edit ->setEchoMode(QLineEdit::Password);
        ui_mw->pw2_edit->setEchoMode(QLineEdit::Password);
    }
}


void MainWindow::receiveData(QString)
{
    qDebug() << "Receiving data in mainwindow.cpp";
}

void MainWindow::on_registrationButton_2_clicked()
{

    QPropertyAnimation *animation = new QPropertyAnimation(ui_mw->rgsWindow, "geometry");
    animation->setDuration(1000);
    animation->setStartValue(ui_mw->rgsWindow->geometry());
    animation->setEndValue  (QRect(ui_mw->rgsWindow->x(),      ui_mw->rgsWindow->y()+500,
                                   ui_mw->rgsWindow->height(), ui_mw->rgsWindow->width()));
    animation->start();
    QPropertyAnimation *animation2 = new QPropertyAnimation(ui_mw->groupBox, "geometry");
    animation2->setDuration(1000);
    animation2->setStartValue(QRect(ui_mw->groupBox->x(),      ui_mw->groupBox->y(),
                                    ui_mw->groupBox->height(), ui_mw->groupBox->width()));
    animation2->setEndValue  (QRect(ui_mw->groupBox->x(),      ui_mw->groupBox->y()+500,
                                    ui_mw->groupBox->height(), ui_mw->groupBox->width()));
    animation2->start();

    QTimer::singleShot(1000, this, SLOT(hide_regWindow()));
    ui_mw->registrationButton_2->setVisible(false);
}
