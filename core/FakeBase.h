//
// 伪基站类
//

#ifndef GMS_NETWORK_FAKEBASE_H
#define GMS_NETWORK_FAKEBASE_H
#include "vector"
#include "move.h"
#define FakeBaseEffi 40
#define BinaryE 0.01
using namespace std;

typedef struct {
    int sx, sy, ex, ey;
    int speed;
    int sth, stm;
    int num;
}FakeBase;

typedef struct {
    vector<long double> fx, fy, fdx, fdy; //伪基站状态
    long double x, y, dx, dy; //终端状态
    unsigned long long cur_t; //当前周期
    int num; //此时连上的伪基站
}State; //表示当前基站状态的结构体

class FakeBaseManager {
public:
    FakeBaseManager(vector<FakeBase>, vector<MoveSeg>);
    void GoForwardAtSeg(int seg); //计算指定标号的移动轨迹
private:
    FakeBase* ConnectFakeBases(State &s); // 根据状态判断是否连上伪基站

    static void MoveOneStep(State &s); //让状态前进一个周期

    //二分搜索, 将保存的状态更加精确, e为误差限, 会返回对应状态的时间
    void BinarySearch(vector<State> &s, double e);

    void FakeBaseTimeOffset(State &s, int h, int m);// 对伪基站进行初始化的时间校正

    vector<MoveSeg> moves; //移动轨迹
    vector<FakeBase> fakes; //伪基站信息
};


#endif //GMS_NETWORK_FAKEBASE_H
