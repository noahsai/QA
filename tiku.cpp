#include "tiku.h"
#include "ui_tiku.h"

tiku::tiku(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tiku)
{
    ui->setupUi(this);
}

tiku::~tiku()
{
    delete ui;
}

void tiku::settiku(QStringList dan, QStringList duo , QStringList pan){
    QString text,text2,text3,tmp;
    ui->dantext->setWordWrap(true);
    ui->duotext->setWordWrap(true);
    ui->pantext->setWordWrap(true);
    QStringList list;
    for(int i=0;i<dan.length();i++)
    {
        tmp = dan.at(i);
        list = tmp.split("\t");
        if(list.length()<6) continue;
        text += QString("%1. %2\nA.%3\tB.%4\tC.%5\tD.%6\n答案：%7-----------------\n").arg(i+1,3,10,QLatin1Char('0')).arg(list.at(0)).arg(list.at(1)).arg(list.at(2)).arg(list.at(3)).arg(list.at(4)).arg(list.at(5));
    }
    ui->dantext->setText(text);

    for(int i=0;i<duo.length();i++)
    {
        tmp = duo.at(i);
        list = tmp.split("\t");
        if(list.length()<6) continue;
        text2 += QString("%1. %2\nA.%3\tB.%4\tC.%5\tD.%6\n答案：%7-----------------\n").arg(i+1,3,10,QLatin1Char('0')).arg(list.at(0)).arg(list.at(1)).arg(list.at(2)).arg(list.at(3)).arg(list.at(4)).arg(list.at(5));
    }
    ui->duotext->setText(text2);

    for(int i=0;i<pan.length();i++)
    {
        tmp = pan.at(i);
        list = tmp.split("\t");
        if(list.length()<4) continue;
        text3 += QString("%1. %2\n答案：%3\n-----------------\n").arg(i+1,3,10,QLatin1Char('0')).arg(list.at(0)).arg(list.at(3));
    }
    ui->pantext->setText(text3);
    //qDebug()<<text3;

}

void tiku::on_fontsize_valueChanged(int arg1)
{
    QFont font;
    font.setPointSize(arg1);
    ui->dantext->setFont(font);
    ui->duotext->setFont(font);
    ui->pantext->setFont(font);
}

void tiku::on_black_clicked(bool checked)
{
    if(checked){
        QString black = "QWidget { background-color: rgb(46, 52, 54);  color:rgb(165, 165, 165); }";
        this->setStyleSheet(black);
        //this->update();
    }
    else {
        this->setStyleSheet("");
        //this->update();
    }
}
