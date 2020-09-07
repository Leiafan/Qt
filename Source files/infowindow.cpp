#include "infowindow.h"
#include "ui_infowindow.h"
#include "mainwindow.h"
#include "moderwindow.h"
#include "adminwindow.h"
#include "file_manager.h"
#include "docwindow.h"

infoWindow::infoWindow(QSqlDatabase db_, QString key, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::infoWindow)
{
    ui->setupUi(this);
    ui->settings_box->   hide();
    ui->cancel_button->  hide();
    ui->apply_ch_button->hide();

    ui->l_name->    setFixedSize(120, 20);
    ui->f_name->    setFixedSize(120, 20);
    ui->patronymic->setFixedSize(120, 20);
    ui->b_date->    setFixedSize(120, 20);
    ui->p_number->  setFixedSize(120, 20);
    ui->faculty->   setFixedSize(57, 20);
    ui->group->     setFixedSize(57, 20);
    ui->hostel_num->setFixedSize(57, 20);
    ui->room->      setFixedSize(57, 20);

    infoWindow::setFixedSize(1000, 650);
    ui->photo->setFixedSize(201,201);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    actual_key = key;
    db = db_;

    if (key != ""){
        QSqlQuery query;

         query.prepare("SELECT First_name, Last_name, Patronymic, Birth_date,"
                       " Phone_number, ID, Authority, Student_photo "
                       "FROM student_main_info "
                       "WHERE Email = :entry");
         query.bindValue(":entry", key);
         query.exec();

         query.first();
         ui->f_name->     setText(query.value(0).toString());
         ui->f_name_edit->setText(query.value(0).toString());
         ui->l_name->     setText(query.value(1).toString());
         ui->l_name_edit->setText(query.value(1).toString());
         ui->patronymic-> setText(query.value(2).toString());
         ui->p_edit    -> setText(query.value(2).toString());
         ui->b_date->     setText(query.value(3).toString());
         ui->b_d_edit->   setDate(query.value(3).toDate());
         ui->p_number->   setText(query.value(4).toString());
         ui->p_num_edit-> setText(query.value(4).toString());
         id = query.value(5).toInt();
         switch(query.value(6).toInt())
         {
            case 3:
             ui->moder_button-> setVisible(false);
             ui->admin_button-> setVisible(false);
             ui->backup_button->setVisible(false);
             ui->doc_button->   setVisible(false);
             break;
            case 2:
             ui->admin_button->setVisible(false);
             ui->doc_button->  setVisible(false);
             break;
            case 1:
             ui->moder_button->setVisible(false);
             break;
            case 4:
             ui->moder_button->setVisible(false);
             ui->admin_button->setVisible(false);
             ui->backup_button->setVisible(false);
         }
         QUrl url(query.value(7).toString());
         qDebug() << "URL is:" << query.value(7).toString();
         QNetworkRequest request(url);
         QNetworkAccessManager *m_netwManager = new QNetworkAccessManager(this);
         connect(m_netwManager, SIGNAL(finished(QNetworkReply*)),
                 this, SLOT(slot_netwManagerFinished(QNetworkReply*)));
         m_netwManager->get(request);

         if(query.lastError().isValid())
             qDebug() << query.lastError();
         query.clear();
         query.prepare("SELECT ID, Faculty, Group_number, Education_form "
                       "FROM student_fac_info "
                       "WHERE ID = :entry");
         query.bindValue(":entry", id);
         query.exec();

         query.first();
         ui->faculty-> setText(query.value(1).toString());
         ui->fac_edit->setText(query.value(1).toString());
         ui->group->   setText(query.value(2).toString());
         ui->gr_edit-> setText(query.value(2).toString());

         if(query.lastError().isValid())
             qDebug() << query.lastError();
         query.clear();
         query.prepare("SELECT Hostel_existence, Hostel_number "
                       "FROM hostel_existence "
                       "WHERE ID = :en");
         query.bindValue(":en", id);
         query.exec();
         if(query.lastError().isValid())
             qDebug() << query.lastError();
         query.first();
         if(query.value(0) == 0){
             ui->hostel_num->  setText("N/A");
             ui->room      ->  setText("N/A");
         }
         else{
             ui->hostel_num->     setText(query.value(1).toString());
             ui->hostel_num_edit->setText(query.value(1).toString());
             query.clear();
             query.prepare("SELECT Room_number "
                           "FROM hostel_info "
                           "WHERE ID = :en");
             query.bindValue(":en", id);
             query.exec();
             query.first();
             ui->room->         setText(query.value(0).toString());
             ui->room_num_edit->setText(query.value(0).toString());
             if(query.lastError().isValid())
                 qDebug() << query.lastError();

         }
         query.clear();
         query.prepare("SELECT Fluorography, Military_registration, Oncology_inspection, "
                       "Payment FROM logo_info WHERE ID = :en");
         query.bindValue(":en", id);
         query.exec();
         query.first();
         if(query.value(0).toInt() == 1) {
             QString con = "You recieved a prevention about the late delivery of fluorography";
             ui->listView->setPalette(Qt::red);
             ui->listView->addItem(con);
             ui->listView->item(0)->setBackgroundColor(Qt::yellow);
         }
         if(query.value(1).toInt() == 1) {
             QString con = "You recieved a warning about the late delivery of military inspection";
             ui->listView->setPalette(Qt::red);
             ui->listView->addItem(con);
             ui->listView->item(ui->listView->count()-1)->setBackgroundColor(Qt::red);
         }
         if(query.value(2).toInt() == 1) {
             QString con = "You recieved a prevention about the late delivery of oncology inspection";
             ui->listView->setPalette(Qt::red);
             ui->listView->addItem(con);
             ui->listView->item(ui->listView->count()-1)->setBackgroundColor(Qt::yellow);
         }
         if(query.value(3).toInt() == 1) {
             QString con = "You recieved a warning about the late payment";
             ui->listView->setPalette(Qt::red);
             ui->listView->addItem(con);
             ui->listView->item(ui->listView->count()-1)->setBackgroundColor(Qt::red);
         }
         else {
             QString proc = "There are no notifications";
             ui->listView->setPalette(Qt::gray);
             ui->listView->addItem(proc);
             ui->listView->item(0)->setBackgroundColor(Qt::green);
         }
    query.finish();
    }
}

infoWindow::~infoWindow()
{
    delete ui;
}    

void infoWindow::on_actionChangeAccount_triggered()
{
    this-> close();     //close window
    emit infoWdw();     //calling for the second window
}

void infoWindow::on_actionExit_triggered()
{
    this->close();
}

void infoWindow::receiveData(QString)
{
    qDebug() << "Receiving data in infowindow.cpp";
}

void infoWindow::on_actionChange_data_triggered()
{
    ui->settings_box->   setVisible(true);
    ui->apply_ch_button->setVisible(true);
    ui->cancel_button->  setVisible(true);
    ui->room_num_edit->  setVisible(false);
    ui->hostel_num_edit->setVisible(false);
    ui->label_room_n->   setVisible(false);
    ui->label_hostel_n-> setVisible(false);
}

void infoWindow::on_cancel_button_clicked()
{
    int button_c =  QMessageBox::question(this, "Close", "Cancel changes?",
                                          QMessageBox::Yes|QMessageBox::No);
    if (button_c == QMessageBox::Yes){
        ui->settings_box->   setVisible(false);
        ui->apply_ch_button->setVisible(false);
        ui->cancel_button->  setVisible(false);
    }
}

void infoWindow::on_apply_ch_button_clicked()
{
    if(ui->l_name_edit->text()      != "" and
            ui->f_name_edit->text() != "" and
            ui->p_edit->text()      != "" and
            ui->p_num_edit->text()  != "" and
            ui->gr_edit->text()     != "" and
            ui->fac_edit->text()    != "")
    {
        QSqlQuery query;
        query.prepare("UPDATE student_main_info "
                      "SET First_name = :fn, Last_name = :ln, Patronymic = :pat, "
                      "Phone_number = :p_num, Birth_date = :bd "
                      "WHERE Email = :entry");
        query.bindValue(":fn",    ui->l_name_edit->text());
        query.bindValue(":ln",    ui->f_name_edit->text());
        query.bindValue(":pat",   ui->p_edit->text());
        query.bindValue(":p_num", ui->p_num_edit->text());
        query.bindValue(":bd",    ui->b_d_edit->date());
        query.bindValue(":entry", actual_key);
        query.exec();
        if(query.lastError().isValid())
            qDebug() << query.lastError();
        query.clear();
        query.prepare("UPDATE student_fac_info "
                      "SET Faculty = :f, Group_number = :gn "
                      "WHERE ID = :entry");
        query.bindValue(":f",      ui->fac_edit->text());
        query.bindValue(":gn",     ui->gr_edit->text());
        query.bindValue(":entry", id);
        query.exec();
        if(query.lastError().isValid())
            qDebug() << query.lastError();
        query.clear();
        if (ui->hostel_yes->isChecked()){
            if(ui->hostel_num_edit->text() != "" and
                    ui->room_num_edit->text() != ""){
                query.prepare("UPDATE hostel_existence "
                              "SET Hostel_existence = 1, Hostel_number = :hn "
                              "WHERE ID = :entry");
                query.bindValue(":hn", ui->hostel_num_edit->text());
                query.bindValue(":entry", id);
                query.exec();
                if(query.lastError().isValid())
                    qDebug() << query.lastError();
                query.clear();
                query.prepare("UPDATE hostel_info "
                              "SET Hostel_number = :hn, Room_number = :rn "
                              "WHERE ID = :entry");
                query.bindValue(":hn", ui->hostel_num_edit->text());
                query.bindValue(":rn", ui->room_num_edit->  text());
                query.bindValue(":entry", id);
                query.exec();
                if(query.lastError().isValid())
                    qDebug() << query.lastError();
            }
            else{
                ui->statusbar->showMessage("An error has occured in hostel's data");
            }
        }
        else if (ui->hostel_no->isChecked()) {
           query.clear();
           query.prepare("UPDATE hostel_existence "
                         "SET Hostel_existence = 0, Hostel_number = NULL "
                         "WHERE ID = :entry");
           query.bindValue(":entry", id);
           query.exec();
           if(query.lastError().isValid())
               qDebug() << query.lastError();
           query.clear();
           query.prepare("UPDATE hostel_info "
                         "SET Hostel_number = NULL, Room_number = NULL "
                         "WHERE ID = :entry");
           query.bindValue(":entry", id);
           query.exec();
           if(query.lastError().isValid())
               qDebug() << query.lastError();
        }
        if(query.lastError().isValid())
            qDebug() << query.lastError();

        QTimer time;
        ui->statusbar->showMessage("You have successfully changed data!");
        time.start(2000);
        ui->settings_box->setVisible   (false);
        ui->cancel_button->setVisible  (false);
        ui->apply_ch_button->setVisible(false);
    }
    else{
        ui->statusbar->showMessage("You have an error in data. Try again later");
    }
}

void infoWindow::on_hostel_yes_clicked()
{
    ui->hostel_no->      setChecked(false);
    ui->room_num_edit->  setVisible(true);
    ui->hostel_num_edit->setVisible(true);
    ui->label_room_n->   setVisible(true);
    ui->label_hostel_n-> setVisible(true);
}

void infoWindow::on_hostel_no_clicked()
{
    ui->hostel_yes->     setChecked(false);
    ui->room_num_edit->  setVisible(false);
    ui->hostel_num_edit->setVisible(false);
    ui->label_room_n->   setVisible(false);
    ui->label_hostel_n-> setVisible(false);
    ui->room_num_edit->  setText("");
    ui->hostel_num_edit->setText("");
}

void infoWindow::on_search_line_edit_cursorPositionChanged(int arg1, int arg2)
{
    if(arg2==arg1) ui->search_line_edit->setText("");
}

void infoWindow::on_moder_button_clicked()
{
    moderWindow *mw = new moderWindow(id);
    mw->setWindowModality(Qt::ApplicationModal);
    mw->show();
}

void infoWindow::on_admin_button_clicked()
{
    adminWindow *aw = new adminWindow(id);
    aw->setWindowModality(Qt::ApplicationModal);
    aw->show();
}

void infoWindow::on_backup_button_clicked()
{
    file_manager *fMan = new file_manager;
    fMan->setWindowModality(Qt::ApplicationModal);
    fMan->setVisible(true);
}

void infoWindow::slot_netwManagerFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError){
        qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
        return;
    }
    QByteArray jpegData = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(jpegData);
    int w = std::min(pixmap.width(),  ui->photo->maximumWidth());
    int h = std::min(pixmap.height(), ui->photo->maximumHeight());
    pixmap = pixmap.scaled(QSize(w, h), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->photo->setPixmap(pixmap);
    ui->photo->setMinimumHeight(pixmap.height());
    qDebug() << jpegData;
}

void infoWindow::on_actionFluorography_triggered()
{

}

void infoWindow::on_doc_button_clicked()
{
    docWindow *fMan = new docWindow(db, id);
    fMan->setWindowModality(Qt::ApplicationModal);
    fMan->setVisible(true);
}
