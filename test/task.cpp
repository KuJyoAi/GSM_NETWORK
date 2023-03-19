//
// 任务点函数
//
#include <cmath>
#include "task.h"
using namespace std;


void Task::PrintBases(const vector<Base *>& b) {
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



void Task::Task12(BaseBlock *block){
    cout<<"------------------------------------------------"<<endl;
    // 39行，0列：2093,2094,2140,2141,50248,
    // 用循环打印以第一个分块为左上角的九个分块
    int bias[9][2] = {{0,0},{0,1},{0,2},{1,0},{1,1},{1,2},{2,0},{2,1},{2,2}};
    for (int i = 0;i<9;i++){
        int row = bias[i][0];
        int col = bias[i][1];
        double x = block->start.first + col*block->size;
        double y = block->start.second - row*block->size;
        cout<<row<<" row, "<<col<<" col("<<x<<","<<y<<"):";
        Task::ShowBlockByPos(block,row,col);
        cout<<endl;
    }
    // 用循环打印以最后一个分块为右下角的9个分块
    int bias_[9][2] = {{0,0},{0,-1},{0,-2},{-1,0},{-1,-1},{-1,-2},{-2,0},{-2,-1},{-2,-2}};
    for (int i = 0;i<9;i++){
        int row = block->height - 1 + bias_[i][0];
        int col = block->width - 1 + bias_[i][1];
        double x = block->start.first + col*block->size;
        double y = block->start.second - row*block->size;
        cout<<row<<" row, "<<col<<" col("<<x<<","<<y<<"):";
        Task::ShowBlockByPos(block,row,col);
        cout<<endl;
    }
}

void Task::Task4(BaseBlock *block){
    cout<<"------------------------------------------------"<<endl;
    // 获取最强信号的基站
    auto v1 = block->GetPowerfulBase(3000,36900);
    // num:2378,x=100947,y=23231,xh=0.959000,dist=274,等效强度1.150
    if (v1 == nullptr){
        ::printf("(3000,36900): No base station found!\n");
    }else{
        ::printf("(3000,36900): num:%d,x=%d,y=%d,xh=%lf,dist=%lf,power=%lf\n",
                 v1->num,v1->x,v1->y,v1->power,
                 sqrt(block->CalcR2(v1,3000,36900)),
                 block->CalcPower(v1,3000,36900));
    }

    // 获取最近的基站
    auto v2 = block->GetPowerfulBase(70000,35000);
    if (v2 == nullptr){
        ::printf("(70000,35000): No base station found!\n");
    }else{
        ::printf("(70000,35000): num:%d,x=%d,y=%d,xh=%lf,dist=%lf,power=%lf\n",
                 v2->num,v2->x,v2->y,v2->power,
                 sqrt(block->CalcR2(v2,70000,35000)),
                 block->CalcPower(v2,70000,35000));
    }

    // 获取最强信号的基站
    auto v3 = block->GetPowerfulBase(100000,7002);
    if (v3 == nullptr){
        ::printf("(100000,7002): No base station found!");
    }else{
        ::printf("(100000,7002): num:%d,x=%d,y=%d,xh=%lf,dist=%lf,power=%lf\n",
                 v3->num,v3->x,v3->y,v3->power,
                 sqrt(block->CalcR2(v3,100000,7002)),
                 block->CalcPower(v3,100000,7002));
    }
}

void Task::Task3(BaseBlock *block){
    cout<<"------------------------------------------------"<<endl;
    // 获取最强信号的基站
    auto v1 = block->GetPowerfulBase(101000,23500);
    // num:2378,x=100947,y=23231,xh=0.959000,dist=274,等效强度1.150
    ::printf("powerful: num:%d,x=%d,y=%d,xh=%lf,dist=%lf,power=%lf\n",
             v1->num,v1->x,v1->y,v1->power,
             sqrt(block->CalcR2(v1,101000,23500)),
             block->CalcPower(v1,101000,23500));

    // 获取最近的基站
    auto v2 = Task::SelectBases(block,18291,26354)[1];
    ::printf("nearest: num:%d,x=%d,y=%d,xh=%lf,dist=%lf,power=%lf\n",
             v2->num,v2->x,v2->y,v2->power,
             sqrt(block->CalcR2(v2,18291,26354)),
             block->CalcPower(v2,18291,26354));

    // 获取最强信号的基站
    auto v3 =Task::SelectBases(block,85000,28000)[2];
    ::printf("unique: num:%d,x=%d,y=%d,xh=%lf,dist=%lf,power=%lf\n",
             v3->num,v3->x,v3->y,v3->power,
             sqrt(block->CalcR2(v3,85000,28000)),block->CalcPower(v3,85000,28000));
}

void Task::Task5(Move *m){
    cout<<"------------------------------------------------"<<endl;
    m->GoForward();
}

void Task::Task6(Move *m){
    // 第一段
    cout<<"------------------------------------------------"<<endl;
    cout<<"task 6 Extend 1:"<<endl;
    m->FindFirstEdge(0.1, 0);
    cout<<endl<<"------------------------------------------------"<<endl;
    cout<<"task 6 Extend 2:"<<endl;
    // 第三段, 第六段
    m->FindFirstCover(0.1, 2);
    cout<<endl<<"------------------------------------------------"<<endl;

    cout<<"task 6 Extend 3:"<<endl;
    m->FindFirstCover(0.1, 5);
    cout<<endl<<"------------------------------------------------"<<endl;
}

void Task::Task7(FakeBaseManager *f){
    cout<<"------------------------------------------------"<<endl;
    cout<<"task 7-1:"<<endl;
    f->GoForwardAtSeg(11);
    cout<<"------------------------------------------------"<<endl;
    cout<<"task 7-2:"<<endl;
    f->GoForwardAtSeg(8);
}

void Task::ShowBlockByPos(BaseBlock *data, int x, int y) {
    // 39行，0列：2093,2094,2140,2141,50248,
    // 根据给出的块的坐标, 显示该坐标所在的分块的基站
    const auto blocks = data->blocks;
    int p = x*data->width + y;
    //获取该坐标所在的分块
    const auto& block = blocks[p];
    // 打印所有基站的编号
    Task::PrintBaseNumber(block);
}

void Task::PrintBaseNumber(const vector<Base *>& b) {
    // 输出基站的编号
    for(auto &i:b){
        if (i == nullptr) continue;
        cout<<i->num<<",";
    }
}

void Task::ITask1(BaseBlock *b, int x, int y) {
    // 39行，0列：2093,2094,2140,2141,50248,
    // 根据给出的块的坐标, 显示该坐标所在的分块的基站
    const auto blocks = b->blocks;
    int p = x*b->width + y;
    //获取该坐标所在的分块
    const auto& block = blocks[p];
    // 打印所有基站的编号
    Task::ShowBlockByPos(b, x, y);
}

void Task::ITask2(BaseBlock *b, int x, int y) {
    // 所有有效的基站
    cout<<"附近的有效基站:"<<endl;
    const auto& bases = b->GetEffBase(x,y);
    for (auto &i:bases){
        ::printf("num:%d,x=%d,y=%d,xh=%lf,dist=%lf,power=%lf\n",
                 i->num,i->x,i->y,i->power,
                 sqrt(b->CalcR2(i,x,y)),
                 b->CalcPower(i,x,y));
    }
    cout<<endl<<"------------------------------------------------"<<endl;
    auto v1 = b->GetPowerfulBase(x,y);
    if (v1 == nullptr){
        ::printf("最强: (%d,%d): No base station found!\n",x,y);
    } else{
        // num:2378,x=100947,y=23231,xh=0.959000,dist=274,等效强度1.150
        ::printf("信号最强: num:%d,x=%d,y=%d,xh=%lf,dist=%lf,power=%lf\n",
                 v1->num,v1->x,v1->y,v1->power,
                 sqrt(b->CalcR2(v1,x,y)),
                 b->CalcPower(v1,x,y));
    }

    // 获取最近的基站
    auto v2_ = Task::GetNearestBase(b,x,y);
    if (v2_ == nullptr){
        ::printf("最近: (%d,%d): No base station found!\n",x,y);
    } else{
        auto v2 = v2_;
        ::printf("最近: num:%d,x=%d,y=%d,xh=%lf,dist=%lf,power=%lf\n",
                 v2->num,v2->x,v2->y,v2->power,
                 sqrt(b->CalcR2(v2,x,y)),
                 b->CalcPower(v2,x,y));
    }

}

void Task::ITask3(Move *m, int s) {
    m->GoDis(m->seg[s], 1);
}

void Task::ITask4(Move *m, int s) {
    m->FindFirstEdge(0.1, s);
}

void Task::ITask5(Move *m, int s) {
    m->FindFirstCover(0.1, s);
}

void Task::ITask6(FakeBaseManager *f, int s) {
    f->GoForwardAtSeg(s);
}

Base *Task::GetNearestBase(BaseBlock *data, double x, double y) {
    // 获取最近的基站
    auto v = data->GetEffBase(x,y);
    if (v.empty()){
        return nullptr;
    }else{
        long double min = 1e9;
        Base *ans = nullptr;
        for(auto &i:v){
            long double dist = sqrt(data->CalcR2(i,x,y));
            if (dist < min){
                min = dist;
                ans = i;
            }
        }
        return ans;
    }

}

