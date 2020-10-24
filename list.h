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
    }
};

class particlePropertiesWindow:public propertiesWindow{
    Q_OBJECT
    QVBoxLayout*v1;
    QHBoxLayout*hBox[5];
    QLabel*label[8];
    QSpinBox*spinBox[8];
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
        for(int i=0;i<8;i++) spinBox[i]=new QSpinBox;
        for(int i=0;i<6;i++) spinBox[i]->setRange(INT_MIN,INT_MAX);
        for(int i=6;i<=7;i++) spinBox[i]->setRange(1,INT_MAX);
        spinBox[6]->setValue(5);
        spinBox[7]->setValue(3);
        b1=new QPushButton("Pick Color");
        cd=new QColorDialog(Qt::white,this);

        for(int i=0;i<4;i++){
            hBox[i]->addWidget(label[2*i]);
            hBox[i]->addWidget(spinBox[2*i]);
            hBox[i]->addWidget(label[2*i+1]);
            hBox[i]->addWidget(spinBox[2*i+1]);
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
        return new particle(spinBox[2]->value(),spinBox[3]->value(),
                            spinBox[0]->value(),spinBox[1]->value(),
                            spinBox[4]->value(),spinBox[5]->value(),
                            spinBox[6]->value(),spinBox[7]->value(),
                            cd->currentColor());
    }
};

class listItem:public QListWidgetItem{
    propertiesWindow*win;
public:
    listItem(const QString&text,QListWidget*parent=NULL):QListWidgetItem(text,parent){
        win=NULL;
    }
    propertiesWindow* PropertiesWindow(){return win;}
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

class listWidget:public QWidget{
    QListWidget*lw1;
    QVBoxLayout*v1;
    QHBoxLayout*h1;
    QPushButton*b1,*b2,*b3;
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
    listWidget(QWidget*parent=NULL):QWidget(parent){
        lw1=new QListWidget(this);
        v1=new QVBoxLayout;
        h1=new QHBoxLayout;
        b1=new QPushButton("New");
        b2=new QPushButton("Edit");
        b3=new QPushButton("Delete");

        h1->addStretch();
        h1->addWidget(b1);
        h1->addWidget(b2);
        h1->addWidget(b3);
        v1->addWidget(lw1);
        v1->addLayout(h1);
        setLayout(v1);

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
};
