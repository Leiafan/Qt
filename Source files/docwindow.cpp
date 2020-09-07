#include "docwindow.h"
#include "ui_docwindow.h"

docWindow::docWindow(QSqlDatabase db_, int doc_id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::docWindow)
{
    ui->setupUi(this);
    id = doc_id;
    db = db_;
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery *query = new QSqlQuery;

    query->prepare("SELECT s.ID, s.First_name, s.Last_name, s.Patronymic, "
                   "f.last_reference, s.Birth_date "
                   "FROM student_main_info AS s "
                   "INNER JOIN fluorography AS f "
                   "ON f.ID_ = s.ID");
    query->exec();
    model->setQuery(*query);

    ui->tableView->setModel(model);

    if(query->lastError().isValid())
        qDebug() << query->lastError();
    query->first();
    ui->fn_edit->        setText(query->value(1).toString());
    ui->ln_edit->        setText(query->value(2).toString());
    ui->patronymic_edit->setText(query->value(3).toString());
    ui->phone_edit->     setText(query->value(4).toString());
    ui->date_edit->      setDate(query->value(5).toDate());
    ui->id_edit->        setText(query->value(0).toString());
    query->clear();
    query->exec("SELECT ID from student_main_info");
    ui->comboBox->setModel(model);
}

docWindow::~docWindow()
{
    delete ui;
}

void docWindow::on_tableView_activated(const QModelIndex &index)
{
    QString value = ui->tableView->model()->data(index).toString();
    QSqlQuery *query = new QSqlQuery;
    auth = index;
    query->prepare("SELECT First_name, Last_name, Patronymic, Phone_number, Birth_date, ID "
                "FROM student_main_info "
                "WHERE First_name = '"+value+"' OR Last_name = '"+value+"' OR Patronymic = '"+value+"' "
                "OR Phone_number = '"+value+"' OR Birth_date = '"+value+"' OR ID = '"+value+"'");
    query->exec();
    query->first();
    ui->fn_edit->        setText(query->value(0).toString());
    ui->ln_edit->        setText(query->value(1).toString());
    ui->patronymic_edit->setText(query->value(2).toString());
    ui->phone_edit->     setText(query->value(3).toString());
    ui->date_edit->      setDate(query->value(4).toDate());
    ui->id_edit->        setText(query->value(5).toString());
}

void docWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query;
    query.prepare("SELECT First_name, Last_name, Patronymic, Phone_number, Birth_date, ID "
                  "FROM student_main_info WHERE ID = '"+arg1+"'");
    query.exec();
    query.first();
    ui->fn_edit->        setText(query.value(0).toString());
    ui->ln_edit->        setText(query.value(1).toString());
    ui->patronymic_edit->setText(query.value(2).toString());
    ui->phone_edit->     setText(query.value(3).toString());
    ui->date_edit->      setDate(query.value(4).toDate());
    ui->id_edit->        setText(query.value(5).toString());
}

void docWindow::on_update_button_clicked()  //sending prevention
{
    QSqlQuery query;
    query.prepare("UPDATE logo_info SET Fluorography = 1 WHERE ID = '"+ui->id_edit->text()+"'");
    if(!query.exec())
        qDebug() << query.lastError();
}
