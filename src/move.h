#ifndef GMS_NETWORK_MOVE_H
#define GMS_NETWORK_MOVE_H
#include "vector"
#include "BaseBlock.h"
#define INCDis 1 //按距s离步进, 单位m
using namespace std;
typedef struct {
    int sx, sy, ex, ey; //开始结束坐标
    int speed;
    int sth; //开始时间和分钟
    int stm;
}MoveSeg;
class Move {
public:
    vector<MoveSeg> seg; // 移动段
    BaseBlock* MoveData; // 移动数据
    Move(vector<MoveSeg> m, BaseBlock* MoveMap); // 构造函数
    ~Move(); // 析构函数
    void GoForward(); // 粗略移动一遍
    void FindFirstEdge(double e, int seg); // 扩展功能1:寻找进入离开第一个基站的位置, e为误差控制
    void FindFirstCover(double e, int seg); // 扩展功能2:寻找第一个重叠区
    void FindCovers(double e); // 扩展功能2:寻找重叠区
    void GoDis(MoveSeg s, int step);

private:
    // 通过距离增量移动
    long double FindCover(MoveSeg s, double e); // 输出重叠区, 并返回秒数
    pair<long double, long double> BinarySearchForCover(
            pair<long double,long double> l,
            pair<long double,long double> r,
            double e); // 用于寻找重叠区的二分
};


#endif
