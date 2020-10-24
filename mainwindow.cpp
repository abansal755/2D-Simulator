#include "mainwindow.h"

float distance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x1 - x2, 2) + pow((y1 - y2), 2));
}

int numDigits(int n){
    if(n==0) return 1;
    int ans=0;
    while(n!=0){
        ans++;
        n/=10;
    }
    return ans;
}

void blur(QImage&img,int radius=1){
    int width = img.width(), height = img.height();
    QImage img1(width,height,img.format());
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int r = 0, g = 0, b = 0;
            int num = 0;
            for (int di = -radius; di <= radius; di++) {
                for (int dj = -radius; dj <= radius; dj++) {
                    if (i + di < 0 || i + di >= height) continue;
                    if (j + dj < 0 || j + dj >= width) continue;
                    QColor c=img.pixelColor(j+dj,i+di);
                    r += c.red();
                    g += c.green();
                    b += c.blue();
                    num++;
                }
            }
            r /= num;
            g /= num;
            b /= num;
            img1.setPixelColor(j,i,QColor(r,g,b,img.pixelColor(j,i).alpha()));
        }
    }
    img=img1;
}

int particleListItem::index=0;
int gravitationalRadialFieldListItem::index=0;
int gravitationalUniformFieldListItem::index=0;
int electricRadialFieldListItem::index=0;
int electricUniformFieldListItem::index=0;

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->doubleSpinBox->setRange(FLT_MIN,FLT_MAX);
    ui->doubleSpinBox_2->setRange(FLT_MIN,FLT_MAX);
    ui->doubleSpinBox_3->setRange(FLT_MIN,FLT_MAX);

    ui->doubleSpinBox->setDecimals(10);
    ui->doubleSpinBox_2->setDecimals(10);
    ui->doubleSpinBox_2->setDecimals(10);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    //Simulate button clicked
    sys.setName(ui->lineEdit->text());
    sys.setScale(ui->doubleSpinBox->value());
    sys.setBoundX(ui->spinBox->value());
    sys.setBoundY(ui->spinBox_2->value());
    sys.setIterations(ui->spinBox_3->value());
    sys.setDuration(ui->spinBox_4->value());
    sys.setTimeFactor(ui->doubleSpinBox_2->value());
    sys.setVisc_K(ui->doubleSpinBox_3->value());

    int numParticles=ui->widget->getListWidget()->count();
    int numFields=ui->widget_2->getListWidget()->count();

    sys.clearParticles();
    sys.clearFields();

    for(int i=0;i<numParticles;i++){
        particlePropertiesWindow*p=(particlePropertiesWindow*)((particleListItem*)ui->widget->getListWidget()->item(i))->getPropertiesWindow();
        sys.addParticle(p->createParticle());
    }
    for(int i=0;i<numFields;i++){
        fieldPropertiesWindow*f=(fieldPropertiesWindow*)(((fieldListItem*)ui->widget_2->getListWidget()->item(i))->getPropertiesWindow());
        sys.addField(f->createField());
    }
    sys.simulate();
}
