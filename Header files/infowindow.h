#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSqlError>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QPixmap>
#include <QUrl>
#include <QNetworkReply>
#include <QTimer>
#include <QSlider>

namespace Ui {
class infoWindow;
}

class infoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit infoWindow(QSqlDatabase db, QString key = "", QWidget *parent = nullptr);
    ~infoWindow();

signals:
    void infoWdw();

    void sendData(QString); // sending data between mainwindow and infowindow

public slots:
    void receiveData(QString);   // sending data between mainwindow and infowindow

    void slot_netwManagerFinished(QNetworkReply *reply);    // upload user's URL from the net (does not work without IC)

private slots:
    void on_actionChangeAccount_triggered();

    void on_actionExit_triggered();

    void on_actionChange_data_triggered();

    void on_cancel_button_clicked();

    void on_apply_ch_button_clicked();

    void on_hostel_yes_clicked();

    void on_hostel_no_clicked();

    void on_search_line_edit_cursorPositionChanged(int arg1, int arg2);

    void on_moder_button_clicked();

    void on_admin_button_clicked();

    void on_backup_button_clicked();

    void on_actionFluorography_triggered();

    void on_doc_button_clicked();

private:
    Ui::infoWindow *ui;

    QSqlDatabase db;

    QString actual_key;         // user's email

    int id;   // holds users ID (got it from mainwindow by calling parametrized constructor infowindow)
};

#endif // INFOWINDOW_H
