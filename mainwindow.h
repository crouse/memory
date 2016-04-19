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
    void initFirstLetter();
    int updateDict(QString name, QString gender, QString phone, QString birthday);
    bool ifSignedToday(QString name, QString gender, QString phone, QString logdate);
    void clearEdits();
    int choose(int filterType);
    QSqlDatabase database;
    QSqlTableModel *modelEdit;
    QSqlTableModel *modelChoose;
    QSqlTableModel *modelQ;
    QSqlTableModel *modelQE;
    QSqlTableModel *modelNote;
    QSqlTableModel *modelNotes;
    QSqlTableModel *modelQuery;
    QLineEdit *searchLineEdit;

    QString sqlitePath;

private:
    int deleteRowNum;

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void on_tabWidget_tabBarClicked(int index);

    void on_actionDict_triggered();

    void on_actionList_triggered();

    void on_pushButtonEsave_clicked();

    void on_tableViewDict_doubleClicked(const QModelIndex &index);

    void on_lineEditEname_editingFinished();

    void on_actionCurrentDateRows_triggered();

    void on_actionExport_triggered();

    void on_pushButtonQ_clicked();

    void on_tableViewQDict_doubleClicked(const QModelIndex &index);

    void on_actionUpdatePerson_triggered();

    void on_pushButtonDataExport_clicked();

    void on_pushButtonDataImport_clicked();

    void on_dateEditNote_editingFinished();

    void on_dateEditNote_dateChanged(const QDate &date);

    void on_tableViewNoteHuman_doubleClicked(const QModelIndex &index);

    void on_pushButtonSaveNote_clicked();

    void on_toolButton_clicked();

    void on_actionFirstLetter_triggered();

    void on_tableViewDict_customContextMenuRequested(const QPoint &pos);

    void on_tableViewSigns_customContextMenuRequested(const QPoint &pos);

    void on_pushButtonExportCurrentNote_clicked();

    void on_actionDeleteRow_triggered();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
