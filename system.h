#include<bits/stdc++.h>
#include<QColor>
using namespace std;

const float _G = 6.67259e-11;
const float _K = 9e9;

float distance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x1 - x2, 2) + pow((y1 - y2), 2));
}

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

class field{
public:
    virtual float Ax(particle p){return 0;}
    virtual float Ay(particle p){return 0;}
};

class uniformField:public field{
protected:
    float ex,ey;
public:
    uniformField(float ex=0,float ey=0):ex(ex),ey(ey){}
    float Ex(){return ex;}
    float Ey(){return ey;}
    void setEx(float ex){this->ex=ex;}
    void setEy(float ey){this->ey=ey;}
};

class radialField:public field{
protected:
    float cx,cy,k;
public:
    radialField(float cx=0,float cy=0,float k=0):cx(cx),cy(cy),k(k){}
    float Cx(){return cx;}
    float Cy(){return cy;}
    float K(){return k;}
    void setCx(float cx){this->cx=cx;}
    void setCy(float cy){this->cy=cy;}
    void setK(float k){this->k=k;}
    float magnitude(float x,float y){
        return k/(pow(x-cx,2)+pow(y-cy,2));
    }
};

class gravitationalUniformField:public uniformField{
public:
    gravitationalUniformField(float ex=0,float ey=0):uniformField(ex,ey){}
    float Ax(particle p){return ex;}
    float Ay(particle p){return ey;}
};

class gravitationalRadialField:public radialField{
public:
    gravitationalRadialField(float cx=0,float cy=0,float k=0):radialField(cx,cy,k){}
    float Ax(particle p){
        float cos=(p.X()-cx)/distance(p.X(),p.Y(),cx,cy);
        return -cos*magnitude(p.X(),p.Y());
    }
    float Ay(particle p){
        float sin=(p.Y()-cy)/distance(p.X(),p.Y(),cx,cy);
        return -sin*magnitude(p.X(),p.Y());
    }
};

class electricUniformField:public uniformField{
public:
    electricUniformField(float ex=0,float ey=0):uniformField(ex,ey){}
    float Ax(particle p){return ex*p.Q()/p.M();}
    float Ay(particle p){return ey*p.Q()/p.M();}
};

class electricRadialField:public radialField{
public:
    electricRadialField(float cx=0,float cy=0,float k=0):radialField(cx,cy,k){}
    float Ax(particle p){
        float cos=(p.X()-cx)/distance(p.X(),p.Y(),cx,cy);
        return p.Q()*cos*magnitude(p.X(),p.Y())/p.M();
    }
    float Ay(particle p){
        float sin=(p.Y()-cy)/distance(p.X(),p.Y(),cx,cy);
        return p.Q()*sin*magnitude(p.X(),p.Y())/p.M();
    }
};
