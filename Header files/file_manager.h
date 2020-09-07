#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <QWidget>
#include <QFileSystemModel>
#include <QDir>
#include <QDateTime>

namespace Ui {
class file_manager;
}

class file_manager : public QWidget
{
    Q_OBJECT

public:
    explicit file_manager(QWidget *parent = nullptr);
    ~file_manager();

private slots:
    void on_left_view_doubleClicked(const QModelIndex &index);

    void on_backup_button_clicked();

private:
    Ui::file_manager *ui;

    QFileSystemModel *model;    // value for moderation in file derictories etc
};


void data_difference(QDir &sDir, QDir &dDir, QFileInfoList &diffList);  // looking for different names and their LM dates

void recursive_data_difference(QDir &Dir, QFileInfoList &dataList);     // same as atop

#endif // FILE_MANAGER_H
