#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include "QtAwesome/QtAwesome.h"
#include "QtAwesome/QtAwesomeAnim.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setDatabase();
    void setModel(QTableView *tableViewName, QString modelName, QString tableName, QStringList *tableFields, QString filter, int orderNum);
    void setMainToolBar();
    void initDict();
    int updateDict(QString name, QString gender, QString phone, QString birthday);
    bool ifSignedToday(QString name, QString gender, QString phone, QString logdate);
    void clearEdits();
    int choose();
    QSqlDatabase database;
    QSqlTableModel *modelEdit;
    QSqlTableModel *modelChoose;
    QSqlTableModel *modelQuery;
    QLineEdit *searchLineEdit;

private slots:
    void on_tabWidget_tabBarClicked(int index);

    void on_actionDict_triggered();

    void on_actionList_triggered();

    void on_pushButtonEsave_clicked();

    void on_tableViewDict_doubleClicked(const QModelIndex &index);

    void on_lineEditEname_editingFinished();

    void on_actionCurrentDateRows_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
