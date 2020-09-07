#ifndef DOCWINDOW_H
#define DOCWINDOW_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QIdentityProxyModel>

namespace Ui {
class docWindow;
}

class docWindow : public QWidget
{
    Q_OBJECT

public:
    explicit docWindow(QSqlDatabase db, int doc_id, QWidget *parent = nullptr);
    ~docWindow();

private slots:
    void on_tableView_activated(const QModelIndex &index);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_update_button_clicked();

private:
    Ui::docWindow *ui;

    QSqlDatabase db;

    int id;         // current users ID

    QModelIndex auth;   // selected raw in table
};

class MyModel : public QSqlTableModel
{
    Q_OBJECT
public:
    MyModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase())
        : QSqlTableModel(parent, db) {;}
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const
    {
        if (role == Qt::BackgroundColorRole)
        {
            const QVariant value(data(index, Qt::DisplayRole));
            return QVariant(QColor(value.toString() == "2009-01-01"?Qt::green:Qt::red));
        }
        return QSqlTableModel::data(index, role);
    }
};

#endif // DOCWINDOW_H
