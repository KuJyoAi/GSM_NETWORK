//
// 任务点函数
//
#include "task.h"
using namespace std;
void Task::PrintBases(vector<Base *> b) {
    for(auto i:b){
        if (i == nullptr) continue;
        string t;
        switch (i->type) {
            case CITY:
                t = "CITY";
                break;
            case TOWN:
                t = "TOWN";
                break;
            case ROAD:
                t = "ROAD";
                break;
        }
        printf("%d,%d %s %lf %d\n",i->x,i->y,t.c_str(),i->power,i->num);
    }
}
void Task::ShowFirstBlock(BaseBlock *data){
    cout<<"ShowFirstBlock:"<<endl;
    //cout<<"显示第一个分块数据:"<<endl;
    //cout<<"数据格式:编号:x,y 类型 基准强度"<<endl;
    auto block1 = data->blocks[0];
    if (block1.empty()){
        cout<<"Empty"<<endl;
        return;
    }
    PrintBases(block1);
}

void Task::ShowLastBlock(BaseBlock *data) {
    cout<<"ShowLastBlock:"<<endl;
    auto block1 = data->blocks[data->blocks.size()-1];
    if (block1.empty()){
        cout<<"Empty"<<endl;
        return;
    }
    PrintBases(block1);
}

void Task::ShowLastBlockNear(BaseBlock *data){

}
vector<Base *> Task::SelectBases(BaseBlock *data, double x, double y) {
    vector<Base*> bases = data->GetBases(x,y);
    vector<Base*> high_bases = data->GetHighroadBases(x,y);
    bases.insert(bases.end(),high_bases.begin(),high_bases.end());

    //记录最强信号和最近距离的基站
    double power_max = 0, r2_min = 123456789;
    int power_pos = -1, r2_pos = -1;
    int cnt=0; //记录有几个基站有效
    for(int i = 0;i<bases.size();i++){
        double power = data->CalcPower(bases[i],x,y);
        double r2 = data->CalcR2(bases[i],x,y);
        if (power!=0){
            cnt++;
            if (power > power_max){
                power_pos = i;
                power_max = power;
            }
            if (r2<r2_min){
                r2_pos = i;
                r2_min = r2;
            }
        }
    }
    // 计算基站, 分别按 最强, 最近, 唯一排序
    // 如果只有一个基站, 即唯一
    // 如果不是一个基站, 则唯一为nullptr
    vector<Base*> ret;
    ret.resize(3);
    if (cnt == 1){
        //唯一基站
        ret[0] = nullptr;
        ret[1] = nullptr;
        ret[2] = bases[power_pos];
    }else{
        ret[0] = power_pos!=-1 ? bases[power_pos] : nullptr;
        ret[1] = r2_pos!=-1 ? bases[r2_pos] : nullptr;
        ret[2] = nullptr;
    }
    return ret;
}
