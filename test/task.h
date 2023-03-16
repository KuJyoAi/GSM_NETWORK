//
// 任务类
//
#include "iostream"
#include "../src/BaseBlock.h"
#include "../src/move.h"
#include "../src/FakeBase.h"

#ifndef GMS_NETWORK_TASK_H
#define GMS_NETWORK_TASK_H
class Task{
public:
    static void Task12(BaseBlock *b);
    static void Task3(BaseBlock *b);
    static void Task4(BaseBlock *b);
    static void Task5(Move *m);
    static void Task6(Move *m);
    static void Task7(FakeBaseManager *f);
    // 根据坐标显示基站
    static void ShowBlockByPos(BaseBlock *data, int x, int y);
private:

    // 打印基站编号
    static void PrintBaseNumber(const vector<Base*>&);
    static void ShowFirstBlock(BaseBlock *data);
    static void ShowLastBlock(BaseBlock *data);
    static void ShowFirstBlockNear(BaseBlock *data);
    static void ShowLastBlockNear(BaseBlock *data);
    static vector<Base*> SelectBases(BaseBlock *data, double x,double y); //选出3组满足条件的基站:最强 距离 唯一
    static void PrintBases(const vector<Base*>&);
    static void ShowBlockNear(BaseBlock *data, double x, double y, int pos);
};
#endif //GMS_NETWORK_TASK_H
