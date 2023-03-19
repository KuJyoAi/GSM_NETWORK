#include <iostream>
#include "src/BaseBlock.h"
#include "src/reader.h"
#include "test/task.h"

using namespace std;
// 测试模式
void TestMode(){
    cout<<"System Start!"<<endl;
    clock_t c = clock();
    clock_t total = c;
    // 读取数据
    vector<string> path = {"../data/jz001.txt","../data/jz002.txt"};
    auto data = Reader::ReadBases(path);
    cout<<"jz001.txt, jz002.txt loaded"<<endl;
    auto *block = new BaseBlock(data);
    auto seg = Reader::ReadMoveSeg("../data/yd001.txt");
    cout<<"yd001.txt loaded"<<endl;
    Move *m = new Move(seg,block);
    auto fakes = Reader::ReadFakeBases("../data/wz001.txt");
    cout<<"wz001.txt loaded, Time: "<<clock()-c<<"ms"<<endl;
    auto *f = new FakeBaseManager(fakes, seg);
    // 输出基本信息
    c = clock();
    Task::Task12(block);
    cout<<endl<<"Task12 Complete, Time: "<<clock()-c<<"ms"<<endl;
    c = clock();
    Task::Task3(block);
    cout<<endl<<"Task3 Complete, Time: "<<clock()-c<<"ms"<<endl;
    c = clock();
    Task::Task4(block);
    cout<<endl<<"Task4 Complete, Time: "<<clock()-c<<"ms"<<endl;
    c = clock();
    Task::Task5(m);
    cout<<endl<<"Task5 Complete, Time: "<<clock()-c<<"ms"<<endl;
    c = clock();
    Task::Task6(m);
    cout<<endl<<"Task6 Complete, Time: "<<clock()-c<<"ms"<<endl;
    c = clock();
    Task::Task7(f);
    cout<<endl<<"Task7 Complete, Time: "<<clock()-c<<"ms"<<endl;
    cout<<endl<<"Total Time: "<<clock()-total<<"ms"<<endl;
}
// 交互模式
void InteractiveMode(){
    cout<<"交互模式:"<<endl;
    string path = "../data";
    // 先读取默认路径下的数据, 如果没有就要求输入路径
    ::FILE *fp = fopen("../data/jz001.txt","r");
    if(fp == nullptr){
        // 读取失败
        cout<<"输入路径:"<<endl;
        cin>>path;
    }else{
        // 读取成功
        cout<<"已在默认路径下找到数据: "<<path<<endl;
        fclose(fp);
    }


    vector<string> BasePath = {path+"/jz001.txt",path+"/jz002.txt"};
    auto data = Reader::ReadBases(BasePath);
    cout<<"jz001.txt, jz002.txt 读取成功"<<endl;
    auto b = new BaseBlock(data);
    auto seg = Reader::ReadMoveSeg(path+"/yd001.txt");
    cout<<"yd001.txt 读取成功"<<endl;
    Move *m = new Move(seg,b);
    auto fakes = Reader::ReadFakeBases(path+"/wz001.txt");
    cout<<"wz001.txt 读取成功"<<endl;
    auto *f = new FakeBaseManager(fakes, seg);

    // 输入任务号
    cout<<"选择任务:"<<endl;
    cout<<"1. 给定分块的坐标x, y, 得到对应分块的基站"<<endl;
    cout<<"2. 给定位置x, y, 得到对应位置的信号最强的或最近的基站"<<endl;
    cout<<"3. 选定移动轨迹s(从0开始计数), 输出基站切换情况"<<endl;
    cout<<"4. 输入移动轨迹s, 输出这段轨迹开始阶段进出基站的时间和位置"<<endl;
    cout<<"5. 输入移动轨迹s, 输出这段轨迹进出重叠区的时间和位置"<<endl;
    cout<<"6. 输入移动轨迹s, 输出这段轨迹连上伪基站的时间和位置"<<endl;
    int task;
    cin>>task;
    switch (task){
        case 1:
            cout<<"输入分块坐标x y:"<<endl;
            int x,y;
            cin>>x>>y;
            Task::ITask1(b,x,y);
            break;
        case 2:
            cout<<"输入位置坐标x y(整数):"<<endl;
            int x1,y1;
            cin>>x1>>y1;
            Task::ITask2(b,x1,y1);
            break;
        case 3:
            cout<<"输入移动轨迹s:"<<endl;
            int s;
            cin>>s;
            if (s<=0||s>=m->seg.size()){
                cout<<"Wrong Input!"<<endl;
                break;
            }
            Task::ITask3(m,s);
            break;
        case 4:
            cout<<"输入移动轨迹s:"<<endl;
            int s1;
            cin>>s1;
            if (s1<=0||s1>=m->seg.size()){
                cout<<"Wrong Input!"<<endl;
                break;
            }
            Task::ITask4(m,s1);
            break;
        case 5:
            cout<<"输入移动轨迹s:"<<endl;
            int s2;
            cin>>s2;
            if (s2<=0||s2>=m->seg.size()){
                cout<<"Wrong Input!"<<endl;
                break;
            }
            Task::ITask5(m,s2);
            break;
        case 6:
            cout<<"输入移动轨迹s:"<<endl;
            int s3;
            cin>>s3;
            if (s3<=0||s3>=m->seg.size()){
                cout<<"Wrong Input!"<<endl;
                break;
            }
            Task::ITask6(f,s3);
            break;
        default:
            cout<<"Wrong Input!"<<endl;
            break;
    }
    cout<<endl<<"任务完成!"<<endl;
    cout<<"Press any key to exit"<<endl;
    ::getchar();
    ::getchar();
}
int main() {
    system("chcp 65001");
    InteractiveMode();
    /*cout<<"选择模式:"<<endl;
    cout<<"1. 任务单模式: 直接按任务单的测试样例输出"<<endl;
    cout<<"2. 交互模式: 自定义输入输出"<<endl;
    int mode;
    cin>>mode;
    switch (mode){
        case 1:
            TestMode();
            break;
        case 2:
            InteractiveMode();
            break;
        default:
            cout<<"Wrong Input!"<<endl;
            break;
    }*/
}
