#include "widget.h"



Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    sqlMan db;
    fLoad=false;
    idLoaded=0;
    version=tr(VER_FILEVERSION_STR);
    ui->setupUi(this);
    dataManager* data=new dataManager();
    QString path=data->getPath()+"bal.ssff";
    ui->currency->setText(data->GetCurrency());
    QFile file_bal(path);
    QTextStream in_bal(&file_bal);
    in_bal.setCodec("UTF-8");
    ui->balance->setNum(db.getBalance());
    ui->date->setDateTime(QDateTime::currentDateTime());
    this->setWindowTitle(tr("Snipe Studio Budget Manager"));
    ui->date->setDateTime(QDateTime::currentDateTime());
    connect(ui->about,SIGNAL(clicked()),this,SLOT(help()));
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(addOperation()));
    connect(ui->settings,SIGNAL(clicked()),this,SLOT(showSettings()));
    if(initDatabase(db))
      close();
    ui->view->show();

}

Widget::~Widget()
{
  delete ui;
}

bool Widget::initDatabase(sqlMan db)
{
  QSqlTableModel* model=db.getModel();
  model->setTable("operations");
  if(db.dbIsOpen())
    {
      QMessageBox* dbOpenError=new QMessageBox(this);
      dbOpenError->warning(this, "Error in Db Loading", "There are some shit happens during database loading");
      dbOpenError->exec();
    }
  ui->view->setModel(model);
  ui->view->resizeColumnToContents(5);
  ui->view->setColumnWidth(3,310);
  ui->view->hideColumn(4);
  ui->view->hideColumn(0);
  ui->view->sortByColumn(0,Qt::DescendingOrder);
  ui->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void Widget::help()
{
    QMessageBox* helpMb=new QMessageBox(this);
    helpMb->about(this,tr("About SSBM"),tr("Snipe Studio Budget Manager v.%1\nUsing QT5\n2010-2016(ɔ)").arg(this->version));
    helpMb->close();
}

void Widget::addOperation()
{
    QString commentText=ui->comment->text();
    const QChar delimiter=(uchar)'.';
    bool summDigits=true;
    if(!ui->sum->text().isEmpty())
      {
      for(int i=0;i<ui->sum->text().length();i++)
      {
        if(!ui->sum->text().at(i).isDigit()&&!(ui->sum->text().at(i)=='.')&&!(ui->sum->text().at(i)==','))
          {
           summDigits=false;
           }
        if((ui->sum->text().at(i)=='.')||(ui->sum->text().at(i)==','))
          {
           ui->sum->setText(ui->sum->text().replace(i,1,delimiter));
           }
        }
      }
    if((commentText.isEmpty()&&ui->sum->text().isEmpty())||!summDigits)
      {
        QMessageBox* a=new QMessageBox(this);
        a->setText("Invalid summ value");
        a->show();
        return;
      }
    if(commentText.isEmpty())
        commentText=tr("Default");

    QString data=ui->date->text();
    if(!ui->sum->text().isEmpty())
    {
        bool side=false;
        double summ=ui->sum->text().toDouble();
        if(ui->profit->isChecked())
        {
           side=true;
        }
        else
        {
            side=false;
        }
        idLoaded++;
        if(!side)
        {
            summ*=-1;
        }
        QDateTime time=ui->date->dateTime();
        db.dbIsOpen();
        qDebug()<<db.getDBName();
        db.addOperation(&db,summ,commentText,side,time);
        QSqlTableModel* model=db.getModel();
        ui->view->setModel(model);
        ui->view->hideColumn(0);
        ui->view->resizeColumnToContents(0);
        ui->view->resizeColumnToContents(5);
        ui->view->setColumnWidth(3,200);
        ui->view->sortByColumn(0,Qt::DescendingOrder);
        ui->view->show();
        ui->balance->setNum(db.getBalance());

    }
}

void Widget::load()
{
  QTranslator translator;
  dataManager* data=new dataManager();
  qDebug()<<"translation/ssbm_"+data->getTranslation()+".qm";
  translator.load(QDir::toNativeSeparators("translation/ssbm_"+data->getTranslation()+".qm"));
  qApp->installTranslator(&translator);
  ui->retranslateUi(this);
  delete data;
}


void Widget::showSettings()
{
    set=new settings(this);
    set->show();
}

void Widget::closeSettings()
{
    delete set;
}

