#include "adminwindow.h"
#include "ui_adminwindow.h"

adminWindow::adminWindow(int admin_id, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::adminWindow)
{
    ui->setupUi(this);
    id = admin_id;
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT ID, First_name, Last_name, Patronymic, Phone_number, Birth_date, Email, Authority "
                   "FROM student_main_info");
    query->exec();
    model->setQuery(*query);
    ui->tableView->setModel(model);
    if(query->lastError().isValid())
        qDebug() << query->lastError();
    query->first();
    ui->id_edit->        setText(query->value(0).toString());
    ui->fn_edit->        setText(query->value(1).toString());
    ui->ln_edit->        setText(query->value(2).toString());
    ui->patronymic_edit->setText(query->value(3).toString());
    ui->phone_edit->     setText(query->value(4).toString());
    ui->date_edit->      setDate(query->value(5).toDate());
    ui->email_edit->     setText(query->value(6).toString());
    ui->authority_edit-> setText(query->value(7).toString());
    query->clear();
    query->exec("SELECT ID FROM student_main_info");
    ui->comboBox->setModel(model);
}

adminWindow::~adminWindow()
{
    delete ui;
}

void adminWindow::on_tableView_activated(const QModelIndex &index)
{
    QString value = ui->tableView->model()->data(index).toString();
    QSqlQuery *query = new QSqlQuery;
    auth = index;
    query->prepare("SELECT ID, First_name, Last_name, Patronymic, Phone_number, Birth_date, "
                   "Email, Authority "
                "FROM student_main_info "
                "WHERE ID = '"+value+"' OR First_name = '"+value+"' OR Last_name = '"+value+"' "
                "OR Patronymic = '"+value+"' "
                "OR Phone_number = '"+value+"' OR Birth_date = '"+value+"' OR Email = '"+value+"' "
                "OR Authority = '"+value+"'");
    query->exec();
    if(query->first()){
    ui->id_edit->        setText(query->value(0).toString());
    ui->fn_edit->        setText(query->value(1).toString());
    ui->ln_edit->        setText(query->value(2).toString());
    ui->patronymic_edit->setText(query->value(3).toString());
    ui->phone_edit->     setText(query->value(4).toString());
    ui->date_edit->      setDate(query->value(5).toDate());
    ui->email_edit->     setText(query->value(6).toString());
    ui->authority_edit-> setText(query->value(7).toString());
    }
}

void adminWindow::on_update_button_clicked()
{
    QSqlQuery query;
    QString value = ui->id_edit->text();
    query.prepare("UPDATE student_main_info "
                  "SET First_name = :fn, Last_name = :ln, Patronymic = :pat, "
                  "Phone_number = :p_num, Birth_date = :bd, Email = :em, Authority = :au "
                  "WHERE ID = '"+value+"'");
    query.bindValue(":fn",    ui->fn_edit->text());
    query.bindValue(":ln",    ui->ln_edit->text());
    query.bindValue(":pat",   ui->patronymic_edit->text());
    query.bindValue(":p_num", ui->phone_edit->text());
    query.bindValue(":bd",    ui->date_edit->date());
    query.bindValue(":em",    ui->email_edit->text());
    query.bindValue(":au",    ui->authority_edit->text());
    if(!query.exec())
        QMessageBox::critical(this, tr("Error"), query.lastError().text());
    else {
        QMessageBox::information(this, "Success", "You have successfully changed data");
    }
}

void adminWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM student_main_info WHERE ID = '"+arg1+"'");
    query.exec();
    query.first();
    ui->id_edit->        setText(query.value(0).toString());
    ui->fn_edit->        setText(query.value(1).toString());
    ui->ln_edit->        setText(query.value(2).toString());
    ui->patronymic_edit->setText(query.value(3).toString());
    ui->phone_edit->     setText(query.value(4).toString());
    ui->date_edit->      setDate(query.value(5).toDate());
    ui->email_edit->     setText(query.value(6).toString());
    ui->authority_edit-> setText(query.value(7).toString());
}

void adminWindow::on_delete_button_clicked()
{
    QSqlQuery query;
    QString value = ui->id_edit->text();
    query.prepare("DELETE FROM login_information WHERE ID = '"+value+"'");
    if(!query.exec())
        QMessageBox::critical(this, tr("Error"), query.lastError().text());
    else {
        QMessageBox::about(this, tr(""), tr("You have successfully deleted record"));
    }
}
