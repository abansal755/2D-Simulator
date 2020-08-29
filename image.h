using namespace std;
class hsv {
private:
    void clampHueHelper(hsv& p) {
        if (p.h >= 0 && p.h <= 360) return;
        if (h < 0) {
            p.h += 360;
            clampHueHelper(p);
            return;
        }
        p.h -= 360;
        clampHueHelper(p);
        return;
    }
public:
    short int h;
    float s, v;
    hsv(short int h = 0, float s = 0, float v = 0) :h(h), s(s), v(v) {};
    void clampHue() {
        return clampHueHelper(*this);
    }
};

class rgb {
public:
    unsigned char r, g, b;
    rgb(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0) :r(r), g(g), b(b) {};
    rgb operator+(const rgb& rgb1) {
        int r1 = r, g1 = g, b1 = b;
        r1 += rgb1.r;
        g1 += rgb1.g;
        b1 += rgb1.b;
        //clamp
        if (r1 > 255) r1 = 255;
        if (r1 < 0) r1 = 0;
        if (g1 > 255) g1 = 255;
        if (g1 < 0) g1 = 0;
        if (b1 > 255) b1 = 255;
        if (b1 < 0) b1 = 0;

        rgb rgb2(r1, g1, b1);
        return rgb2;
    }
    void operator+=(const rgb& rgb1) {
        int r1 = r, g1 = g, b1 = b;
        r1 += rgb1.r;
        g1 += rgb1.g;
        b1 += rgb1.b;
        //clamp
        if (r1 > 255) r1 = 255;
        if (r1 < 0) r1 = 0;
        if (g1 > 255) g1 = 255;
        if (g1 < 0) g1 = 0;
        if (b1 > 255) b1 = 255;
        if (b1 < 0) b1 = 0;

        r = r1;
        g = g1;
        b = b1;
    }
    rgb operator-(const rgb& rgb1) {
        int r1 = r, g1 = g, b1 = b;
        r1 -= rgb1.r;
        g1 -= rgb1.g;
        b1 -= rgb1.b;
        //clamp
        if (r1 > 255) r1 = 255;
        if (r1 < 0) r1 = 0;
        if (g1 > 255) g1 = 255;
        if (g1 < 0) g1 = 0;
        if (b1 > 255) b1 = 255;
        if (b1 < 0) b1 = 0;

        rgb rgb2(r1, g1, b1);
        return rgb2;
    }
    void operator-=(const rgb& rgb1) {
        int r1 = r, g1 = g, b1 = b;
        r1 -= rgb1.r;
        g1 -= rgb1.g;
        b1 -= rgb1.b;
        //clamp
        if (r1 > 255) r1 = 255;
        if (r1 < 0) r1 = 0;
        if (g1 > 255) g1 = 255;
        if (g1 < 0) g1 = 0;
        if (b1 > 255) b1 = 255;
        if (b1 < 0) b1 = 0;

        r = r1;
        g = g1;
        b = b1;
    }
    rgb operator*(const rgb& rgb1) {
        int r1 = r * rgb1.r / 255;
        int g1 = g * rgb1.g / 255;
        int b1 = b * rgb1.b / 255;
        //clamp
        if (r1 > 255) r1 = 255;
        if (r1 < 0) r1 = 0;
        if (g1 > 255) g1 = 255;
        if (g1 < 0) g1 = 0;
        if (b1 > 255) b1 = 255;
        if (b1 < 0) b1 = 0;

        rgb rgb2(r1, g1, b1);
        return rgb2;
    }
    void operator*=(const rgb& rgb1) {
        int r1 = r * rgb1.r / 255;
        int g1 = g * rgb1.g / 255;
        int b1 = b * rgb1.b / 255;
        //clamp
        if (r1 > 255) r1 = 255;
        if (r1 < 0) r1 = 0;
        if (g1 > 255) g1 = 255;
        if (g1 < 0) g1 = 0;
        if (b1 > 255) b1 = 255;
        if (b1 < 0) b1 = 0;

        r = r1;
        g = g1;
        b = b1;
    }
};

class image {
public:
    rgb** frame;
    const int width, height;
    image(const int width, const int height) :width(width), height(height) {
        frame = new rgb * [height];
        for (int i = 0; i < height; i++) {
            frame[i] = new rgb[width];
            for (int j = 0; j < width; j++) {
                rgb p;
                frame[i][j] = p;
            }
        }
    }
    image(const image& img) :width(img.width), height(img.height) {
        frame = new rgb * [height];
        for (int i = 0; i < height; i++) {
            frame[i] = new rgb[width];
            for (int j = 0; j < width; j++) {
                frame[i][j] = img.frame[i][j];
            }
        }
    }
    ~image() {
        for (int i = 0; i < height; i++) {
            delete[]frame[i];
        }
        delete[]frame;
    }
};

image* readFile(string fileName = "input.ppm") {
    ifstream ifile(fileName, ios::binary);
    if (!ifile.is_open()) {
        cout << "File unable to open" << endl;
        return NULL;
    }
    string magicNum;
    int width, height, maxValue;
    ifile >> magicNum;
    if (magicNum != "P3" && magicNum != "P6") {
        cout << "Unable to read the file" << endl;
        return NULL;
    }
    ifile >> width >> height >> maxValue;
    if (maxValue != 255) {
        cout << "Unable to read the file" << endl;
        return NULL;
    }
    image* input = new image(width, height);
    if (magicNum == "P3") {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int r, g, b;
                ifile >> r >> g >> b;
                rgb temp(r, g, b);
                input->frame[i][j] = temp;
            }
        }
    }
    else {
        char x;
        ifile.get(x);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                unsigned char r, g, b;
                ifile.get(*(char*)&r);
                ifile.get(*(char*)&g);
                ifile.get(*(char*)&b);
                rgb temp(r, g, b);
                input->frame[i][j] = temp;
            }
        }
    }
    ifile.close();
    cout << "File opened" << endl;
    return input;
}

void writeFile(image* input, string fileName = "output.ppm", char mode = 1) {
    //mode:0-P3
    //mode:1-P6
    ofstream ofile(fileName, ios::binary);
    int width = input->width, height = input->height;
    if (mode == 0) {
        ofile << "P3" << endl << width << " " << height << endl << 255 << endl;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                rgb& p = input->frame[i][j];
                ofile << (int)p.r << " " << (int)p.g << " " << (int)p.b << " ";
            }
        }
    }
    else {
        ofile << "P6" << endl << width << " " << height << endl << 255 << endl;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                rgb& p = input->frame[i][j];
                ofile << p.r << p.g << p.b;
            }
        }
    }
    cout << fileName << " " << "saved" << endl;
    ofile.close();
}

hsv RGBtoHSV(rgb p) {
    hsv p1;

    float rd = ((float)(p.r)) / 255;
    float gd = ((float)(p.g)) / 255;
    float bd = ((float)(p.b)) / 255;
    float cmax = max(rd, max(gd, bd));
    float cmin = min(rd, min(gd, bd));
    float delta = cmax - cmin;

    if (cmax == 0) p1.s = 0;
    else p1.s = delta / cmax;

    if (delta == 0) p1.h = 0;
    else if (cmax == rd) p1.h = 60 * ((int)((gd - bd) / delta) % 6);
    else if (cmax == gd) p1.h = 60 * (((bd - rd) / delta) + 2);
    else p1.h = 60 * (((rd - gd) / delta) + 4);

    p1.v = cmax;

    p1.clampHue();
    return p1;
}

rgb HSVtoRGB(hsv p1) {
    rgb p;

    float c = p1.v * p1.s;
    float x = c * (1 - abs(fmod((float)p1.h / 60, 2) - 1));
    float m = p1.v - c;

    float rd, gd, bd;

    if (p1.h >= 0 && p1.h < 60) {
        rd = c;
        gd = x;
        bd = 0;
    }
    else if (p1.h >= 60 && p1.h < 120) {
        rd = x;
        gd = c;
        bd = 0;
    }
    else if (p1.h >= 120 && p1.h < 180) {
        rd = 0;
        gd = c;
        bd = x;
    }
    else if (p1.h >= 180 && p1.h < 240) {
        rd = 0;
        gd = x;
        bd = c;
    }
    else if (p1.h >= 240 && p1.h < 300) {
        rd = x;
        gd = 0;
        bd = c;
    }
    else {
        rd = c;
        gd = 0;
        bd = x;
    }

    p.r = (rd + m) * 255;
    p.g = (gd + m) * 255;
    p.b = (bd + m) * 255;
    return p;
}
void blur(image*& img, unsigned int radius);
void saturate(image* img, float x);