#include <iostream>
#include "core/BaseBlock.h"
#include "IO/reader.h"
#include "core/task.h"
void Task1(BaseBlock *block){
    cout<<"task 2:"<<endl;
    Task::ShowFirstBlock(block);
    Task::ShowLastBlock(block);
}
void Task2(BaseBlock *block){
    Task::ShowFirstBlockNear(block);
    Task::ShowLastBlockNear(block);
}
void Task3(BaseBlock *block){
    cout<<"task 3:"<<endl;
    auto v1 = Task::SelectBases(block,101000,23500);
    cout<<1<<endl;
    Task::PrintBases(v1);
    auto v2 = Task::SelectBases(block,18291,26354);
    cout<<2<<endl;
    Task::PrintBases(v2);
    auto v3 = Task::SelectBases(block,85000,28000);
    cout<<3<<endl;
    Task::PrintBases(v3);
}

void Task4(BaseBlock *block){
    cout<<"task 4:"<<endl;
    auto v4 = block->GetEffBase(3000,36900);
    cout<<1<<endl;
    Task::PrintBases(v4);
    auto v5 = block->GetEffBase(70000,35000);
    cout<<2<<endl;
    Task::PrintBases(v5);
    auto v6 = block->GetEffBase(100000,7002);
    cout<<3<<endl;
    Task::PrintBases(v6);
}
void Task5(Move *m){
    cout<<"task 5:"<<endl;
    m->GoForward();
}
void Task6(Move *m){
    cout<<"task 6:"<<endl;
    cout<<1<<endl;
    m->FindFirstEdge(0.1);
    cout<<2<<endl;
    m->FindCovers(0.1);
}
void Task7(FakeBaseManager *f){
    cout<<"task 7:"<<endl;
    cout<<1<<endl;
    f->GoForwardAtSeg(11);
    cout<<2<<endl;
    f->GoForwardAtSeg(8);
}
int main() {
    vector<string> path = {"../data/jz001.txt","../data/jz002.txt"};
    auto data = Reader::ReadBases(path);
    BaseBlock *block = new BaseBlock(data);
    //block->PrintDist();
    cout<<block->start.first<<' '<<block->start.second<<endl;
    printf("width:%d height:%d size:%d\n",block->width, block->height, block->size);
    auto seg = Reader::ReadMoveSeg("../data/yd001.txt");
    Move *m = new Move(seg,block);

    auto fakes = Reader::ReadFakeBases("../data/wz001.txt");
    FakeBaseManager *f = new FakeBaseManager(fakes, seg);

    //Task1(block);
    //Task2(block);
    //Task3(block);
    //Task4(block);
    //Task5(m);
    Task6(m);
    //Task7(f);

    cout<<endl<<"finish";
}
