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
    static vector<Base*> SelectBases(BaseBlock *data, double x,double y); //选出3组满足条件的基站:最强 距离 唯一
    static void PrintBases(vector<Base*>);
    void ShowFirstBlockNear(BaseBlock *data);
    void ShowLastBlockNear(BaseBlock *data);
private:

};
#endif //GMS_NETWORK_TASK_H
