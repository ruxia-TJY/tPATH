#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<stdlib.h>
#include<QDebug>
#include<QMessageBox>

extern char ** environ;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("tPATH");

    ui->tW_Env->setColumnCount(2);
    ui->tW_Env->setHeaderLabels(QStringList() << "Key" << "Value");
    ui->tW_Env->setSelectionBehavior(QAbstractItemView::SelectRows);

    lbl_info = new QLabel(this);
    ui->statusBar->addPermanentWidget(lbl_info);

    connect(ui->tW_Env,&QTreeWidget::currentItemChanged,
            this,&MainWindow::tW_Env_currentItemChanged);
    connect(ui->lE_search,&QLineEdit::textChanged,
            this,&MainWindow::onSearch);
    connect(ui->cB_SearchMode,&QComboBox::currentTextChanged,
            this,&MainWindow::onSearch);


    connect(ui->actionView_Search,&QAction::triggered,
            this,&MainWindow::viewsShow);
    connect(ui->actionViews_Key_Value,&QAction::triggered,
            this,&MainWindow::viewsShow);
    connect(ui->actionViews_Value,&QAction::triggered,
            this,&MainWindow::viewsShow);



    readEnvKeyList();

    viewsShow();
    if(ui->tW_Env->topLevelItemCount())
        ui->tW_Env->setCurrentItem(ui->tW_Env->topLevelItem(0));
    ui->lE_search->setFocus();
}

void MainWindow::viewsShow()
{
    ui->frame_search->setVisible(ui->actionView_Search->isChecked());
    if(ui->frame_search->isVisible())
        ui->lE_search->setFocus();
    ui->tW_Env->setVisible(ui->actionViews_Key_Value->isChecked());
    ui->lW_Value->setVisible(ui->actionViews_Value->isChecked());
}

void MainWindow::onSearch()
{
    int resultNums = 0;
    QString searchText = ui->lE_search->text();    
    QTreeWidgetItem* Item;
    QTreeWidgetItem* topItem;

    switch (ui->cB_SearchMode->currentIndex()) {
    case 0:
        // Mode Key and Value
        for (int i = ui->tW_Env->topLevelItemCount() - 1;i >= 0; i--)
        {
            Item = ui->tW_Env->topLevelItem(i);
            if(Item->text(0).contains(searchText,Qt::CaseInsensitive) || Item->text(1).contains(searchText,Qt::CaseInsensitive)){
                Item->setHidden(false);
                topItem=Item;
                resultNums += 1;
            }
            else
                Item->setHidden(true);
        }
        break;
    case 1:
        // Mode Only Key
        for (int i = ui->tW_Env->topLevelItemCount() - 1;i >= 0; i--)
        {
            Item = ui->tW_Env->topLevelItem(i);
            if(Item->text(0).contains(searchText,Qt::CaseInsensitive)){
                Item->setHidden(false);
                topItem=Item;
                resultNums += 1;
            }
            else
                Item->setHidden(true);
        }
        break;
    case 2:
        // Mode Only Value
        for (int i = ui->tW_Env->topLevelItemCount() - 1;i >= 0; i--)
        {
            Item = ui->tW_Env->topLevelItem(i);
            if(Item->text(1).contains(searchText,Qt::CaseInsensitive)){
                Item->setHidden(false);
                topItem=Item;
                resultNums += 1;
            }
            else
                Item->setHidden(true);
        }
        break;
    }

    if(resultNums)
        ui->tW_Env->setCurrentItem(topItem);

    if (searchText.length() == 0)
        ui->statusBar->showMessage("");
    else
        ui->statusBar->showMessage(QString("search %1").arg(resultNums));
}

void MainWindow::tW_Env_currentItemChanged(QTreeWidgetItem *currentItem,QTreeWidgetItem *pre)
{
    ui->lW_Value->clear();

    QStringList lst = currentItem->text(1).split(":");
    ui->lW_Value->addItems(lst);
    ui->lW_Value->setCurrentRow(0);

    lbl_info->setText(QString("<font color=blue>%1</font> Keys,<font color=red>%2</font> is <font color=blue>%3</font>").arg(
                          QString::number(ui->tW_Env->topLevelItemCount()),
                          ui->tW_Env->currentItem()->text(0),
                          QString::number(ui->lW_Value->count())
                          ));
}

void MainWindow::readEnvKeyList()
{
    ui->tW_Env->clear();
    char ** env_p = environ;

    while(*env_p){
        QStringList Key = QString(*env_p).split("=");
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << Key[0] << Key[1]);
        ui->tW_Env->addTopLevelItem(item);
        *env_p++;
    }
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       QString("tPATH"),
                       QString("Better view for Environment.\n\n-----\nVer.%1\nopensource - MIT\nAuthor: ruxia-TJY").arg(QCoreApplication::applicationVersion()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

