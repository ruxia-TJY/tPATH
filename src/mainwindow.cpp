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
#ifdef Q_OS_WIN
    QTreeWidgetItem* winChildItem;
#endif
    QTreeWidgetItem* topItem;

    switch (ui->cB_SearchMode->currentIndex()) {
    case 0:
        // Mode Key and Value
        for (int i = ui->tW_Env->topLevelItemCount() - 1;i >= 0; i--)
        {
            Item = ui->tW_Env->topLevelItem(i);
#ifdef Q_OS_WIN
            for(int j = Item->childCount() - 1;j >= 0 ;j--){
                winChildItem = Item->child(j);
                if(winChildItem->text(0).contains(searchText,Qt::CaseInsensitive) || winChildItem->text(1).contains(searchText,Qt::CaseInsensitive)){
                    winChildItem->setHidden(false);
                    topItem=winChildItem;
                    resultNums += 1;
                }
                else
                    winChildItem->setHidden(true);
            }
#elif defined(Q_OS_LINUX)
            if(Item->text(0).contains(searchText,Qt::CaseInsensitive) || Item->text(1).contains(searchText,Qt::CaseInsensitive)){
                Item->setHidden(false);
                topItem=Item;
                resultNums += 1;
            }
            else
                Item->setHidden(true);
#endif
        }
        break;
    case 1:
        // Mode Only Key
        for (int i = ui->tW_Env->topLevelItemCount() - 1;i >= 0; i--)
        {
            Item = ui->tW_Env->topLevelItem(i);
#ifdef Q_OS_WIN
            for(int j = Item->childCount() - 1;j >= 0 ;j--){
                winChildItem = Item->child(j);
                if(winChildItem->text(0).contains(searchText,Qt::CaseInsensitive)){
                    winChildItem->setHidden(false);
                    topItem=winChildItem;
                    resultNums += 1;
                }
                else
                    winChildItem->setHidden(true);
            }
#elif defined(Q_OS_LINUX)
            if(Item->text(0).contains(searchText,Qt::CaseInsensitive)){
                Item->setHidden(false);
                topItem=Item;
                resultNums += 1;
            }
            else
                Item->setHidden(true);
#endif
        }
        break;
    case 2:
        // Mode Only Value
        for (int i = ui->tW_Env->topLevelItemCount() - 1;i >= 0; i--)
        {
            Item = ui->tW_Env->topLevelItem(i);
#ifdef Q_OS_WIN
            for(int j = Item->childCount() - 1;j >= 0 ;j--){
                winChildItem = Item->child(j);
                if(winChildItem->text(1).contains(searchText,Qt::CaseInsensitive)){
                    winChildItem->setHidden(false);
                    topItem=winChildItem;
                    resultNums += 1;
                }
                else
                    winChildItem->setHidden(true);
            }
#elif defined(Q_OS_LINUX)
            if(Item->text(1).contains(searchText,Qt::CaseInsensitive)){
                Item->setHidden(false);
                topItem=Item;
                resultNums += 1;
            }
            else
                Item->setHidden(true);
#endif
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
#ifdef Q_OS_WIN
    QStringList lst = currentItem->text(1).split(";");
#elif defined(Q_OS_LINUX)
    QStringList lst = currentItem->text(1).split(":");
#endif
    lst.removeAll("");
    ui->lW_Value->addItems(lst);
    ui->lW_Value->setCurrentRow(0);
#ifdef Q_OS_WIN
    QString cur_count;
    if(ui->tW_Env->currentItem()->parent() != nullptr){
        cur_count = QString::number(ui->lW_Value->count());
    }
    else{
        cur_count = QString::number(ui->tW_Env->currentItem()->childCount());
    }

    lbl_info->setText(QString("User <font color=blue>%1</font> Keys,System <font color=blue>%2</font> Keys,<font color=red>%3</font> has <font color=blue>%4</font>").arg(
        QString::number(ui->tW_Env->topLevelItem(0)->childCount()),
        QString::number(ui->tW_Env->topLevelItem(1)->childCount()),
        ui->tW_Env->currentItem()->text(0),
        cur_count
        ));
#elif defined(Q_OS_LINUX)
    lbl_info->setText(QString("<font color=blue>%1</font> Keys,<font color=red>%2</font> has <font color=blue>%3</font>").arg(
                          QString::number(ui->tW_Env->topLevelItemCount()),
                          ui->tW_Env->currentItem()->text(0),
                          QString::number(ui->lW_Value->count())
                          ));
#endif
}

void MainWindow::readEnvKeyList()
{
    ui->tW_Env->clear();

#ifdef Q_OS_WIN
    QSettings usersettings("HKEY_CURRENT_USER\\Environment", QSettings::NativeFormat);
    QStringList userkeys = usersettings.allKeys();

    QTreeWidgetItem *useritem = new QTreeWidgetItem(QStringList() << QString("User Environment") << QString(""));
    ui->tW_Env->addTopLevelItem(useritem);

    for (const QString &key : userkeys) {
        QTreeWidgetItem *item = new QTreeWidgetItem(useritem,QStringList() << key << usersettings.value(key).toString());
    }

    QSettings systemsettings("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", QSettings::NativeFormat);
    QStringList keys = systemsettings.allKeys();

    QTreeWidgetItem *systemitem = new QTreeWidgetItem(QStringList() << QString("System Environment") << QString(""));
    ui->tW_Env->addTopLevelItem(systemitem);

    for (const QString &key : keys) {
        QTreeWidgetItem *item = new QTreeWidgetItem(systemitem,QStringList() << key << systemsettings.value(key).toString());
    }

    ui->tW_Env->expandAll();
#elif defined(Q_OS_LINUX)
    char ** env_p = environ;

    while(*env_p){
        QStringList Key = QString(*env_p).split("=");
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << Key[0] << Key[1]);
        ui->tW_Env->addTopLevelItem(item);
        *env_p++;
    }

#endif
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       QString("tPATH"),
                       QString("Better view for Environment.\n\n-----\nVersion.%1\nAuthor: ruxia-TJY\nLicense - MIT\nOpenSource - https://github.com/ruxia-TJY/tPATH\n\nThanks for use!").arg(QCoreApplication::applicationVersion()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

