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

int minlen = -1;
void rec(pos xy, int len)
{
    if (!(xy.x < 0 || xy.x >= sizeX ||
        xy.y < 0 || xy.y >= sizeY ||
        gDone[inedx(xy.x, xy.y)] > -1 &&
        gDone[inedx(xy.x, xy.y)] <= len)) {

        if (gMap[inedx(xy.x, xy.y)] == 'E') {
            gDone[inedx(xy.x, xy.y)] = len;
            for (int i = 0; i < 8; i++)
            {
                rec({xy.x + star[i][0], xy.y + star[i][1]}, len + 1);
                //xy = {xy.x - star[i][0], xy.y - star[i][1]};
            }
        } else if (gMap[inedx(xy.x, xy.y)] == 'G') {

            if (minlen == -1)
                minlen = len;
            else if (minlen > len)
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
        if ((xy.x < 0 || xy.x >= sizeX ||
            xy.y < 0 || xy.y >= sizeY ||
            gDone[inedx(xy.x, xy.y)] > -1 &&
            gDone[inedx(xy.x, xy.y)] <= len) &&
            mode[len] == 0 || mode[len] >= 8) {
        } else if (gMap[inedx(xy.x, xy.y)] == 'E') {
            gDone[inedx(xy.x, xy.y)] = len;
            int i = mode[len]++;
            stack[len] = xy;
            xy = {xy.x + star[i][0], xy.y + star[i][1]};

            // push
            len++;
            mode[len] = 0;
            continue;

        } else if (gMap[inedx(xy.x, xy.y)] == 'G') {

            if (minlen == -1)
                minlen = len;
            else if (minlen > len)
                minlen = len;
            gDone[inedx(xy.x, xy.y)] = -99;
        }

        len--;
        if (len >= 0)
            xy = stack[len];
    }

}

typedef const function<void()> timerHandler;
void timing(const string &display, timerHandler doing)
{
    clock_t t = clock();
    doing();
    cout << display << (clock() - t) << " ms" << endl;
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

    int time = 500;
    // recursive
    cout << "recursive: " << endl;
    for (int i = 0; i < time; i++) {
        minlen = -1;
        fill(gDone, gDone + sizeX * sizeY, -1);

        timing("", [&]() { rec({0, 0}, 0); });
    }
    cout << "min: "<< minlen << endl;
    for (int y = 0; y < sizeY; y++){
        for (int x = 0; x < sizeX; x++)
            cout << setw(3) << done[x][y];
        cout << endl;
    }

    // loop
    cout << "loop: " << endl;
    for (int i = 0; i < time; i++) {
        minlen = -1;
        fill(gDone, gDone + sizeX * sizeY, -1);

        timing("", [&]() { loop(); });
    }
    cout << "min: "<< minlen << endl;
    for (int y = 0; y < sizeY; y++){
        for (int x = 0; x < sizeX; x++)
            cout << setw(3) << done[x][y];
        cout << endl;
    }
}

