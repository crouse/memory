#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardPaths>
#include <QMessageBox>
#include <QSqlQuery>
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

    ui->labelPhone->setText(QString("%1|%2").arg(QChar(fa::phone)).arg(QChar(fa::mobilephone)));
    ui->labelPhone->setFont(awesome->font(16));

    ui->labelDate->setText(QChar(fa::calendar));
    ui->labelDate->setFont(awesome->font(16));

    ui->labelGender->setText(QString("%1|%2").arg(QChar(fa::male)).arg(QChar(fa::female)));
    ui->labelGender->setFont(awesome->font(16));

    ui->actionDict->setText(QChar(fa::refresh));
    ui->actionDict->setFont(awesome->font(16));

    ui->actionList->setText(QChar(fa::barchart));
    ui->actionList->setFont(awesome->font(16));
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
                                  " current datetime"
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

        signTableFieldsStringList << "姓名" << "性别" << "手机" << "生日" << "日期" << "当前记录时间";
        signDictTableFieldsStringList << "姓名" << "性别" << "手机" << "生日" << "签到次数";

        setModel(ui->tableViewSigns, modelEdit, "sign", &signTableFieldsStringList, "");
        setModel(ui->tableViewDict, modelChoose, "sign_dict", &signDictTableFieldsStringList, "");
    }
}

void MainWindow::setModel(QTableView *tableViewName, QSqlTableModel *modelName, QString tableName, QStringList *tableFields, QString filter)
{
    modelName = new QSqlTableModel(this);
    modelName->setTable(tableName);
    modelName->setEditStrategy(QSqlTableModel::OnFieldChange);
    modelName->setSort(0, Qt::AscendingOrder);

    int tableFieldsNum = tableFields->length();
    for (int i = 0; i < tableFieldsNum; i++) {
        modelName->setHeaderData(i, Qt::Horizontal, tableFields->at(i));
    }

    tableViewName->setModel(modelName);
    tableViewName->alternatingRowColors();
    tableViewName->horizontalHeader()->setStretchLastSection(true);

    modelName->setFilter(filter);
    modelName->select();
    tableViewName->reset();
    qDebug() << modelName;
}

void MainWindow::setMainToolBar()
{
    ;
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    qDebug() << index;
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
        initDict();
        ui->tableViewSigns->hide();
        ui->tableViewDict->show();
    } else {
        ui->tableViewDict->hide();
        ui->tableViewSigns->show();
    }
}

void MainWindow::on_pushButtonEsave_clicked()
{
    /*
    QString name = ui->lineEditEname->text().trimmed();
    QString gender = ui->comboBoxEgender->currentText().trimmed();
    QString phone = ui->lineEditEphone->text().trimmed();
    QString birthday = ui->dateEditEBirth->date().toString("yyyy-MM-dd");
    QString current = ui->dateEditEcurrent->dateTime().toString("yyyy-MM-dd HH:mm:ss");
    */
}
