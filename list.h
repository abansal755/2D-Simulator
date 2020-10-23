#include<QListWidget>
#include<QLabel>
#include"system.h"

class propertiesWindow:public QWidget{
public:
    propertiesWindow(QString title,QWidget*parent=NULL):QWidget(parent){
        setWindowTitle(title);
    }
};

class particlePropertiesWindow:public propertiesWindow{
public:
    particlePropertiesWindow(QString title,QWidget*parent=NULL):propertiesWindow(title,parent){}
};

class listItem:public QListWidgetItem{
protected:
    propertiesWindow*win;
public:
    listItem(const QString&text,QListWidget*parent=NULL):QListWidgetItem(text,parent){
        win=NULL;
    }
    virtual ~listItem(){
        delete win;
    }
};

class particleListItem:public listItem{
    particle p;
public:
    particleListItem(QListWidget*parent=NULL):listItem("particle"+QString::number(index++),parent){
        win=new particlePropertiesWindow(text());
    }
    particle& Particle(){return p;}
    static int index;
};

class listWidget:public QWidget{
public:
    listWidget(QWidget*parent=NULL):QWidget(parent){}
};
