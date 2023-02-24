//
// Created by KuJyo on 2023/2/18.
//

#include "BaseBlock.h"
#include "math.h"
#define Edge 50
#define BlockSize 1000 //块大小, 恰好为村镇块大小
BaseBlock::BaseBlock(vector<Base *> data) {
    this->size = BlockSize;
    // 处理数据: 额外将高速基站存储起来
    for(auto i:data){
        if(i->type == ROAD){
            RoadBase.push_back(data[data.size()-1-RoadBase.size()]);
        }
    }
    //初始化
    Init(data);
    AssignBlocks(data);
}
void BaseBlock::AssignBlocks(vector<Base*> data) {
    for(auto i:data) {
        //除去高速公路基站
        if (i->type == ROAD){
            RoadBase.push_back(i);
            continue;
        }
        int p = Find(i->x, i->y);
        blocks[p].push_back(i);
    }
}
void BaseBlock::Init(vector<Base*> data) {
    //中心点
    origin.first = 0;
    origin.second = 0;
    //确定范围
    int left=100000000, right=-100000000,
        top=-100000000, down=100000000; //四个方向的边框
    for(auto d:data){
        origin.first += d->x;
        origin.second += d->y;
        left = min(d->x, left);
        right = max(d->x, right);
        top  = max(d->y, top);
        down = min(d->y, down);
    }
    origin.first /= data.size();
    origin.second /= data.size();
    left -= Edge;
    right += Edge;
    top += Edge;
    down -= Edge;
    //分块数据
    width = (int)((right - left) / size) + 1;
    height = (int)((top - down) / size) + 1;
    blocks.resize(width*height);
    start.first = left;
    start.second = top;
}
int BaseBlock::Find(double x, double y) {
    int x_pos = (int)(x - start.first)/size;
    int y_pos = (int)(start.second - y)/size;
    int p =y_pos*width+x_pos;
    return p;
}
void BaseBlock::PrintDist() {
    for(int y=0;y<height;y++){
        for(int x=0;x<width;x++){
            int p = y*width+x;
            printf("%.1d ",blocks[p].size());
        }
        cout<<endl;
    }
    cout<<"blocks:"<<blocks.size()<<endl;
}
vector<Base *> BaseBlock::GetBases(double x, double y) {
    //判断缓存
    if (Find(x, y) == lst_block){
        // 还在上一次的块中, 缓存命中
        return buffer;
    }

    //分别求得周围的基站
    int p = Find(x,y);
    int y_way = p / width;
    int x_way = p % width;
    int bias[9][2] = {{-1,-1},{0,-1},{1,-1},
                    {-1,0},{0,0},{1,0},
                    {-1,1},{0,1},{1,1}};
    vector<Base*> ret;

    // 把周围的基站加入ret中
    for (int i = 0; i < 9; ++i) {
        int x_t = x_way + bias[i][0];
        int y_t = y_way + bias[i][1];
        if (x_t>=0 && x_t<width && y_t>=0 && y_t<height){
            int t = y_t * width + x_t;
            ret.insert(ret.end(),blocks[t].begin(),blocks[t].end());
        }
    }
    return ret;
}
vector<Base*> BaseBlock::GetHighroadBases(double x, double y) {
    vector<Base*> ret;
    for(auto i:RoadBase){
        if (CalcPower(i,x,y)!=0){
            ret.push_back(i);
        }
    }
    return ret;
}
double BaseBlock::CalcPower(Base *b, double x, double y) {
    double r2 = (b->x-x)*(b->x-x)+(b->y-y)*(b->y-y);
    double effect_r2;
    switch (b->type) {
        case CITY:
            effect_r2 = 300*300;
            break;
        case TOWN:
            effect_r2 = 1000*1000;
            break;
        case ROAD:
            effect_r2 = 5000*5000;
            break;
    }
    //超出有效距离返回0
    double power = b->power * effect_r2/r2;
    return power>=1 ? power : 0;
}

double BaseBlock::CalcR2(Base *b, double x, double y) {
    return (b->x-x)*(b->x-x)+(b->y-y)*(b->y-y);
}

Base *BaseBlock::GetPowerfulBase(double x, double y) {
    vector<Base*> bases = GetBases(x,y);
    vector<Base*> high_bases = GetHighroadBases(x,y);
    bases.insert(bases.end(),high_bases.begin(),high_bases.end());

    double power_max = 0;
    int power_pos = -1;
    for(int i = 0;i<bases.size();i++){
        double power = CalcPower(bases[i],x,y);
        if (power!=0){
            if (power > power_max){
                power_pos = i;
                power_max = power;
            }
        }
    }
    return power_pos!=-1 ? bases[power_pos] : nullptr;
}

vector<Base *> BaseBlock::GetEffBase(double x, double y) {
    vector<Base*> bases = GetBases(x,y);
    vector<Base*> high_bases = GetHighroadBases(x,y);
    bases.insert(bases.end(),high_bases.begin(),high_bases.end());

    vector<Base*> ret;
    //计算有效基站
    for(auto i : bases){
        if (CalcPower(i,x,y)!=0){
            ret.push_back(i);
        }
    }
    return ret;
}
