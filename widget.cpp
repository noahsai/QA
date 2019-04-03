#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("题检");
    setWindowIcon(QIcon(":/tiku/ti.png"));
    ui->testset->setWindowFlags(Qt::Window);
    ui->testset->setWindowTitle("设置题数");
    path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)+"/QAtiku/tiku";
    QDir().mkpath(path);
    QDir().mkpath(path +"/DefaultTiku");

    ui->testset->hide();
    test_or_cuoti = true;
    t = NULL;
    ku = NULL;
    cuotiji = NULL;
    nowtiku = "DefaultTiku";
    reset();
    readcfg();
    inittiku();
}

Widget::~Widget()
{
    savetiku();
    savecuoti();
    savecfg();
    delete ui;
}

bool Widget::readtiku(QString& name){
    reset(); //重置数据

    QFile file;
    file.setFileName(name);
    //name不是文件路径
    if(!file.exists()){
        QString filepath ;
        if(name == "DefaultTiku")   filepath = ":/tiku/ti";
        else    filepath =  path +"/"+name +"/ti";
        file.setFileName(filepath);
    }
    if(file.open(QIODevice::ReadOnly))
    {
        QString line,word;
        QStringList list;
        QString tmp;
        QString tmp2;
        while(!file.atEnd())
        {
            tmp = file.readLine();
            line = tmp;
            bool isline = true;
            if(line.indexOf("$")!=-1) {
                isline = false;//isline后面没用的，只为了不用每次indexof罢了
            }
            while(!file.atEnd()&& (!isline)){
                tmp2 = file.readLine();
                line = line + tmp2;
                if(tmp2.indexOf(QRegularExpression("^[^\\$]*\\$[^\\$]*$"))!=-1) {
                    line.remove("$");
                    break;
                }
            }
            list = line.split("\t");
            word = list.at(0);
            list.removeFirst();
            line = list.join("\t");
            if(word.indexOf("单")!=-1){
                danlist.append(line);
            }
            else if(word.indexOf("多")!=-1){
                duolist.append(line);
            }
            else if(word.indexOf("判")!=-1){
                panlist.append(line);
            }
            else qDebug()<<"unknow type!"<<line;
        }
        if(danlist.isEmpty() && duolist.isEmpty() && panlist.isEmpty()) return false;
        //qDebug()<<danlist<<endl<<duolist<<endl<<panlist;
        file.close();
        return true;
    }
    else {
        qDebug()<<"open tiku error!";
        return false;
    }

}

void Widget::readcfg(){
    QSettings settings("QAtiku","cfg");
    nowtiku = settings.value("nowtiku", "DefaultTiku").toString();
}

void Widget::inittiku(){
    QFile file(path +"/" + nowtiku + "/tikudata");
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in>>danlist>>duolist>>panlist;
        if(nowtiku == "DefaultTiku") ui->nowtiku->setText("当前题库：默认" );
        else ui->nowtiku->setText("当前题库：" + nowtiku );
        file.close();
    }
    else {
        if(readtiku(nowtiku))    ui->nowtiku->setText("当前题库：" + nowtiku );
        else {
            if(nowtiku!="DefaultTiku"){
                nowtiku = "DefaultTiku";
                QDir().mkpath(path + "DefaultTiku");
                readtiku(nowtiku);
                ui->nowtiku->setText("当前题库：默认" );
            }
        }
    }

    file.setFileName( path +"/" + nowtiku + "/cuoti");//nowtiku在上面会变化，所以用即时相加的形式
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in>>dancuoti>>duocuoti>>pancuoti;
        file.close();
    }
//qDebug()<<danlist<<endl<<duolist<<endl<<panlist;
}


void Widget::savetiku(){
    if(danlist.isEmpty()&&duolist.isEmpty()&&panlist.isEmpty()) return;
    QFile file(path + "/" + nowtiku + "/tikudata");
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out<<danlist<<duolist<<panlist;
    }
    file.close();
    qDebug()<<"savetiku";
}

void Widget::savecuoti(){
    if(dancuoti.isEmpty()&&duocuoti.isEmpty()&&pancuoti.isEmpty()) return;
    QFile file(path + "/" + nowtiku +"/cuoti");
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out<<dancuoti<<duocuoti<<pancuoti;
    }
    file.close();
    qDebug()<<"savecuoti";
}

void Widget::on_pushButton_clicked()
{
    if(t!=NULL) {
        t->activateWindow();
        return;
    }
    test_or_cuoti = true;
    ui->dan->setMaximum(danlist.length());
    ui->danall->setText("共"+QString().setNum( danlist.length())+"题");
    ui->dan->setValue(120);
    ui->duo->setMaximum(duolist.length());
    ui->duoall->setText("共"+QString().setNum( duolist.length())+"题");
    ui->duo->setValue(50);
    ui->pan->setMaximum(panlist.length());
    ui->panall->setText("共"+QString().setNum( panlist.length())+"题");
    ui->pan->setValue(80);
    ui->testset->show();
}

void Widget::on_test_clicked()
{
    if(ui->dan->value()==0 && ui->duo->value()==0 && ui->pan->value()==0 )
        return;
    t = new test(&danlist  ,ui->dan->value(),&duolist ,ui->duo->value(),&panlist,ui->pan->value(),ui->shijian->value());
    t->setWindowFlags(Qt::Window);
    t->setWindowIcon(QIcon(":/tiku/ti.png"));
    if(!test_or_cuoti) t->setcuoti(&dancuoti , &duocuoti , &pancuoti);
    connect(t,SIGNAL(finish(int,int,QList<int>,QList<int>,QList<int>)),this,SLOT(tested(int,int,QList<int>,QList<int>,QList<int>)));
    t->start();
    t->show();

    ui->testset->hide();
    this->showMinimized();
}


void Widget:: tested(int cuo , int all ,QList<int> dan, QList<int> duo ,QList<int> pan){
    t = NULL;
    if(all ==0 ) {
        this->showNormal();
        this->activateWindow();
        return;//说明考试窗口是手动关闭的。
    }
    //cuo和all用于做历史记录的。暂未使用
    if(test_or_cuoti){
        for(int i=0;i<dan.length();i++){
            int n = dan.at(i);
            if(dancuoti.indexOf(n)==-1) dancuoti.append(n);
        }

        for(int i=0;i<duo.length();i++){
            int n = duo.at(i);
            if(duocuoti.indexOf(n)==-1) duocuoti.append(n);
        }

        for(int i=0;i<pan.length();i++){
            int n = pan.at(i);
            if(pancuoti.indexOf(n)==-1) pancuoti.append(n);
        }
    }
    else {
        //由于重做错题时能设置题数，所以不能直接替换错题列表。
        for(int i=0;i<dancuoti.length();i++){
            int n = dancuoti.at(i);
            if(dan.indexOf(n)==-1) {
                dancuoti.removeOne(n);
                i--;
            }
        }

        for(int i=0;i<duocuoti.length();i++){
            int n = duocuoti.at(i);
            if(duo.indexOf(n)==-1) {
                duocuoti.removeOne(n);
                i--;
            }
        }

        for(int i=0;i<pancuoti.length();i++){
            int n = pancuoti.at(i);
            if(pan.indexOf(n)==-1) {
                pancuoti.removeOne(n);
                i--;
            }
        }
    }
    double result = 100.0*(all-cuo)/all;
    QString t = QString("上次得分：%1（%2/%3）").arg(result).arg(all-cuo).arg(all);
    ui->score->setText(t);
    //qDebug()<<dan<<duo<<pan;
    //qDebug()<<dancuoti<<duocuoti<<pancuoti;
    this->showNormal();
    this->activateWindow();
    savecuoti();
}

void Widget::on_pushButton_2_clicked()
{
    if(t!=NULL) {
        t->activateWindow();
        return;
    }
    test_or_cuoti = false;
    ui->dan->setMaximum(dancuoti.length());
    ui->danall->setText("共"+QString().setNum( dancuoti.length())+"题");
    ui->duo->setMaximum(duocuoti.length());
    ui->duoall->setText("共"+QString().setNum( duocuoti.length())+"题");
    ui->pan->setMaximum(pancuoti.length());
    ui->panall->setText("共"+QString().setNum( pancuoti.length())+"题");

    ui->testset->show();
}

void Widget::on_pushButton_4_clicked()
{
    if(ku) {
        ku->showNormal();
        ku->activateWindow();
        return;
    }
    ku = new tiku(this);
    ku->setObjectName("ku");
    connect( ku ,SIGNAL(destroyed(QObject*)),this , SLOT(tikuclose(QObject*)));
    ku->setWindowTitle("题库");
    ku->setWindowFlags(Qt::Window);
    ku->setAttribute(Qt::WA_DeleteOnClose);
    ku->settiku(danlist , duolist , panlist);
    ku->showNormal();
}

void Widget::on_pushButton_3_clicked()
{
    if(cuotiji){
        cuotiji->showNormal();
        cuotiji->activateWindow();
        return;
    }
    QStringList danlist , duolist , panlist;
    for(int i=0;i<dancuoti.length();i++)
    {
        danlist.append(this->danlist.at(dancuoti.at(i)));
    }
    for(int i=0;i<duocuoti.length();i++)
    {
        duolist.append(this->duolist.at(duocuoti.at(i)));
    }
    for(int i=0;i<pancuoti.length();i++)
    {
        panlist.append(this->panlist.at(pancuoti.at(i)));
    }

    cuotiji = new tiku(this);
    cuotiji->setObjectName("cuoti");
    connect( cuotiji ,SIGNAL(destroyed(QObject*)),this , SLOT(tikuclose(QObject*)));
    cuotiji->setWindowTitle("错题集");
    cuotiji->setWindowFlags(Qt::Window);
    cuotiji->setAttribute(Qt::WA_DeleteOnClose);
    cuotiji->settiku(danlist , duolist , panlist);
    cuotiji->showNormal();

}

void Widget::tikuclose(QObject* w){
    //qDebug()<<w->objectName();
    if(w->objectName() == "ku"){
        ku = NULL;
    }
    else cuotiji = NULL;
}

void Widget::savecfg(){
    QSettings settings("QAtiku","cfg");
    settings.setValue("nowtiku", nowtiku);
    qDebug()<<"savecfged";
}

void Widget::on_xuantiku_activated(int index)
{
    if(t) {
        QMessageBox::warning(this,"操作失败","正在做题，不能操作题库！","确定");
        qDebug()<<"正在做题！";
        return;
    }
    if(index == 0 )
    {
        QString fp = QFileDialog().getExistingDirectory(this , "选择题库文件夹", path );
        if(fp.isEmpty())  return;
        QString name = fp.split('/').last();
        nowtiku = name;
        inittiku();
    }
    else if(index == 1 ){
        QString fp = QFileDialog().getOpenFileName(this , "选择csv题库", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) ,"csv表格 (*.csv);;全部 (*)");
        if(fp.isEmpty())  return;
        if(readtiku(fp)) {
            QString name = fp.split('/').last();
            name = name.remove('.');
            QDir().mkpath(path +"/" + name);
            QFile(path +"/" + name + "/ti").remove();
            if(QFile(fp).copy(path +"/" + name + "/ti")) {
                nowtiku = name;
                ui->nowtiku->setText("当前题库：" + name );
                qDebug()<<"读取并复制题库";
            }
            else   qDebug()<<"读取题库成功，但复制题库失败";

            QFile file(path+"/"+nowtiku + "/cuoti");
            if(file.open(QIODevice::ReadOnly))
            {
                QDataStream in(&file);
                in>>dancuoti>>duocuoti>>pancuoti;
                file.close();
            }
        }
        else   {
            inittiku();//重新载入当前nowtiku的题库数据
            qDebug()<<"读取题库失败";
        }
    }
    else {
        QProcess pro;
        QString cmd = "xdg-open \""+ path +"\"";
        pro.startDetached(cmd);
    }
}

void Widget::reset(){
    danlist.clear();
    duolist.clear();
    panlist.clear();
    dancuoti.clear();
    duocuoti.clear();
    pancuoti.clear();
}
