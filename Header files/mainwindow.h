#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "infowindow.h"
#include <QMainWindow>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QDebug>
#include <QtSql/QSqlRecord>
#include <QApplication>
#include <QStyleFactory>
#include <QtSql/QSqlError>
#include <QHostInfo>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QSplashScreen>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class infoWindow;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendData(QString var);     // send data between infowindow

public slots:
    void receiveData(QString vars); // send data between infowindow

private slots:
    void on_pushButton_clicked();

    void on_registrationButton_clicked();

    void show_regWindow();

    void hide_regWindow();

    void hide_onSuccessfulAuth();

    void showInfo();

    void setupUi();

    void on_checkBox_stateChanged(int argv);

    void on_regButton_clicked();

    void on_checkBox_2_stateChanged(int arg1);

    void on_registrationButton_2_clicked();

private:
    Ui::MainWindow *ui_mw;

    QSqlDatabase db;            // actual database holder

    QSqlTableModel *model;      // table which reproduces databases cells

    QString email;              // user's email (general)

    QCryptographicHash* hash;   // hashed password

    infoWindow* info;           // infowindow object

    int temp = 0;
};

#endif // MAINWINDOW_H
