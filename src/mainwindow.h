#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"QTreeWidgetItem"
#include<QLabel>
#include<QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void readEnvKeyList(); // Read Environment Key List

public slots:
    void tW_Env_currentItemChanged(QTreeWidgetItem *currentItem,QTreeWidgetItem *pre);
    void onSearch();
    void viewsShow();
    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    QLabel* lbl_info;
};
#endif // MAINWINDOW_H
