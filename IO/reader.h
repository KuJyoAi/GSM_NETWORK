//
// 文件读取类
//

#ifndef GMS_NETWORK_READER_H
#define GMS_NETWORK_READER_H

#include "../core/base.h"
#include "../core/move.h"
#include "../core/FakeBase.h"
#include "string"


class Reader {
public :
    static vector<Base*> ReadBases(string path); //读取基站数据
    static vector<MoveSeg> ReadMoveSeg(string path); //读取移动轨迹
    static vector<FakeBase> ReadFakeBases(string path); //读取伪基站
};


#endif //GMS_NETWORK_READER_H
