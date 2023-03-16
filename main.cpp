#include <iostream>
#include "src/BaseBlock.h"
#include "src/reader.h"
#include "test/task.h"

using namespace std;
int main() {
    clock_t c = clock();
    // 读取数据
    vector<string> path = {"../data/jz001.txt","../data/jz002.txt"};
    auto data = Reader::ReadBases(path);
    auto *block = new BaseBlock(data);
    auto seg = Reader::ReadMoveSeg("../data/yd001.txt");
    Move *m = new Move(seg,block);
    auto fakes = Reader::ReadFakeBases("../data/wz001.txt");
    auto *f = new FakeBaseManager(fakes, seg);
    // 输出基本信息

    Task::Task12(block);
    Task::Task3(block);
    Task::Task4(block);
    Task::Task5(m);
    Task::Task6(m);
    Task::Task7(f);

    cout<<endl<<"Time: "<<clock()-c<<"ms"<<endl;
}
