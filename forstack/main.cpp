#include <iostream>
#include <fstream>
#include <ctime>
#include <functional>
#include <algorithm>
#include <iomanip>
using namespace std;
#define inedx(a ,b) a * sizeY + b

typedef struct pos {
    int x, y;
} pos;

int sizeX, sizeY;

char *gMap;
int *gDone;
int star[8][2] = {
    {0, -1},
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1}
};

#define inside(x, y) \
    x < 0 || x >= sizeX || \
    y < 0 || y >= sizeY


int minlen = -1;
void rec(pos xy, int len)
{
    if (!(inside(xy.x, xy.y) ||
        gDone[inedx(xy.x, xy.y)] > -1 &&
        gDone[inedx(xy.x, xy.y)] <= len ||
        minlen != -1 && minlen < len)) {

        if (gMap[inedx(xy.x, xy.y)] == 'E') {
            gDone[inedx(xy.x, xy.y)] = len;
            for (int i = 0; i < 8; i++)
                rec({xy.x + star[i][0], xy.y + star[i][1]}, len + 1);

        } else if (gMap[inedx(xy.x, xy.y)] == 'G') {
            if (minlen == -1 || minlen > len)
                minlen = len;

            gDone[inedx(xy.x, xy.y)] = -99;
        }
    }
}

void loop()
{
    pos xy = {0, 0};
    pos stack[sizeX * sizeY];
    int mode[sizeX * sizeY];
    int len = 0;
    mode[len] = 0;

    while (len >= 0) {
        if (!((inside(xy.x, xy.y) ||
            gDone[inedx(xy.x, xy.y)] > -1 &&
            gDone[inedx(xy.x, xy.y)] <= len) &&
            mode[len] == 0 || mode[len] >= 8 ||
            minlen != -1 && minlen < len)) {

            if (gMap[inedx(xy.x, xy.y)] == 'E') {
                gDone[inedx(xy.x, xy.y)] = len;
                stack[len] = xy;
                xy = {xy.x + star[mode[len]][0],
                    xy.y + star[mode[len]][1]};

                ++mode[len];

                // push
                len++;
                mode[len] = 0;
                continue;

            } else if (gMap[inedx(xy.x, xy.y)] == 'G') {
                if (minlen == -1 || minlen > len)
                    minlen = len;

                gDone[inedx(xy.x, xy.y)] = -99;
            }
        }
        // pop
        len--;
        if (len >= 0)
            xy = stack[len];
    }

}

typedef const function<void()> timerHandler;
int timing(const string &display, timerHandler doing)
{
    clock_t t = clock();
    doing();
    cout << display << (clock() - t) << " ms" << endl;
    return (clock() - t);
}

int main(int argc, char *argv[]) {
    fstream fin;
    int id;
    cin >> id;
    fin.open("input" + to_string(id) + ".txt", fstream::in);
    fin >> sizeX >> sizeY;

    char map[sizeX][sizeY];
    gMap = &map[0][0];
    for (int y = 0; y < sizeY; y++){
        for (int x = 0; x < sizeX; x++)
            fin >> skipws >> map[x][y];
    }

    int done[sizeX][sizeY];
    gDone = &done[0][0];

    int time = 1000;
    long long avg = 0;
    // recursive
    cout << "recursive: " << endl;
    for (int i = 0; i < time; i++) {
        minlen = -1;
        fill(gDone, gDone + sizeX * sizeY, -1);

        avg += timing("", [&]() { rec({0, 0}, 0); });
    }

    cout << "avg: "<< avg / time << endl;
    cout << "min: "<< minlen << endl;
    for (int y = 0; y < sizeY; y++){
        for (int x = 0; x < sizeX; x++)
            cout << setw(3) << done[x][y];
        cout << endl;
    }

    avg = 0;
    // loop
    cout << "loop: " << endl;
    for (int i = 0; i < time; i++) {
        minlen = -1;
        fill(gDone, gDone + sizeX * sizeY, -1);

        avg += timing("", [&]() { loop(); });
    }

    cout << "avg: "<< avg / time << endl;
    cout << "min: "<< minlen << endl;
    for (int y = 0; y < sizeY; y++){
        for (int x = 0; x < sizeX; x++)
            cout << setw(3) << done[x][y];
        cout << endl;
    }
}

