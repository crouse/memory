#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardPaths>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QColor>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setDatabase();
    ui->tableViewDict->hide();
    setMainToolBar();
    ui->lineEditEname->setFocus();

    QtAwesome *awesome = new QtAwesome(this);
    awesome->initFontAwesome();
    awesome->setDefaultOption( "color", QColor(122, 122, 122) );
    ui->pushButtonEsave->setIcon(awesome->icon(fa::database));
    ui->pushButtonEsave->setFont(awesome->font(13));

    ui->labelBirth->setText(QChar(fa::birthdaycake));
    ui->labelBirth->setFont(awesome->font(16));

    ui->labelName->setText(QChar(fa::smileo));
    ui->labelName->setFont(awesome->font(16));

    ui->labelQName->setText(QChar(fa::smileo));
    ui->labelQName->setFont(awesome->font(16));


    ui->labelPhone->setText(QString("%1|%2").arg(QChar(fa::phone)).arg(QChar(fa::mobilephone)));
    ui->labelPhone->setFont(awesome->font(16));

    ui->labelQPhone->setText(QString("%1|%2").arg(QChar(fa::phone)).arg(QChar(fa::mobilephone)));
    ui->labelQPhone->setFont(awesome->font(16));

    ui->labelDate->setText(QChar(fa::calendar));
    ui->labelDate->setFont(awesome->font(16));

    ui->labelGender->setText(QString("%1|%2").arg(QChar(fa::male)).arg(QChar(fa::female)));
    ui->labelGender->setFont(awesome->font(16));

    ui->labelQGender->setText(QString("%1|%2").arg(QChar(fa::male)).arg(QChar(fa::female)));
    ui->labelQGender->setFont(awesome->font(16));

    ui->actionDict->setText(QChar(fa::refresh));
    ui->actionDict->setFont(awesome->font(16));

    ui->actionList->setText(QChar(fa::barchart));
    ui->actionList->setFont(awesome->font(16));

    ui->actionExport->setText(QChar(fa::umbrella));
    ui->actionExport->setFont(awesome->font(16));

    ui->pushButtonQ->setText(QString("%1 %2").arg(QChar(fa::search)).arg("search"));
    ui->pushButtonQ->setFont(awesome->font(14));

    ui->dateEditEcurrent->setDate(QDate::currentDate());
    ui->dateEditEBirth->setDate(QDate(1900, 1, 1));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    QString dbpath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    database.setDatabaseName(QString("%1/sign.db").arg(dbpath));

    if (!database.open()) {
        QMessageBox::information(this, "", "无法连接数据库");
    } else {
        QSqlQuery query;
        QString signSql = QString("Create table sign ("
                                  " name varchar(32), "
                                  " gender varchar(4), "
                                  " phone varchar(32), "
                                  " birthday datetime, "
                                  " logdate datetime,"
                                  " current datetime,"
                                  " ps varchar(64)"
                                  ");");
        QString signDictSql = QString("Create table sign_dict ( "
                                      " name varchar(32), "
                                      " gender varchar(32),"
                                      " phone varchar(32),"
                                      " birthday datetime,"
                                      " cnt int"
                                      " );");

        QString notesSql = QString("Create table notes ("
                                   " name varchar(32), "
                                   " phone varchar(32), "
                                   " logtime datetime, "
                                   " note text)");

        query.exec(signSql);
        query.exec(signDictSql);
        query.exec(notesSql);
        database.commit();

        QStringList signTableFieldsStringList;
        QStringList signDictTableFieldsStringList;

        signTableFieldsStringList << "姓名" << "性别" << "手机" << "生日" << "日期" << "当前记录时间" << "附注";
        signDictTableFieldsStringList << "姓名" << "性别" << "手机" << "生日" << "签到次数";

        setModel(ui->tableViewSigns, "modelEdit", "sign", &signTableFieldsStringList, "", 0);
        setModel(ui->tableViewDict, "modelChoose", "sign_dict", &signDictTableFieldsStringList, "", 4);
        setModel(ui->tableViewQDict, "modelQ", "sign_dict", &signDictTableFieldsStringList, "", 4);
        setModel(ui->tableView, "modelQE", "sign", &signTableFieldsStringList, "", 0);
    }
}

void MainWindow::setModel(QTableView *tableViewName, QString modelName, QString tableName, QStringList *tableFields, QString filter, int orderNum)
{
    QSqlTableModel *model;
    if (modelName == "modelEdit") {
        modelEdit = new QSqlTableModel;
        model = modelEdit;
    } else if (modelName == "modelChoose") {
        modelChoose = new QSqlTableModel;
        model = modelChoose;
    } else if (modelName == "modelQ") {
        modelQ = new QSqlTableModel;
        model = modelQ;
    } else if (modelName == "modelQE") {
        modelQE = new QSqlTableModel;
        model = modelQE;
    } else {
        return;
    }

    model= new QSqlTableModel(this);
    model->setTable(tableName);
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    model->setSort(orderNum, Qt::DescendingOrder);

    int tableFieldsNum = tableFields->length();
    for (int i = 0; i < tableFieldsNum; i++) {
        model->setHeaderData(i, Qt::Horizontal, tableFields->at(i));
    }

    tableViewName->setModel(model);
    tableViewName->alternatingRowColors();
    tableViewName->horizontalHeader()->setStretchLastSection(true);

    model->setFilter(filter);
    model->select();
    tableViewName->reset();
    qDebug() << modelName << model << model->rowCount();

    if (modelName == "modelEdit") {
        modelEdit = model;
    } else if (modelName == "modelChoose") {
        modelChoose = model;
    } else {
        return;
    }
}

void MainWindow::setMainToolBar()
{
    ;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    qDebug() << index;
    switch(index) {
    case 0:
        ui->lineEditEname->setFocus();
        break;
    case 1:
        ui->lineEditQName->setFocus();
        break;
    default:
        break;
    }
}

void MainWindow::initDict()
{
    QSqlQuery query;
    query.exec("delete from sign_dict");
    QString sql = "insert into sign_dict (name, gender, phone, birthday, cnt) select name, gender, phone, birthday, count(*) as cnt from sign group by name, phone";
    query.exec(sql);
    database.commit();
}

void MainWindow::on_actionDict_triggered()
{
    initDict();
}

void MainWindow::on_actionList_triggered()
{
    if (ui->tableViewDict->isHidden()) {
        modelChoose->setFilter("");
        modelChoose->select();
        ui->tableViewDict->reset();
        ui->tableViewSigns->hide();
        ui->tableViewDict->show();
        ui->pushButtonEsave->setDisabled(true);
    } else {
        ui->tableViewDict->hide();
        ui->tableViewSigns->show();
        ui->pushButtonEsave->setEnabled(true);
    }
}

int MainWindow::updateDict(QString name, QString gender, QString phone, QString birthday)
{
    int num = 0;
    QSqlQuery query;
    query.exec(QString("select name from sign_dict where name = '%1' and gender = '%2' and phone = '%3'").arg(name).arg(gender).arg(phone));
    qDebug() << query.lastQuery();
    while(query.next())  { num+=1; }

    // 如果字典表里有记录，计数器加1
    if (num > 0) {
        query.exec(QString("update sign_dict set cnt = cnt + 1 where name = '%1' and gender = '%2' and phone = '%3'").arg(name).arg(gender).arg(phone));
        qDebug() << query.lastQuery();
        return num;
    }


    // 如果字典表里没有数据，添加到字典表
    query.exec(QString("insert into sign_dict (name, gender, phone, birthday, cnt) values ('%1', '%2', '%3', '%4', '%5')").arg(name).arg(gender).arg(phone).arg(birthday).arg(1));
    qDebug() << query.lastQuery();

    return 0;
}

bool MainWindow::ifSignedToday(QString name, QString gender, QString phone, QString logdate)
{
    int cnt = 0;
    QSqlQuery query;
    query.exec(QString("select * from sign where name = '%1' and gender = '%2' and phone = '%3' and logdate = '%4'").arg(name).arg(gender).arg(phone).arg(logdate));
    qDebug() << query.lastQuery();

    while(query.next()) {
        cnt+=1;
    }

    return cnt;
}

int MainWindow::choose()
{
    QString name = ui->lineEditEname->text();
    modelChoose->setFilter(QString("name = '%1'").arg(name));
    modelChoose->select();
    int rowCount = modelChoose->rowCount();
    ui->tableViewDict->reset();
    qDebug() << "rowCount" << rowCount;
    if (rowCount == 0) {
        return 0;
    } else if (rowCount == 1) {
        QString gender = modelChoose->record(0).value("gender").toString();
        QString phone = modelChoose->record(0).value("phone").toString();
        QString birthday = modelChoose->record(0).value("birthday").toString();
        ui->lineEditEphone->setText(phone);
        ui->comboBoxEgender->setCurrentText(gender);
        ui->dateEditEBirth->setDate(QDate::fromString(birthday, "yyyy-MM-dd"));
    } else if (rowCount > 1) {
        ui->tableViewDict->show();
        qDebug() << "dao";
    }

    return rowCount;
}

void MainWindow::on_pushButtonEsave_clicked()
{
    QString name = ui->lineEditEname->text().trimmed();
    QString gender = ui->comboBoxEgender->currentText().trimmed();
    QString phone = ui->lineEditEphone->text().trimmed();
    QString birthday = ui->dateEditEBirth->date().toString("yyyy-MM-dd");
    QString logdate = ui->dateEditEcurrent->dateTime().toString("yyyy-MM-dd HH:mm:ss");

    if (!ui->tableViewDict->isHidden()) {
        ui->tableViewDict->hide();
    }

    if (name.isEmpty()) {
        QMessageBox::information(this, "", "贤二，姓名不能为空滴啊！");
        return;
    }


    bool signStat = ifSignedToday(name, gender, phone, logdate);

    if (signStat) {
        QMessageBox::information(this, "", QString("嘿嘿，%1 %2 今天签到已经录进去啦，我们是一天签到一次哦！").arg(name).arg(phone));
        clearEdits();
        return;
    }

    updateDict(name, gender, phone, birthday);

    QString current = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    QSqlQuery query;
    query.exec(QString("insert into sign(name, gender, phone, birthday, logdate, current) values ('%1', '%2', '%3', '%4', '%5', '%6')").arg(name).arg(gender).arg(phone).arg(birthday).arg(logdate).arg(current));
    modelEdit->setFilter(QString("logdate = '%1'").arg(logdate));
    modelEdit->select();
    ui->tableViewSigns->reset();

    clearEdits();

}

void MainWindow::clearEdits()
{
    ui->lineEditEname->clear();
    ui->lineEditEphone->clear();
    ui->dateEditEBirth->setDate(QDate(1900, 1, 1));
    ui->lineEditEname->setFocus();
}

void MainWindow::on_tableViewDict_doubleClicked(const QModelIndex &index)
{
    int rowNum = index.row();
    QString phone = modelChoose->record(rowNum).value("phone").toString();
    QString gender = modelChoose->record(rowNum).value("gender").toString();
    QDate birthday = modelChoose->record(rowNum).value("birthday").toDate();
    ui->lineEditEphone->setText(phone);
    ui->comboBoxEgender->setCurrentText(gender);
    ui->dateEditEBirth->setDate(birthday);
}

void MainWindow::on_lineEditEname_editingFinished()
{
    choose();
}

void MainWindow::on_actionCurrentDateRows_triggered()
{
    modelEdit->setFilter(QString("logdate = '%1'").arg(ui->dateEditEcurrent->date().toString("yyyy-MM-dd 00:00:00")));
    modelEdit->select();
    ui->tableViewSigns->reset();
}

void MainWindow::on_actionExport_triggered()
{
    qDebug() << "export current backups";
}
