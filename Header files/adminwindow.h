#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class adminWindow;
}

class adminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminWindow(int admin_id, QWidget *parent = nullptr);
    ~adminWindow();

private slots:
    void on_tableView_activated(const QModelIndex &index);

    void on_update_button_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_delete_button_clicked();

private:
    Ui::adminWindow *ui;

    int id; // current person's ID from database

    QModelIndex auth;   // value of selected table cell
};

#endif // ADMINWINDOW_H
