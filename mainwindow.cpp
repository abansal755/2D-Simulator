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

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}
