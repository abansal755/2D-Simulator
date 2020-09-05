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
    int trajectRadius;
    rgb colour;
    particle(float vx = 0, float vy = 0, float x = 0, float y = 0, float q = 0, float m = 0, int radius = 5, int trajectRadius = 5, rgb colour = { 255,255,255 })
        :vx(vx), vy(vy), x(x), y(y), q(q), m(m), radius(radius), colour(colour), trajectRadius(trajectRadius) {};
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
    uniformField(float ex = 0, float ey = 0) :ex(ex), ey(ey) {}
};

class System {
private:
    void updateAccn(particle& p) {
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
            if (&p == &particles[j]) continue;
            particle& p2 = particles[j];
            float cos = (p.x - p2.x) / distance(p.x, p.y, p2.x, p2.y);
            float sin = (p.y - p2.y) / distance(p.x, p.y, p2.x, p2.y);
            float k = 9e9;
            p.ax += ((k * p.q * p2.q) / (pow(p.x - p2.x, 2) + pow(p.y - p2.y, 2)) * p.m) * cos;
            p.ay += ((k * p.q * p2.q) / (pow(p.x - p2.x, 2) + pow(p.y - p2.y, 2)) * p.m) * sin;
        }
        for (int j = 0; j < particles.size(); j++) {
            if (&p == &particles[j]) continue;
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
    void updateBuffer(image*& buffer) {
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
                    buffer->frame[boundY - I - di - 1][J + dj] = p.colour;
                }
            }
        }
        blur(buffer, 3);
        for (int i = 0; i < boundX; i++) {
            buffer->frame[0][i] = { 255,0,0 };
            buffer->frame[boundY - 1][i] = { 255,0,0 };
        }
        for (int i = 0; i < boundY; i++) {
            buffer->frame[i][0] = { 255,0,0 };
            buffer->frame[i][boundX - 1] = { 255,0,0 };
        }
    }
    void updateTraject(int i) {
        particle& p = particles[i];
        int I = p.y / scale;
        int J = p.x / scale;
        for (int di = -p.trajectRadius; di <= p.trajectRadius; di++) {
            for (int dj = -p.trajectRadius; dj <= p.trajectRadius; dj++) {
                if (I + di < 0 || I + di >= boundY) continue;
                if (J + dj < 0 || J + dj >= boundX) continue;
                traject[i]->frame[boundY - I - di - 1][J + dj] = p.colour;
            }
        }
    }
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
    vector<image*> traject;
    System(float scale = 1, int boundX = 100, int boundY = 100, int iterations = 300, int duration = 1, float timeFactor = 1, int padding = 3, float visc_k = 0)
        :scale(scale), boundX(boundX), boundY(boundY), iterations(iterations), duration(duration), timeFactor(timeFactor), padding(padding), visc_k(visc_k) {
        time = 0;
        unitTime = ((float)1 / iterations) * timeFactor;
    }
    void simulate() {
        for (int i = 0; i < particles.size(); i++) traject.push_back(new image(boundX, boundY));
        for (int i = 0; i < particles.size(); i++) updateAccn(particles[i]);

        image* buffer = new image(boundX, boundY);
        int frame = 0;
        updateBuffer(buffer);
        _mkdir(".\\output");
        _mkdir(".\\output\\simulation");
        _mkdir(".\\output\\trajectories");
        _mkdir(".\\output\\converted");
        string fileName = (string)".\\output\\simulation\\" + (string)"simulation_";
        string newFileName = fileName;
        for (int i = 0; i < padding; i++) newFileName += '0';
        newFileName += ".ppm";
        writeFile(buffer, newFileName);
        frame++;

        for (int i = 1; i < iterations * duration; i++) {
            for (int j = 0; j < particles.size(); j++) {
                particle& p = particles[j];
                particle pC = p;
                time += unitTime;
                p.x += pC.vx * unitTime;
                p.y += pC.vy * unitTime;
                p.vx += pC.ax * unitTime;
                p.vy += pC.ay * unitTime;
                updateAccn(particles[j]);
                updateTraject(j);
            }

            if (i % (iterations / 30) == 0) {
                updateBuffer(buffer);
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
        }
        delete buffer;
        cout << endl;
        for (int i = 0; i < traject.size(); i++) {
            blur(traject[i], 2);
            for (int j = 0; j < boundX; j++) {
                traject[i]->frame[0][j] = { 255,0,0 };
                traject[i]->frame[boundY - 1][j] = { 255,0,0 };
            }
            for (int j = 0; j < boundY; j++) {
                traject[i]->frame[j][0] = { 255,0,0 };
                traject[i]->frame[j][boundX - 1] = { 255,0,0 };
            }
            writeFile(traject[i], ".\\output\\trajectories\\particle_" + to_string(i) + (string)".ppm");
            delete traject[i];
        }
    }
};