//
// 文件读取类
//

#ifndef GMS_NETWORK_READER_H
#define GMS_NETWORK_READER_H

#include "../core/base.h"
#include "../core/move.h"
#include "string"


class Reader {
public :
    static vector<Base*> ReadBases(std::string path); //读取基站数据
    static vector<MoveSeg> ReadMoveSeg(string path);
};


#endif //GMS_NETWORK_READER_H
