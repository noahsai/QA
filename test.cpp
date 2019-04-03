#include "test.h"
#include "ui_test.h"

test::test( QStringList* dan ,  int dannum ,  QStringList* duo , int duonum ,  QStringList* pan , int pannum , int min, QWidget *parent ) :
    QWidget(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);
    setWindowTitle("正在做题");
    setAutoFillBackground(true);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->A , SIGNAL(clicked(bool)),SLOT(saveanswer()));
    connect(ui->B , SIGNAL(clicked(bool)),SLOT(saveanswer()));
    connect(ui->C , SIGNAL(clicked(bool)),SLOT(saveanswer()));
    connect(ui->D , SIGNAL(clicked(bool)),SLOT(saveanswer()));
    connect(ui->AA , SIGNAL(clicked(bool)),SLOT(saveanswer()));
    connect(ui->BB , SIGNAL(clicked(bool)),SLOT(saveanswer()));
    connect(ui->CC , SIGNAL(clicked(bool)),SLOT(saveanswer()));
    connect(ui->DD , SIGNAL(clicked(bool)),SLOT(saveanswer()));
    connect(ui->A_2 , SIGNAL(clicked(bool)),SLOT(saveanswer2()));
    connect(ui->B_2 , SIGNAL(clicked(bool)),SLOT(saveanswer2()));
    connect(ui->C_2 , SIGNAL(clicked(bool)),SLOT(saveanswer2()));
    connect(ui->D_2 , SIGNAL(clicked(bool)),SLOT(saveanswer2()));
    connect(ui->AA_2 , SIGNAL(clicked(bool)),SLOT(saveanswer2()));
    connect(ui->BB_2 , SIGNAL(clicked(bool)),SLOT(saveanswer2()));
    connect(ui->CC_2 , SIGNAL(clicked(bool)),SLOT(saveanswer2()));
    connect(ui->DD_2 , SIGNAL(clicked(bool)),SLOT(saveanswer2()));

    deadline = min*60;
    this->dan = dan;
    this->duo = duo;
    this->pan = pan;
    if(dannum <= dan->length())    this->dannum = dannum;
    else this->dannum = dan->length();
    if(duonum <= duo->length())    this->duonum = duonum;
    else this->duonum = duo->length();
    if(pannum <= pan->length())    this->pannum = pannum;
    else this->pannum = pan->length();

    dancuoall = NULL;
    duocuoall = NULL;
    pancuoall = NULL;

    did = 0;
    now = 0;
    all = this->dannum + this->duonum + this->pannum;

    ui->xuanti->setMaximum(all);
    ui->xuanti->setMinimum(1);
    ui->xuanti->setValue(now+1);
    ui->yizuo->setText(QString("已做%1题/共%2题").arg(0).arg(all));
    //qDebug()<<"all"<<all;
    ui->ti1->setFocus();
    ui->pre->setEnabled(true);
}

test::test( QWidget *parent ) :
    QWidget(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);
    setAutoFillBackground(true);

}

test::~test()
{
    delete ui;
}

void test:: rand(){

    QTime time;
    int rand;
    danrand.clear();
    duorand.clear();
    panrand.clear();
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    //int qr =qrand();
    if(!dancuoall) //证明不是错题重做
    {
        for(int i = 0;i<dannum ;i++)
        {
            rand = qrand() % dan->length();    //产生随机数
            if(danrand.indexOf(rand)==-1)   danrand.append(rand);
            else i--;

        }
    //qDebug()<< danrand;
        for(int i = 0;i<duonum ;i++)
        {
            rand = qrand() % duo->length();    //产生随机数
            if(duorand.indexOf(rand)==-1)   duorand.append(rand);
            else i--;
        }

        for(int i = 0;i<pannum ;i++)
        {
            rand = qrand() % pan->length();    //产生随机数
            if(panrand.indexOf(rand)==-1)   panrand.append(rand);
            else i--;
        }
    }
    else {
        for(int i = 0;i<dannum ;i++)
        {
            rand = qrand() % dancuoall->length();    //产生随机数
            if(danrand.indexOf(rand)==-1)   danrand.append(rand);
            else i--;

        }
    //qDebug()<< danrand;
        for(int i = 0;i<duonum ;i++)
        {
            rand = qrand() % duocuoall->length();    //产生随机数
            if(duorand.indexOf(rand)==-1)   duorand.append(rand);
            else i--;
        }

        for(int i = 0;i<pannum ;i++)
        {
            rand = qrand() % pancuoall->length();    //产生随机数
            if(panrand.indexOf(rand)==-1)   panrand.append(rand);
            else i--;
        }
//        for(int i=0;i<danrand.length();i++)
//        {
//           qDebug()<<dancuoall->at(danrand.at(i));
//        }
//        for(int i=0;i<duorand.length();i++)
//        {
//           qDebug()<<duocuoall->at(duorand.at(i));
//        }
//        for(int i=0;i<panrand.length();i++)
//        {
//           qDebug()<<pancuoall->at(panrand.at(i));
//        }
    }

}

void test:: refresh(int tinum , bool first ){
//    qDebug()<<"ti";

    ui->xuanti->setValue(tinum);
    ui->ti2->show();

    int n = tinum;
    int type = 0;
     QStringList *list = NULL;
     QList<int> * cuolist = NULL;
     QList<int> *rand;
    if((tinum >= (all))){
        if(!first) {
            ui->ti2->hide();
        }
        return;
    }
    else if(tinum >= (dannum +duonum)){
        n = tinum - dannum - duonum;
        list = pan;
        rand = &panrand;
        cuolist = pancuoall;
        type=2;
    }
    else if(tinum >= dannum){
        n = tinum - dannum;
        list = duo;
        rand = &duorand;
        cuolist = duocuoall;
        type = 1;
    }
    else {
        list = dan;
        rand = &danrand;
        cuolist = dancuoall;
        type = 0;
    }
   // qDebug()<<"type"<<type  ;
    QString ti;
    if(!list) return;//list为NULL
    if(!dancuoall) ti = list->at(rand->at(n));
    else ti = list->at(cuolist->at(rand->at(n)));

    ti = ti.remove(QRegularExpression("\n$"));
   // qDebug()<<"ti:"<<ti;

    QStringList l = ti.split("\t");
    if(l.length()< 5 && type == 2) return;
    else if(l.length()<6 && type < 2) return;

    QString choose;
    choose = youranswer.at(tinum);
    QString a;
    if(first){
        ui->ti1->setTitle("第"+QString().setNum(tinum+1)+"题");
        ui->test1->setText(l.at(0));
        if(type < 2) {
            a = l.at(5);
            if(a.indexOf("#") != -1 ) a = "#" +a;//与youranswer保持一致
            settype1(type , l.at(1),l.at(2),l.at(3),l.at(4),choose);
        }
        else {
            settype1(type , l.at(1),l.at(2),"","" ,choose);
            a = l.at(4);
        }
        a = a.remove("\n");//别去除#号，需要用到
        answer.replace(tinum , a);
        refresh(tinum+1 , false);
    }
    else {
        ui->ti2->setTitle("第"+QString().setNum(tinum+1)+"题");
        ui->test2->setText(l.at(0));
        if(type < 2) {
            a = l.at(5);
            if(a.indexOf("#") != -1 ) a = "#" +a;//与youranswer保持一致
            settype2(type , l.at(1),l.at(2),l.at(3),l.at(4) ,choose);
        }
        else {
            settype2(type , l.at(1),l.at(2),"","" ,choose);
            a = l.at(4);
        }
        a = a.remove("\n");//别去除#号，需要用到
        answer.replace(tinum , a);
    }
}

void test:: settype1( int t ,QString a, QString b , QString c , QString d ,QString choose ){
    ui->A->setAutoExclusive(false);
    ui->B->setAutoExclusive(false);
    ui->C->setAutoExclusive(false);
    ui->D->setAutoExclusive(false);

    ui->A->setChecked(false);
    ui->B->setChecked(false);
    ui->C->setChecked(false);
    ui->D->setChecked(false);
    ui->AA->setChecked(false);
    ui->BB->setChecked(false);
    ui->CC->setChecked(false);
    ui->DD->setChecked(false);
    switch (t) {
    case 0:
        ui->A->setText(a);
        ui->B->setText(b);
        ui->C->setText(c);
        ui->D->setText(d);
        ui->A->show();
        ui->B->show();
        ui->C->show();
        ui->D->show();
        ui->AA->hide();
        ui->BB->hide();
        ui->CC->hide();
        ui->DD->hide();
        break;
    case 1:
        ui->AA->setText(a);
        ui->BB->setText(b);
        ui->CC->setText(c);
        ui->DD->setText(d);
        ui->A->hide();
        ui->B->hide();
        ui->C->hide();
        ui->D->hide();
        ui->AA->show();
        ui->BB->show();
        ui->CC->show();
        ui->DD->show();

        break;
    case 2:
        ui->A->setText(a);
        ui->B->setText(b);
        ui->A->show();
        ui->B->show();
        ui->C->hide();
        ui->D->hide();
        ui->AA->hide();
        ui->BB->hide();
        ui->CC->hide();
        ui->DD->hide();
        break;
    }
    ui->A->setAutoExclusive(true);
    ui->B->setAutoExclusive(true);
    ui->C->setAutoExclusive(true);
    ui->D->setAutoExclusive(true);

    QStringList list = choose.split("#");
    if(list.length()==1){
        if (list.at(0) == "A") {
            ui->A->setChecked(true);
        }
        else if (list.at(0) == "B"){
            ui->B->setChecked(true);
        }
        else if (list.at(0) == "C") {
            ui->C->setChecked(true);
        }
        else if (list.at(0) == "D") {
            ui->D->setChecked(true);
        }
    }
    else {
        for(int i=0; i<list.length();i++)
        {
            if (list.at(i) == "A") {
                ui->AA->setChecked(true);
            }
            if (list.at(i) == "B"){
                ui->BB->setChecked(true);
            }
            if (list.at(i) == "C") {
                ui->CC->setChecked(true);
            }
            if (list.at(i) == "D") {
                ui->DD->setChecked(true);
            }
        }
    }

}

void test:: settype2( int t ,QString a, QString b , QString c , QString d ,QString choose ){
    ui->A_2->setAutoExclusive(false);
    ui->B_2->setAutoExclusive(false);
    ui->C_2->setAutoExclusive(false);
    ui->D_2->setAutoExclusive(false);

    ui->A_2->setChecked(false);
    ui->B_2->setChecked(false);
    ui->C_2->setChecked(false);
    ui->D_2->setChecked(false);
    ui->AA_2->setChecked(false);
    ui->BB_2->setChecked(false);
    ui->CC_2->setChecked(false);
    ui->DD_2->setChecked(false);

    switch (t) {
    case 0:
        ui->A_2->setText(a);
        ui->B_2->setText(b);
        ui->C_2->setText(c);
        ui->D_2->setText(d);
        ui->A_2->show();
        ui->B_2->show();
        ui->C_2->show();
        ui->D_2->show();
        ui->AA_2->hide();
        ui->BB_2->hide();
        ui->CC_2->hide();
        ui->DD_2->hide();
        break;
    case 1:
        ui->AA_2->setText(a);
        ui->BB_2->setText(b);
        ui->CC_2->setText(c);
        ui->DD_2->setText(d);
        ui->A_2->hide();
        ui->B_2->hide();
        ui->C_2->hide();
        ui->D_2->hide();
        ui->AA_2->show();
        ui->BB_2->show();
        ui->CC_2->show();
        ui->DD_2->show();
        break;
    case 2:
        ui->A_2->setText(a);
        ui->B_2->setText(b);
        ui->A_2->show();
        ui->B_2->show();
        ui->C_2->hide();
        ui->D_2->hide();
        ui->AA_2->hide();
        ui->BB_2->hide();
        ui->CC_2->hide();
        ui->DD_2->hide();
        break;
    }
    ui->A_2->setAutoExclusive(true);
    ui->B_2->setAutoExclusive(true);
    ui->C_2->setAutoExclusive(true);
    ui->D_2->setAutoExclusive(true);

    QStringList list = choose.split("#");
    if(list.length()==1){
        if (list.at(0) == "A") {
            ui->A_2->setChecked(true);
        }
        else if (list.at(0) == "B"){
            ui->B_2->setChecked(true);
        }
        else if (list.at(0) == "C") {
            ui->C_2->setChecked(true);
        }
        else if (list.at(0) == "D") {
            ui->D_2->setChecked(true);
        }
    }
    else {
        for(int i=0; i<list.length();i++)
        {
            if (list.at(i) == "A") {
                ui->AA_2->setChecked(true);
            }
            if (list.at(i) == "B"){
                ui->BB_2->setChecked(true);
            }
            if (list.at(i) == "C") {
                ui->CC_2->setChecked(true);
            }
            if (list.at(i) == "D") {
                ui->DD_2->setChecked(true);
            }
        }
    }
}

void test:: initanswer(){
    answer.clear();
    youranswer.clear();
    for (int i=0;i<all;i++)
    {
        answer.append("");
        youranswer.append("");
    }
}

void test::on_xuanti_editingFinished()
{
    //qDebug()<<"on_xuanti_editingFinished()";
    now = ui->xuanti->value()-1;
    if(now == 0){
        ui->pre->setEnabled(false);
        if(all > 1 ) ui->next->setEnabled(true);
    }
    else if(now >= all -2 ) {
        ui->next->setEnabled(false);
        if(all > 1 ) ui->pre->setEnabled(true);

    }
    refresh( now );

}

void test::on_pre_clicked()
{

    now -=2 ;
    if(now <=0 ) {
        now = 0 ;
        ui->pre->setEnabled(false);
    }
    if(all > 1 ) ui->next->setEnabled(true);
    //qDebug()<<"pre_clicked()"<<now;
    refresh(now);
}

void test::on_next_clicked()
{
    now +=2 ;
    if(now >= all -2) {
        ui->next->setEnabled(false);
        if(now >= all-1) now = all - 1;
    }

    if(all > 1 ) ui->pre->setEnabled(true);
    //qDebug()<<"next_clicked()"<<now;
    refresh(now);
}

void test::on_submit_clicked()
{
    int n ;
    float sum = 0;
    QString undo;
    int ud = 0;
    QStringList l;
    qDebug()<<youranswer<<answer;
    QString ya;
    for(int i = 0 ;i<youranswer.length();i++)
    {
        ya = youranswer.at(i);
        if(!ya.isEmpty() &&  ya == answer.at(i))
        {
                sum += (100.0/youranswer.length()) ;
        }
        else if(ya.isEmpty()){
            ud ++;
            if(undo.isEmpty()) undo += QString().setNum(i+1);
            else undo += "、"+QString().setNum(i+1);
        } 

        else{
            n = i;
            if(dancuoall){
                //错题重做
                if(i >= (dannum +duonum)){
                    //判断提
                    n = i - dannum - duonum;
                    if(pancuo.indexOf(pancuoall->at(panrand[n]))==-1) pancuo.append(pancuoall->at(panrand[n]));
                }
                else if(i >= dannum){
                    n = i - dannum ;
                    //多选
                    if(duocuo.indexOf(duocuoall->at(duorand[n]))==-1) duocuo.append(duocuoall->at(duorand[n]));
                }
                else {
                    //单选
                    if(dancuo.indexOf(dancuoall->at(danrand[n]))==-1) dancuo.append(dancuoall->at(danrand[n]));
                }
            }
            else {
                if(i >= (dannum +duonum)){
                    //判断提
                    n = i - dannum - duonum;
                    if(pancuo.indexOf(panrand[n])==-1) pancuo.append(panrand[n]);
                }
                else if(i >= dannum){
                    n = i - dannum ;
                    //多选
                    if(duocuo.indexOf(duorand[n])==-1) duocuo.append(duorand[n]);
                }
                else {
                    //单选
                    if(dancuo.indexOf(danrand[n])==-1) dancuo.append(danrand[n]);
                }
            }
        }
    }
    if(deadline >0 && !undo.isEmpty()){
        QMessageBox::warning(this,"不能交卷",undo+"题目还没做!","是");
        return;
    }
    timer->stop();
    QString inf = "你的总成绩："+QString().setNum(sum)+
            "\n其中做错：\n单选题："+QString().setNum(dancuo.length())+
            "题\n多选题：" + QString().setNum(duocuo.length())+
            "题\n判选题："+QString().setNum(pancuo.length())+
            "题\n未完成："+QString().setNum(ud)+"题";
    QMessageBox msgBox;
    msgBox.setWindowTitle("成绩");
    msgBox.setInformativeText(inf);
    msgBox.setButtonText(QMessageBox::Ok,"完成");
    msgBox.exec();
    qDebug()<<"score:"<<sum;
    int cuo = dancuo.length()+duocuo.length()+pancuo.length();
    emit finish(cuo ,all ,dancuo,duocuo,pancuo);
    close();
}

void test::saveanswer(){
    //qDebug()<<"save";
    if(QString( youranswer.at(now)).isEmpty()) did +=1;//证明这题还未做的
    QString a;
    if(ui->A->isChecked()) a = "A";
    else if(ui->B->isChecked()) a = "B";
    else if(ui->C->isChecked()) a = "C";
    else if(ui->D->isChecked()) a = "D";
    else {
     if( ui->AA->isChecked()) a="#A";//前面的#必须的，用于区分选了单个A时
     if( ui->BB->isChecked()) a+="#B";
     if( ui->CC->isChecked()) a+="#C";
     if( ui->DD->isChecked()) a+="#D";
    }
    if(a.isEmpty()) did--;
    youranswer.replace(now , a);//
    ui->yizuo->setText(QString("已做%1题/共%2题").arg(did).arg(all));

}

void test::saveanswer2(){
   // qDebug()<<"save2";
    if(QString( youranswer.at(now+1)).isEmpty()) did +=1;//证明这题还未做的
    QString a;
    if(ui->A_2->isChecked()) a = "A";
    else if(ui->B_2->isChecked()) a = "B";
    else if(ui->C_2->isChecked()) a = "C";
    else if(ui->D_2->isChecked()) a = "D";
    else {
     if( ui->AA_2->isChecked()) a="#A";//前面的#必须的，用于区分选了单个A时
     if( ui->BB_2->isChecked()) a+="#B";
     if( ui->CC_2->isChecked()) a+="#C";
     if( ui->DD_2->isChecked()) a+="#D";
    }
    if(a.isEmpty()) did--;
    youranswer.replace((now+1) , a);//
    ui->yizuo->setText(QString("已做%1题/共%2题").arg(did).arg(all));

}


void test::timeout(){
    timer->stop();
    QString t ;
    deadline-=1;
    if(deadline <= 0) {
        on_submit_clicked();
        return;

    }
    int min = deadline/60;
    int sec = deadline%60;
    t = "剩余时间："+QString("%1").arg(min,2,10,QLatin1Char('0'))+":"+QString("%1").arg(sec , 2 ,10,QLatin1Char('0'));
    ui->timer->setText(t);
    timer->start(1000);

}


void test:: closeEvent(QCloseEvent* event){
    if(!timer->isActive()) //交卷后
    {
        return;
    }
    int result = QMessageBox::question(this,"退出考试？","是否要退出考试？","是","否");
    if (result!=0) {
        event->ignore();
        return;
    }
    emit finish(0 ,0 ,QList<int>(),QList<int>(),QList<int>());
    return;
}

void test::setcuoti(QList<int>* dan,QList<int>* duo ,QList<int>* pan){
    dancuoall = dan;
    duocuoall = duo;
    pancuoall = pan;

    if(dannum > dan->length()) this->dannum = dan->length();
    if(duonum > duo->length())  this->duonum = duo->length();
    if(pannum > pan->length())   this->pannum = pan->length();

    all = dannum + duonum + pannum;

    ui->xuanti->setMaximum(all);
    ui->xuanti->setMinimum(1);
    ui->xuanti->setValue(1);
    ui->yizuo->setText(QString("已做%1题/共%2题").arg(0).arg(all));
    qDebug()<<"all"<<all;
}


void test:: start(){
    dancuo.clear();
    duocuo.clear();
    pancuo.clear();
    rand();
    qDebug()<<"randed."  ;
    initanswer();
    qDebug()<<"ininanswered."  ;
    refresh(0);
    timer = new QTimer(this);
    timer->setInterval(1000);
    timer->setSingleShot(false);
    connect(timer , SIGNAL(timeout()) ,this,SLOT(timeout()));
    timer->start();
}
