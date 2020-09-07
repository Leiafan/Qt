#include "file_manager.h"
#include "ui_file_manager.h"

file_manager::file_manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::file_manager)
{
    ui->setupUi(this);
    model = new QFileSystemModel;
    model->setFilter(QDir::QDir::AllEntries);
    model->setRootPath("");
    ui->left_view-> setModel(model);
    ui->right_view->setModel(model);

    connect(ui->right_view, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(on_left_view_doubleClicked(QModelIndex)));
}

file_manager::~file_manager()
{
    delete ui;
}

void file_manager::on_left_view_doubleClicked(const QModelIndex &index)
{
    QListView *list_view = qobject_cast<QListView*>(sender());
    QFileInfo file_info = model->fileInfo(index);
    if (file_info.fileName() == "..") {
        QDir dir = file_info.dir();
        dir.cdUp();
        list_view->setRootIndex(model->index(dir.absolutePath()));
    }
    else if (file_info.fileName() == ".") {
        list_view->setRootIndex(model->index(""));
    }
    else if (file_info.isDir()) {
        list_view->setRootIndex(index);
    }
}

void data_difference(QDir &sDir, QDir &dDir, QFileInfoList &diffList){
    foreach (QFileInfo sInfo, sDir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot,
                                                 QDir::Name|QDir::DirsFirst)){
        bool fileExists = false;
        foreach(QFileInfo dInfo, dDir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot,
                                                    QDir::Name|QDir::DirsFirst)){
            if (sInfo.fileName() == dInfo.fileName()){
                if(sInfo.isDir() || sInfo.lastModified() <= dInfo.lastModified())
                    fileExists = true;
                break;
            }
        }
        if(!fileExists)
           diffList.append(sInfo);
        if(sInfo.isFile())
            continue;
        if(fileExists){
            sDir.cd(sInfo.fileName());
            dDir.cd(sInfo.fileName());
            data_difference(sDir, dDir, diffList);
            sDir.cdUp();
            dDir.cdUp();
        }
        else{
            sDir.cd(sInfo.fileName());
            recursive_data_difference(sDir, diffList);
            sDir.cdUp();
        }
    }
}

void recursive_data_difference(QDir &Dir, QFileInfoList &dataList){
    foreach(QFileInfo info, Dir.entryInfoList(QDir::Files|QDir::Dirs|QDir::NoDotAndDotDot,
                                              QDir::Name|QDir::DirsFirst)){
        dataList.append(info);
        if (info.isDir() && Dir.cd(info.fileName())){
            recursive_data_difference(Dir, dataList);
            Dir.cdUp();
        }
    }
}

void file_manager::on_backup_button_clicked()
{
    QDir sDir = QDir(model->filePath(ui->left_view->rootIndex()));
    QDir dDir = QDir(model->filePath(ui->right_view->rootIndex()));

    QFileInfoList diffList = QFileInfoList();
    data_difference(sDir, dDir, diffList);

    foreach(QFileInfo diffInfo, diffList){
        QString backupPath = diffInfo.filePath().replace(sDir.absolutePath(), dDir.absolutePath());
        if (diffInfo.isFile()){
            QFile::remove(backupPath);
            QFile::copy(diffInfo.absoluteFilePath(), backupPath);
        }
        if (diffInfo.isDir()){
            dDir.mkdir(backupPath);
        }
    }
}
