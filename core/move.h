//
// 实现移动的类
//

#ifndef GMS_NETWORK_MOVE_H
#define GMS_NETWORK_MOVE_H
#include "vector"
#include "BaseBlock.h"
#define INCTime 1 //每次计算的增量时间, 单位s
#define INCDis 1 //按距离步进, 单位m
using namespace std;
typedef struct {
    int sx, sy, ex, ey; //开始结束坐标
    int speed;
    int sth; //开始时间和分钟
    int stm;
}MoveSeg;
class Move {
public:
    vector<MoveSeg> seg; //移动段
    BaseBlock* MoveData;
    Move(vector<MoveSeg> m, BaseBlock* MoveMap);
    void GoForward(); //粗略移动一遍
    void FindFirstEdge(double e); //扩展功能1:寻找进入离开第一个基站的位置, e为误差控制
    void FindCovers(double e); //扩展功能2:寻找重叠区
private:
    void GoDis(MoveSeg s, int step);//通过距离增量移动
    void GoTime(MoveSeg s); //通过时间增量移动
    long double FindCover(MoveSeg s, double e); //输出重叠区, 并返回秒数
    pair<long double, long double> BinarySearchForCover(
            pair<long double,long double> l,
            pair<long double,long double> r,
            double e); //用于寻找重叠区的二分
};


#endif //GMS_NETWORK_MOVE_H
