//
// 任务类
//
#include "iostream"
#include "BaseBlock.h"
#ifndef GMS_NETWORK_TASK_H
#define GMS_NETWORK_TASK_H
class Task{
public:
    static void ShowFirstBlock(BaseBlock *data);
    static void ShowLastBlock(BaseBlock *data);
    static void ShowFirstBlockNear(BaseBlock *data);
    static void ShowLastBlockNear(BaseBlock *data);

    static vector<Base*> SelectBases(BaseBlock *data, double x,double y); //选出3组满足条件的基站:最强 距离 唯一
    static void PrintBases(vector<Base*>);
    static void ShowBlockNear(BaseBlock *data, double x, double y, int pos);

private:

};
#endif //GMS_NETWORK_TASK_H
