#include <iostream>
#include "core/BaseBlock.h"
#include "IO/reader.h"
#include "core/task.h"

int main() {
    auto data = Reader::ReadBases("../data/jz001.txt");
    BaseBlock *block = new BaseBlock(data);
    //Task::ShowFirstBlock(block);
    //Task::ShowLastBlock(block);
    //block->PrintDist();
    //auto v1 = block->GetBases(3000,36900);
    auto v = Task::SelectBases(block,3000,36900);
    //Task::PrintBases(v);
    auto seg = Reader::ReadMoveSeg("../data/yd001.txt");
    Move *m = new Move(seg,block);
    //m->GoForward();
    m->FindCovers(0.1);
    cout<<endl<<"finish";
}
