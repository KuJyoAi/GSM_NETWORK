//
// Created by KuJyo on 2023/2/18.
//

#ifndef GMS_NETWORK_BASEBLOCK_H
#define GMS_NETWORK_BASEBLOCK_H


#include "base.h"
#include "iostream"
using namespace std;
// 基站组
class BaseBlock{
public:
    BaseBlock(vector<Base*> data);
    double CalcR2(Base* b, double x, double y);
    double CalcPower(Base* b, double x, double y); //计算信号强度
    vector<Base*> GetBases(double x, double y); //获取附近的基站
    vector<Base*> GetHighroadBases(double x, double y); //获取高速公路基站
    Base* GetPowerfulBase(double x, double y); //获取信号最强的基站
    vector<Base*> GetEffBase(double x, double y); //获取有效范围内的基站
    int Find(double x, double y);//根据坐标寻找对应块

    pair<double,double> start;//左上角点
    int size; //块大小
    int width; //块的横向个数
    int height; //块的纵向个数
    pair<double,double> origin; //原点

    vector<vector<Base*>> blocks; //分块存储
    void PrintDist(); //打印查看基站分布数据
private:
    vector<Base*> RoadBase; //高速公路基站
    void Init(vector<Base*> v);
    void AssignBlocks(vector<Base*> v); //初始化分配块

    vector<Base*> buffer; //上一次获取基站的缓存
    int lst_block; //上一次所在块, 用于缓存
};




#endif //GMS_NETWORK_BASEBLOCK_H
