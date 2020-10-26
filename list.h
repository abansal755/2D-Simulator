#include<QListWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QPushButton>
#include<QLabel>
#include<QSpinBox>
#include<QColorDialog>
#include"system.h"

class propertiesWindow:public QDialog{
public:
    propertiesWindow(QString title,QWidget*parent=NULL):QDialog(parent){
        setWindowTitle(title);
        setWindowFlags(Qt::WindowTitleHint|Qt::WindowCloseButtonHint);
        QSS(this);
    }
};

class particlePropertiesWindow:public propertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
    QHBoxLayout*hBox[5];
    QLabel*label[8];
    QDoubleSpinBox*doubleSpinBox[6];
    QSpinBox*spinBox[2];
    QPushButton*b1;
    QColorDialog*cd;
public:
    particlePropertiesWindow(QString title,QWidget*parent=NULL):propertiesWindow(title,parent){
        v1=new QVBoxLayout;
        for(int i=0;i<5;i++) hBox[i]=new QHBoxLayout;
        label[0]=new QLabel("x:");
        label[1]=new QLabel("y:");
        label[2]=new QLabel("vx:");
        label[3]=new QLabel("vy:");
        label[4]=new QLabel("q:");
        label[5]=new QLabel("m:");
        label[6]=new QLabel("radius:");
        label[7]=new QLabel("trajectRadius:");
        for(int i=0;i<6;i++) doubleSpinBox[i]=new QDoubleSpinBox;
        for(int i=0;i<2;i++) spinBox[i]=new QSpinBox;
        for(int i=0;i<6;i++){
            doubleSpinBox[i]->setRange(-FLT_MAX,FLT_MAX);
            doubleSpinBox[i]->setDecimals(10);
        }
        for(int i=0;i<2;i++) spinBox[i]->setRange(1,INT_MAX);
        spinBox[0]->setValue(5);
        spinBox[1]->setValue(3);
        b1=new QPushButton("Pick Color");
        cd=new QColorDialog(Qt::white,this);
        cd->setOptions(QColorDialog::NoButtons);

        for(int i=0;i<4;i++){
            hBox[i]->addWidget(label[2*i]);
            if(i!=3) hBox[i]->addWidget(doubleSpinBox[2*i]);
            else hBox[i]->addWidget(spinBox[0]);
            hBox[i]->addWidget(label[2*i+1]);
            if(i!=3) hBox[i]->addWidget(doubleSpinBox[2*i+1]);
            else hBox[i]->addWidget(spinBox[1]);
            hBox[i]->addStretch();
        }
        hBox[4]->addWidget(b1);
        hBox[4]->addStretch();
        for(int i=0;i<5;i++) v1->addLayout(hBox[i]);
        v1->addStretch();
        setLayout(v1);

        connect(b1,SIGNAL(clicked()),cd,SLOT(exec()));
    }
    particle* createParticle(){
        return new particle(doubleSpinBox[2]->value(),doubleSpinBox[3]->value(),
                            doubleSpinBox[0]->value(),doubleSpinBox[1]->value(),
                            doubleSpinBox[4]->value(),doubleSpinBox[5]->value(),
                            spinBox[0]->value(),spinBox[1]->value(),
                            cd->currentColor());
    }
};

class fieldPropertiesWindow:public propertiesWindow{
public:
    fieldPropertiesWindow(QString title,QWidget*parent=NULL):propertiesWindow(title,parent){}
    virtual field* createField(){return new field;}
};

class radialFieldPropertiesWindow:public fieldPropertiesWindow{
    QVBoxLayout*v1;
    QHBoxLayout*hBox[2];
    QLabel*label[3];
    QDoubleSpinBox*spinBox[3];
public:
    radialFieldPropertiesWindow(QString title,QWidget*parent=NULL):fieldPropertiesWindow(title,parent){
        v1=new QVBoxLayout;
        for(int i=0;i<2;i++) hBox[i]=new QHBoxLayout;
        for(int i=0;i<3;i++) spinBox[i]=new QDoubleSpinBox;
        label[0]=new QLabel("cx:");
        label[1]=new QLabel("cy:");
        label[2]=new QLabel("k:");
        for(int i=0;i<2;i++) spinBox[i]->setRange(-FLT_MAX,FLT_MAX);
        spinBox[2]->setRange(0,FLT_MAX);
        for(int i=0;i<3;i++) spinBox[i]->setDecimals(10);

        for(int i=0;i<2;i++){
            hBox[0]->addWidget(label[i]);
            hBox[0]->addWidget(spinBox[i]);
        }
        hBox[1]->addWidget(label[2]);
        hBox[1]->addWidget(spinBox[2]);
        for(int i=0;i<2;i++){
            hBox[i]->addStretch();
            v1->addLayout(hBox[i]);
        }
        v1->addStretch();
        setLayout(v1);
    }
    float getCx(){return spinBox[0]->value();}
    float getCy(){return spinBox[1]->value();}
    float getK(){return spinBox[2]->value();}
};

class gravitationalRadialFieldPropertiesWindow:public radialFieldPropertiesWindow{
public:
    gravitationalRadialFieldPropertiesWindow(QString title,QWidget*parent=NULL):radialFieldPropertiesWindow(title,parent){}
    field * createField(){return new GRF(getCx(),getCy(),getK());}
};

class electricRadialFieldPropertiesWindow:public radialFieldPropertiesWindow{
public:
    electricRadialFieldPropertiesWindow(QString title,QWidget*parent=NULL):radialFieldPropertiesWindow(title,parent){}
    field * createField(){return new ERF(getCx(),getCy(),getK());}
};

class uniformFieldPropertiesWindow:public fieldPropertiesWindow{
    QVBoxLayout*v1;
    QHBoxLayout*h1;
    QLabel*label[2];
    QDoubleSpinBox*spinBox[2];
public:
    uniformFieldPropertiesWindow(QString title,QWidget*parent=NULL):fieldPropertiesWindow(title,parent){
        v1=new QVBoxLayout;
        h1=new QHBoxLayout;
        for(int i=0;i<2;i++){
            spinBox[i]=new QDoubleSpinBox;
            spinBox[i]->setRange(-FLT_MAX,FLT_MAX);
            spinBox[i]->setDecimals(10);
        }
        label[0]=new QLabel("ex:");
        label[1]=new QLabel("ey:");
        for(int i=0;i<2;i++){
            h1->addWidget(label[i]);
            h1->addWidget(spinBox[i]);
        }
        h1->addStretch();
        v1->addLayout(h1);
        v1->addStretch();
        setLayout(v1);
    }
    float getEx(){return spinBox[0]->value();}
    float getEy(){return spinBox[1]->value();}
};

class gravitationalUniformFieldPropertiesWindow:public uniformFieldPropertiesWindow{
public:
    gravitationalUniformFieldPropertiesWindow(QString title,QWidget*parent=NULL):uniformFieldPropertiesWindow(title,parent){}
    field* createField(){return new GUF(getEx(),getEy());}
};

class electricUniformFieldPropertiesWindow:public uniformFieldPropertiesWindow{
public:
    electricUniformFieldPropertiesWindow(QString title,QWidget*parent=NULL):uniformFieldPropertiesWindow(title,parent){}
    field* createField(){return new EUF(getEx(),getEy());}
};

class listItem:public QListWidgetItem{
    propertiesWindow*win;
public:
    listItem(const QString&text,QListWidget*parent=NULL):QListWidgetItem(text,parent){
        win=NULL;
    }
    propertiesWindow* getPropertiesWindow(){return win;}
    void setPropertiesWindow(propertiesWindow*win){this->win=win;}
    virtual ~listItem(){
        delete win;
    }
};

class particleListItem:public listItem{
public:
    particleListItem(QListWidget*parent=NULL):listItem("particle"+QString::number(index++),parent){
        setPropertiesWindow(new particlePropertiesWindow(text()));
    }
    static int index;
};

class fieldListItem:public listItem{
public:
    fieldListItem(const QString&text,QListWidget*parent=NULL):listItem(text,parent){}
};

class gravitationalRadialFieldListItem:public fieldListItem{
public:
    gravitationalRadialFieldListItem(QListWidget*parent=NULL):fieldListItem("gravitationalRadialField"+QString::number(index++),parent){
        setPropertiesWindow(new gravitationalRadialFieldPropertiesWindow(text()));
    }
    static int index;
};

class gravitationalUniformFieldListItem:public fieldListItem{
public:
    gravitationalUniformFieldListItem(QListWidget*parent=NULL):fieldListItem("gravitationalUniformField"+QString::number(index++),parent){
        setPropertiesWindow(new gravitationalUniformFieldPropertiesWindow(text()));
    }
    static int index;
};

class electricRadialFieldListItem:public fieldListItem{
public:
    electricRadialFieldListItem(QListWidget*parent=NULL):fieldListItem("electricRadialField"+QString::number(index++),parent){
        setPropertiesWindow(new electricRadialFieldPropertiesWindow(text()));
    }
    static int index;
};

class electricUniformFieldListItem:public fieldListItem{
public:
    electricUniformFieldListItem(QListWidget*parent=NULL):fieldListItem("electricUniformField"+QString::number(index++),parent){
        setPropertiesWindow(new electricUniformFieldPropertiesWindow(text()));
    }
    static int index;
};

class listWidget:public QWidget{
    QListWidget*lw1;
    QHBoxLayout*h1;
    Q_OBJECT
protected slots:
    virtual void newClicked(){}
    virtual void editClicked(){}
    void deleteClicked(){
        delete lw1->takeItem(lw1->currentRow());
    }
    void change(QListWidgetItem*current,QListWidgetItem*previous){
        if(current==NULL){
            b2->setDisabled(true);
            b3->setDisabled(true);
        }else{
            b2->setEnabled(true);
            b3->setEnabled(true);
        }
    }
public:
    QVBoxLayout*v1,*v2;
    QPushButton*b1,*b2,*b3;
    listWidget(QWidget*parent=NULL):QWidget(parent){
        lw1=new QListWidget(this);
        v1=new QVBoxLayout;
        v2=new QVBoxLayout;
        h1=new QHBoxLayout;
        b1=new QPushButton("New");
        b2=new QPushButton("Edit");
        b3=new QPushButton("Delete");

        v1->addWidget(lw1);
        v2->addWidget(b1);
        v2->addWidget(b2);
        v2->addWidget(b3);
        v2->addStretch();
        h1->addLayout(v1);
        h1->addLayout(v2);
        setLayout(h1);

        connect(b1,SIGNAL(clicked()),this,SLOT(newClicked()));
        connect(b2,SIGNAL(clicked()),this,SLOT(editClicked()));
        connect(b3,SIGNAL(clicked()),this,SLOT(deleteClicked()));
        connect(lw1,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(change(QListWidgetItem*,QListWidgetItem*)));

        b2->setDisabled(true);
        b3->setDisabled(true);
    }
    ~listWidget(){
        lw1->clear();
    }
    QListWidget* getListWidget(){return lw1;}
};

class particlesListWidget:public listWidget{
    Q_OBJECT
protected slots:
    void newClicked(){
        getListWidget()->addItem(new particleListItem);
    }
    void editClicked(){
        particleListItem*current=(particleListItem*)getListWidget()->currentItem();
        if(current) current->getPropertiesWindow()->exec();
    }
public:
    particlesListWidget(QWidget*parent=NULL):listWidget(parent){}
};

class fieldsListWidget:public listWidget{
    Q_OBJECT
    QPushButton*b4,*b5,*b6;
protected slots:
    void editClicked(){
        fieldListItem*current=(fieldListItem*)getListWidget()->currentItem();
        if(current) current->getPropertiesWindow()->exec();
    }
    void newClicked(){
        getListWidget()->addItem(new gravitationalRadialFieldListItem);
    }
    void b4Clicked(){
        getListWidget()->addItem(new gravitationalUniformFieldListItem);
    }
    void b5Clicked(){
        getListWidget()->addItem(new electricRadialFieldListItem);
    }
    void b6Clicked(){
        getListWidget()->addItem(new electricUniformFieldListItem);
    }
public:
    fieldsListWidget(QWidget*parent=NULL):listWidget(parent){
        b1->setText("New Gravitational Radial Field");
        b4=new QPushButton("New Gravitational Uniform Field",this);
        b5=new QPushButton("New Electric Radial Field",this);
        b6=new QPushButton("New Electric Uniform Field",this);
        v2->insertWidget(1,b4);
        v2->insertWidget(2,b5);
        v2->insertWidget(3,b6);

        connect(b4,SIGNAL(clicked()),this,SLOT(b4Clicked()));
        connect(b5,SIGNAL(clicked()),this,SLOT(b5Clicked()));
        connect(b6,SIGNAL(clicked()),this,SLOT(b6Clicked()));
    }
};
