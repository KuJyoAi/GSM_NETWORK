#include "reader.h"
#include "iostream"

using namespace std;
vector<Base*> Reader::ReadBases(vector<string> path) {
    vector<Base*> ret;
    for (auto p:path) {
        FILE* f = fopen(p.c_str(), "r");
        // 除去第一行
        while (true){
            char c = fgetc(f);
            if(c=='\n'){
                break;
            }
        }
        while(true){
            char buf[10]={0};
            Base *b = new Base();
            fscanf(f,"%d,%d,%s  %lf,%d",&b->x,&b->y, buf,&b->power,&b->num);
            if (b->num==0){
                break;
            }
            switch (buf[0]) {
                case -77:
                    b->type = CITY;
                    break;
                case -49:
                    b->type = TOWN;
                    break;
                default:
                    b->type = ROAD;
            }
            ret.push_back(b);
        }
    }
    return ret;
}

vector<MoveSeg> Reader::ReadMoveSeg(string path) {
    FILE* f = fopen(path.c_str(), "r");
    // 除去第一行
    while (true){
        char c = fgetc(f);
        if(c=='\n'){
            break;
        }
    }
    vector<MoveSeg> ret;
    while (true){
        int sx = -1, sy, ex, ey, speed, sth, stm;
        fscanf(f,"%d,%d,%d,%d,%d,%d,%d",
               &sx,&sy,&ex,&ey,&speed,&sth,&stm);
        if (sx == -1) break;
        ret.push_back(MoveSeg{sx, sy, ex, ey, speed, sth, stm});
    }
    return ret;
}

vector<FakeBase> Reader::ReadFakeBases(string path) {
    FILE* f = fopen(path.c_str(), "r");
    // 除去第一行
    while (true){
        char c = fgetc(f);
        if(c=='\n'){
            break;
        }
    }
    vector<FakeBase> ret;
    while (true){
        int sx = -1, sy, ex, ey, speed, sth, stm, num;
        fscanf(f,"%d,%d,%d,%d,%d,%d,%d,%d",
               &sx,&sy,&ex,&ey,&speed,&sth,&stm,&num);
        if (sx == -1) break;
        auto fb = FakeBase {sx, sy, ex, ey, speed, sth, stm, num};
        ret.push_back(fb);
    }
    return ret;
}

