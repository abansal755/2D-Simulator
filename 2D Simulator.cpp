#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <direct.h>
#include "image.h"
#include "System.h"

#define RF radialField
#define UF uniformField
#define EFs electricFields
#define GFs gravitationalFields
#define UGFs uniformGravitationalFields
#define UEFs uniformElectricFields
using namespace std;

void blur(image*& img, unsigned int radius=1) {
    int width = img->width, height = img->height;
    image* img1 = new image(width, height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int r = 0, g = 0, b = 0;
            int num = 0;
            for (int di = -1 * (int)radius; di <= (int)radius; di++) {
                for (int dj = -1 * (int)radius; dj <= (int)radius; dj++) {
                    if (i + di < 0 || i + di >= height) continue;
                    if (j + dj < 0 || j + dj >= width) continue;
                    rgb& p = img->frame[i + di][j + dj];
                    r += p.r;
                    g += p.g;
                    b += p.b;
                    num++;
                }
            }
            r /= num;
            g /= num;
            b /= num;
            rgb p(r, g, b);
            img1->frame[i][j] = p;
        }
    }
    swap(img, img1);
    delete img1;
}

void saturate(image* img, float x) {
    if (x > 1 || x < -1) return;
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            rgb p = img->frame[i][j];
            hsv p1 = RGBtoHSV(p);
            if (x >= 0) p1.s += (1 - p1.s) * x;
            else p1.s *= 1 - abs(x);
            p = HSVtoRGB(p1);
            img->frame[i][j] = p;
        }
    }
}

float distance(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x1 - x2, 2) + pow((y1 - y2), 2));
}

int numOfDigits(int n) {
    if (n == 0) return 1;
    int count = 0;
    while (n != 0) {
        n /= 10;
        count++;
    }
    return count;
}

int main() {
    System s(1,500,500,300,2,3,3);
    particle p(80,40,0,0,5,5);
    UF f(0, -10);
    s.particles.push_back(p);
    s.UGFs.push_back(f);
    s.simulate();
}