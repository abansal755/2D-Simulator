using namespace std;
float distance(float x1, float y1, float x2, float y2);
int numOfDigits(int n);

class particle {
public:
    float ax, ay;
    float vx, vy;
    float x, y;
    float q, m;
    int radius; // in px
    particle(float vx = 0, float vy = 0, float x = 0, float y = 0, float q = 0, float m = 0, int radius = 5)
        :vx(vx), vy(vy), x(x), y(y), q(q), m(m), radius(radius) {};
};

class radialField {
public:
    //radial field of the form k/r^2
    float cx, cy; //centre
    float k;
    radialField(float cx = 0, float cy = 0, float k = 0) :cx(cx), cy(cy), k(k) {}
    float magnitude(float x, float y) {
        return k / (pow(x - cx, 2) + pow((y - cy), 2));
    }
};

class uniformField {
public:
    float ex, ey;
    uniformField(float ex, float ey) :ex(ex), ey(ey) {}
};

class System {
public:
    vector<particle> particles;
    vector<radialField> electricFields;
    vector<radialField> gravitationalFields;
    vector<uniformField> uniformElectricFields;
    vector<uniformField> uniformGravitationalFields;
    float scale; //1px corresponds to scale meters
    float time;
    int duration; //num of seconds to simulate
    int iterations; //per sec and must be a multiple of 30(fps)
    int boundX, boundY; //in px
    float unitTime; // 1/iterations;
    float timeFactor; //1-real time
    int padding;
    float visc_k;
    System(float scale = 1, int boundX = 100, int boundY = 100, int iterations = 300, int duration = 1, float timeFactor = 1, int padding = 3, float visc_k = 0)
        :scale(scale), boundX(boundX), boundY(boundY), iterations(iterations), duration(duration), timeFactor(timeFactor), padding(padding), visc_k(visc_k) {
        time = 0;
        unitTime = ((float)1 / iterations) * timeFactor;
    }
    void simulate() {
        for (int i = 0; i < particles.size(); i++) {
            //set ax and ay intially
            particle& p = particles[i];
            p.ax = 0;
            p.ay = 0;
            for (int j = 0; j < electricFields.size(); j++) {
                radialField& field = electricFields[j];
                float cos = (p.x - field.cx) / distance(p.x, p.y, field.cx, field.cy);
                float sin = (p.y - field.cy) / distance(p.x, p.y, field.cx, field.cy);
                p.ax += (p.q / p.m) * field.magnitude(p.x, p.y) * cos;
                p.ay += (p.q / p.m) * field.magnitude(p.x, p.y) * sin;
            }
            for (int j = 0; j < gravitationalFields.size(); j++) {
                radialField& field = gravitationalFields[j];
                float cos = (p.x - field.cx) / distance(p.x, p.y, field.cx, field.cy);
                float sin = (p.y - field.cy) / distance(p.x, p.y, field.cx, field.cy);
                p.ax += -1 * field.magnitude(p.x, p.y) * cos;
                p.ay += -1 * field.magnitude(p.x, p.y) * sin;
            }
            for (int j = 0; j < uniformElectricFields.size(); j++) {
                uniformField& f = uniformElectricFields[j];
                p.ax += (p.q / p.m) * f.ex;
                p.ay += (p.q / p.m) * f.ey;
            }
            for (int j = 0; j < uniformGravitationalFields.size(); j++) {
                uniformField& f = uniformGravitationalFields[j];
                p.ax += f.ex;
                p.ay += f.ey;
            }
            for (int j = 0; j < particles.size(); j++) {
                if (j == i) continue;
                particle& p2 = particles[j];
                float cos = (p.x - p2.x) / distance(p.x, p.y, p2.x, p2.y);
                float sin = (p.y - p2.y) / distance(p.x, p.y, p2.x, p2.y);
                float k = 9e9;
                p.ax += ((k * p.q * p2.q) / (pow(p.x - p2.x, 2) + pow(p.y - p2.y, 2)) * p.m) * cos;
                p.ay += ((k * p.q * p2.q) / (pow(p.x - p2.x, 2) + pow(p.y - p2.y, 2)) * p.m) * sin;
            }
            for (int j = 0; j < particles.size(); j++) {
                if (j == i) continue;
                particle& p2 = particles[j];
                float cos = (p.x - p2.x) / distance(p.x, p.y, p2.x, p2.y);
                float sin = (p.y - p2.y) / distance(p.x, p.y, p2.x, p2.y);
                float G = 6.67e-11;
                p.ax += -1 * ((G * p2.m) / (pow(p.x - p2.x, 2) + pow(p.y - p2.y, 2))) * cos;
                p.ay += -1 * ((G * p2.m) / (pow(p.x - p2.x, 2) + pow(p.y - p2.y, 2))) * sin;
            }
            //Viscous Force
            p.ax -= (visc_k * p.vx) / p.m;
            p.ay -= (visc_k * p.vy) / p.m;
        }

        image* buffer = new image(boundY, boundX);
        int frame = 0;
        for (int i = 0; i < particles.size(); i++) {
            particle& p = particles[i];
            int I = p.y / scale;
            int J = p.x / scale;
            for (int di = -p.radius; di <= p.radius; di++) {
                for (int dj = -p.radius; dj <= p.radius; dj++) {
                    if (I + di < 0 || I + di >= boundY) continue;
                    if (J + dj < 0 || J + dj >= boundX) continue;
                    buffer->frame[boundY - I - di - 1][J + dj] = { 255,255,255 };
                }
            }
        }

        blur(buffer, 3);
        for (int i = 0; i < boundX; i++) buffer->frame[0][i] = { 255,0,0 };
        for (int i = 0; i < boundX; i++) buffer->frame[boundY - 1][i] = { 255,0,0 };
        for (int i = 0; i < boundY; i++) buffer->frame[i][0] = { 255,0,0 };
        for (int i = 0; i < boundY; i++) buffer->frame[i][boundX - 1] = { 255,0,0 };
        _mkdir(".\\output");
        string fileName = (string)".\\output\\" + (string)"output_";
        string newFileName = fileName;
        for (int i = 0; i < padding; i++) newFileName += '0';
        newFileName += ".ppm";
        writeFile(buffer, newFileName);
        frame++;

        for (int i = 1; i < iterations * duration; i++) {
            if (i % (iterations / 30) == 0) {
                for (int j = 0; j < boundY; j++) {
                    for (int k = 0; k < boundX; k++) {
                        buffer->frame[j][k] = { 0,0,0 };
                    }
                }
                for (int j = 0; j < particles.size(); j++) {
                    particle& p = particles[j];
                    int I = p.y / scale;
                    int J = p.x / scale;
                    for (int di = -p.radius; di <= p.radius; di++) {
                        for (int dj = -p.radius; dj <= p.radius; dj++) {
                            if (I + di < 0 || I + di >= boundY) continue;
                            if (J + dj < 0 || J + dj >= boundX) continue;
                            buffer->frame[boundY - I - di - 1][J + dj] = { 255,255,255 };
                        }
                    }
                }
                blur(buffer, 3);
                for (int i = 0; i < boundX; i++) buffer->frame[0][i] = { 255,0,0 };
                for (int i = 0; i < boundX; i++) buffer->frame[boundY - 1][i] = { 255,0,0 };
                for (int i = 0; i < boundY; i++) buffer->frame[i][0] = { 255,0,0 };
                for (int i = 0; i < boundY; i++) buffer->frame[i][boundX - 1] = { 255,0,0 };
                int numDigits = numOfDigits(frame);
                if (numDigits >= padding) writeFile(buffer, fileName + to_string(frame) + ".ppm");
                else {
                    string newFileName = fileName;
                    for (int j = 0; j < padding - numDigits; j++) newFileName += '0';
                    newFileName += to_string(frame) + ".ppm";
                    writeFile(buffer, newFileName);
                }
                frame++;
            }
            for (int j = 0; j < particles.size(); j++) {
                particle& p = particles[j];
                particle pC = p;

                time += unitTime;
                p.x += pC.vx * unitTime;
                p.y += pC.vy * unitTime;
                p.vx += pC.ax * unitTime;
                p.vy += pC.ay * unitTime;
                p.ax = 0;
                p.ay = 0;
                for (int k = 0; k < electricFields.size(); k++) {
                    radialField& field = electricFields[k];
                    float cos = (pC.x - field.cx) / distance(pC.x, pC.y, field.cx, field.cy);
                    float sin = (pC.y - field.cy) / distance(pC.x, pC.y, field.cx, field.cy);
                    p.ax += (pC.q / pC.m) * field.magnitude(pC.x, pC.y) * cos;
                    p.ay += (pC.q / pC.m) * field.magnitude(pC.x, pC.y) * sin;
                }
                for (int k = 0; k < gravitationalFields.size(); k++) {
                    radialField& field = gravitationalFields[k];
                    float cos = (pC.x - field.cx) / distance(pC.x, pC.y, field.cx, field.cy);
                    float sin = (pC.y - field.cy) / distance(pC.x, pC.y, field.cx, field.cy);
                    p.ax += -1 * field.magnitude(pC.x, pC.y) * cos;
                    p.ay += -1 * field.magnitude(pC.x, pC.y) * sin;
                }
                for (int k = 0; k < uniformElectricFields.size(); k++) {
                    uniformField& f = uniformElectricFields[k];
                    p.ax += (p.q / p.m) * f.ex;
                    p.ay += (p.q / p.m) * f.ey;
                }
                for (int k = 0; k < uniformGravitationalFields.size(); k++) {
                    uniformField& f = uniformGravitationalFields[k];
                    p.ax += f.ex;
                    p.ay += f.ey;
                }
                for (int l = 0; l < particles.size(); l++) {
                    if (l == j) continue;
                    particle& p2 = particles[l];
                    float cos = (p.x - p2.x) / distance(p.x, p.y, p2.x, p2.y);
                    float sin = (p.y - p2.y) / distance(p.x, p.y, p2.x, p2.y);
                    float k = 9e9;
                    p.ax += ((k * p.q * p2.q) / (pow(p.x - p2.x, 2) + pow(p.y - p2.y, 2)) * p.m) * cos;
                    p.ay += ((k * p.q * p2.q) / (pow(p.x - p2.x, 2) + pow(p.y - p2.y, 2)) * p.m) * sin;
                }
                for (int l = 0; l < particles.size(); l++) {
                    if (l == j) continue;
                    particle& p2 = particles[l];
                    float cos = (p.x - p2.x) / distance(p.x, p.y, p2.x, p2.y);
                    float sin = (p.y - p2.y) / distance(p.x, p.y, p2.x, p2.y);
                    float G = 6.67e-11;
                    p.ax += -1 * ((G * p2.m) / (pow(p.x - p2.x, 2) + pow(p.y - p2.y, 2))) * cos;
                    p.ay += -1 * ((G * p2.m) / (pow(p.x - p2.x, 2) + pow(p.y - p2.y, 2))) * sin;
                }
                //Viscous Force
                p.ax -= (visc_k * p.vx) / p.m;
                p.ay -= (visc_k * p.vy) / p.m;
            }
        }
        delete buffer;
    }
};