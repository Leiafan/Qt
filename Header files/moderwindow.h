#ifndef MODERWINDOW_H
#define MODERWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class moderWindow;
}

class moderWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit moderWindow(int moder_id, QWidget *parent = nullptr);
    ~moderWindow();

private slots:
    void on_tableView_activated(const QModelIndex &index);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_update_button_clicked();

private:
    Ui::moderWindow *ui;

    int id;         // current users ID

    QModelIndex auth;   // selected raw in table
};

#endif // MODERWINDOW_H
