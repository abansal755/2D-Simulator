#include<QColor>
using namespace std;

class particle{
    float ax,ay;
    float vx,vy;
    float x,y;
    float q,m;
    int radius,trajectRadius;
    QColor color;
public:
    particle(float vx=0,float vy=0,float x=0,float y=0,float q=0,float m=0,int radius=5,int trajectRadius=5,QColor color=Qt::white)
        :ax(0),ay(0),vx(vx),vy(vy),x(x),y(y),q(q),m(m),radius(radius),trajectRadius(trajectRadius),color(color){}
    //getters
    float Ax(){return ax;}
    float Ay(){return ay;}
    float Vx(){return vx;}
    float Vy(){return vy;}
    float X(){return x;}
    float Y(){return y;}
    float Q(){return q;}
    float M(){return m;}
    int Radius(){return radius;}
    int TrajectRadius(){return trajectRadius;}
    QColor Color(){return color;}
    //setters
    void setAx(float ax){this->ax=ax;}
    void setAy(float ay){this->ay=ay;}
    void setVx(float vx){this->vx=vx;}
    void setVy(float vy){this->vy=vy;}
    void setX(float x){this->x=x;}
    void setY(float y){this->y=y;}
    void setQ(float q){this->q=q;}
    void setM(float m){this->m=m;}
    void setRadius(int radius){this->radius=radius;}
    void setTrajectRadius(int trajectRadius){this->trajectRadius=trajectRadius;}
    void setColor(QColor color){this->color=color;}
};
