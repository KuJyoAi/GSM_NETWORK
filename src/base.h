#ifndef GMS_NETWORK_BASE_H
#define GMS_NETWORK_BASE_H
#include "vector"
using namespace std;
// 基站类型
#define CITY 0
#define TOWN 1
#define ROAD 2
// 单个基站数据
struct Base {
    int x;
    int y;
    int type;
    double power;
    int num;
    Base();
};

#endif
