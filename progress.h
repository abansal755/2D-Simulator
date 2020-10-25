#include"sliderint.h"
#include<QDialog>
#include<QVBoxLayout>
#include<QDialogButtonBox>

class qualityFactorDialog:public QDialog{
    Q_OBJECT
    sliderInt*slider1;
    QVBoxLayout*v1;
    QDialogButtonBox*b1;
    QHBoxLayout*h1;
public:
    qualityFactorDialog(QWidget*parent=NULL):QDialog(parent){
        setWindowTitle("Quality Factor");
        setWindowFlags(Qt::WindowTitleHint|Qt::WindowCloseButtonHint);
        v1=new QVBoxLayout;
        slider1=new sliderInt;
        slider1->setText("Quality Factor:");
        slider1->setRange(0,100);
        slider1->setDefaultValue(100);
        h1=new QHBoxLayout;
        b1=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);

        h1->addStretch();
        h1->addWidget(b1);
        v1->addWidget(slider1);
        v1->addLayout(h1);
        v1->addStretch();
        setLayout(v1);

        connect(b1,SIGNAL(accepted()),this,SLOT(accept()));
        connect(b1,SIGNAL(rejected()),this,SLOT(reject()));

        setStyleSheet("QWidget{background-color: rgb(30,30,30);color:rgb(231,231,231);font: 800 10pt ""Consolas"";border: 0px;}"
                      "QWidget#centralwidget{background-color: rgb(37,37,37);}"
                      "QSpinBox{background:rgb(60,60,60);padding: 5px;font: 10pt ""Consolas"";}"
                      "QSpinBox:focus{border: 1px solid rgb(0,127,212);}"
                      "QDoubleSpinBox{background:rgb(60,60,60);padding: 3px;font: 10pt ""Consolas"";}"
                      "QDoubleSpinBox:focus{border: 1px solid rgb(0,127,212);}"
                      "QPushButton{background-color:rgb(14,99,156);color: white;padding: 10px;}"
                      "QPushButton:disabled{background-color:rgb(77,77,77);}"
                      "QPushButton:hover{background-color:rgb(17,119,187);}"
                      );
    }
    int getValue(){
        return slider1->getValue();
    }
};
