#include "FakeBase.h"

#include "math.h"
FakeBaseManager::FakeBaseManager(vector<FakeBase> fakes, vector<MoveSeg> seg) {
    this->fakes = fakes;
    this->moves = seg;
}

void FakeBaseManager::GoForwardAtSeg(int seg) {
    // 定义总状态
    State state;
    // 初始化伪基站当前位置, 每周期增量
    for (int i = 0; i < fakes.size(); ++i) {
        const auto &f = fakes[i];
        const auto &speed = moves[seg].speed;
        long double dx = f.ex - f.sx;
        long double dy = f.ey - f.sy;
        long double dr = sqrt(dx*dx+dy*dy);
        // 此处设定终端的移动距离为1, 伪基站按速度配比
        state.fx.push_back(f.sx);
        state.fy.push_back(f.sy);
        state.fdx.push_back(dx / dr * INCDis * (f.speed*1.0/speed));
        state.fdy.push_back(dy / dr * INCDis * (f.speed*1.0/speed));
        state.IsOn.push_back(false);
    }
    // 初始化终端的位置和增量
    const auto &t = moves[seg];
    long double dx = t.ex - t.sx;
    long double dy = t.ey - t.sy;
    long double dr = sqrt(dx*dx+dy*dy);
    state.x = t.sx;
    state.y = t.sy;
    state.dx = dx / dr * INCDis;
    state.dy = dy / dr * INCDis;
    // 根据当前时间修正基站位置和速度(未启动的)
    int Next = FakeBaseTimeOffset(state,t.sth, t.stm);
    // 设定总周期数, 并计算伪基站的启动检查点
    state.cur_t = 0;
    unsigned long long total_t = dr / INCDis;
    unsigned long long check_t;
    if (Next==-1){
        check_t = 0xffffffffff;
    }else{
        // 计算秒数
        check_t =
                ((fakes[Next].sth-moves[seg].sth)*60*60
                +(fakes[Next].stm-moves[seg].stm))*60;
        check_t = (moves[seg].speed*1.0/3.6)*check_t/INCDis;
    }
    // 开始计算
    bool Connect = false; // 是否连上伪基站信号量
    vector<State> standpoint; // 记录出入的时间点, 后面二分计算边界
    if (ConnectFakeBases(state)){
        // 一开始就连上伪基站
        Connect = true;
        standpoint.push_back(state);
    }
    while (state.cur_t<=total_t){
        // 判断是否需要启动伪基站
        if (state.cur_t==check_t){
            // 启动伪基站
            state.IsOn[Next] = true;
        }
        // 前进一步
        MoveOneStep(state);
        // 计算连接情况
        auto fbs = ConnectFakeBases(state);
        if (Connect && !fbs){
            // 离开伪基站
            standpoint.push_back(state);
            Connect = false;
        } else if(!Connect && fbs){
            // 进入伪基站
            standpoint.push_back(state);
            Connect = true;
        }
    }
    // 计算并打印驻点
    BinarySearch(standpoint, BinaryE);
    for(int i=0;i<standpoint.size();i++){
        auto s = standpoint[i];
        if (s.num != -1){
            // 在基站中
            printf("Enter FakeBase %d(%Lf,%Lf): (%Lf,%Lf)\n",
                   fakes[s.num].num, s.fx[s.num], s.fy[s.num], s.x,s.y);
        }else{
            // 计算经过时间
            if (i >= 1){
                // 不是第一个驻点
                auto enter = standpoint[i-1];
                auto leave = standpoint[i];
                long double dx1 = enter.x - fakes[enter.num].sx;
                long double dy1 = enter.y - fakes[enter.num].sy;
                long double dx2 = leave.x - fakes[enter.num].sx;
                long double dy2 = leave.y - fakes[enter.num].sy;
                long double dr1 = sqrt(dx1*dx1+dy1*dy1);
                long double dr2 = sqrt(dx2*dx2+dy2*dy2);
                dx = leave.x - enter.x;
                dy = leave.y - enter.y;
                dr = sqrt(dx*dx+dy*dy);
                long double dt = dr/(t.speed/3.6);
                long double dt1 = dr1/(t.speed/3.6);
                long double dt2 = dr2/(t.speed/3.6);
                printf("Leave FakeBase %d(%Lf,%Lf): (%Lf,%Lf) period=%Lfs\n"
                       "t1=%Lfs \t t2=%Lfs\n",
                       fakes[enter.num].num,leave.fx[enter.num], leave.fy[enter.num],
                       leave.x, leave.y, dt,
                       dt1, dt2);
            }else{
                // 是第一个驻点
                printf("Start Moving, No FakeBase!\n");
            }

        }
    }
}

FakeBase* FakeBaseManager::ConnectFakeBases(State &s) {
    for (int i = 0; i < s.fx.size(); ++i) {
        long double dx = s.fx[i] - s.x;
        long double dy = s.fy[i] - s.y;
        if (dx*dx+dy*dy <= FakeBaseEffi*FakeBaseEffi){
            //连上伪基站, 返回对应基站
            // 判断伪基站是否启动
            if (!s.IsOn[i]){
                continue;
            }
            s.num = i;
            return &fakes[i];
        }
    }
    //没有连上, 返回空, 并设num为-1;
    s.num = -1;
    return nullptr;
}

void FakeBaseManager::MoveOneStep(State &s) {
    for (int i = 0; i < s.fx.size(); ++i) {
        if (!s.IsOn[i]){
            continue;
        }
        s.fx[i] += s.fdx[i];
        s.fy[i] += s.fdy[i];
    }
    s.x += s.dx;
    s.y += s.dy;
    s.cur_t++;
}

void FakeBaseManager::BinarySearch(vector<State> &sp, double e) {
    vector<long double> ret;
    for (int i=0;i<sp.size();i++) {
        auto &s = sp[i];
        if (i == 0 && s.num != -1){
            // 第一个驻点, 且第一个驻点是已经在基站, 就不用迭代
            continue;
        }
        int num = s.num;
        if (num == -1){
            // 如果是出基站, 需要先计算上一个出的基站
            num = sp[i-1].num;
        }
        // 基站数据
        long double lst_fx = s.fx[num] - s.fdx[num];
        long double lst_fy = s.fy[num] - s.fdy[num];
        long double cur_fx = s.fx[num];
        long double cur_fy = s.fy[num];
        long double fdx = s.fdx[num], fdy = s.fdy[num];
        long double fdr = sqrt(fdx*fdx*fdy*fdy);
        // 终端数据
        long double lst_x = s.x - s.dx, lst_y = s.y;
        long double cur_x = s.x, cur_y = s.y;
        long double dx = s.dx, dy = s.dy;
        // 误差指标
        long double dr = sqrt(dx*dx+dy*dy);
        // 连接信号量
        bool Connect = false;
        // 计算之前状态
        if ((lst_x-lst_fx)*(lst_x-lst_fx)+
            (lst_y-lst_fy)*(lst_y-lst_fy) <= FakeBaseEffi*FakeBaseEffi){
            // 连接上
            Connect = true;
        }
        while (dr>=e || fdr >= e){
            fdx = fdx * 0.5;
            fdy = fdy * 0.5;
            fdr = fdr * 0.5;
            dx = dx * 0.5;
            dy = dy * 0.5;
            dr = dr * 0.5;
            long double mfx = lst_fx + fdx;
            long double mfy = lst_fy + fdy;
            long double mx = lst_x + dx;
            long double my = lst_y + dy;
            if (Connect){
                // 连上伪基站到离开伪基站的边界
                if ((mfx-mx)*(mfx-mx)+(mfy-my)*(mfy-my) > FakeBaseEffi*FakeBaseEffi){
                    // 已离开
                    cur_fx = mfx;
                    cur_fy = mfy;
                    cur_x = mx;
                    cur_y = my;
                } else{
                    lst_fx = mfx;
                    lst_fy = mfy;
                    lst_x = mx;
                    lst_y = my;
                }
            }else{
                // 未连上伪基站到进入伪基站的边界
                if ((mfx-mx)*(mfx-mx)+(mfy-my)*(mfy-my) <= FakeBaseEffi*FakeBaseEffi){
                    cur_fx = mfx;
                    cur_fy = mfy;
                    cur_x = mx;
                    cur_y = my;
                } else{
                    lst_fx = mfx;
                    lst_fy = mfy;
                    lst_x = mx;
                    lst_y = my;
                }
            }
        }
        // 迭代满足要求
        s.fx[num] = (lst_fx+cur_fx)/2;
        s.fy[num] = (lst_fy+cur_fy)/2;
        s.x = cur_x;
        s.y = cur_y;
    }
}

int FakeBaseManager::FakeBaseTimeOffset(State &s, int h, int m) {
    int Next=-1;
    for (int i = 0; i < fakes.size(); ++i) {
        double fh = fakes[i].sth;
        double fm = fakes[i].stm;
        if (h+m/60 >= fh+fm/60){
            // 已经启动的基站, 位置偏移
            long double dr = ((h-fh)*fakes[i].speed + (m-fm)*fakes[i].speed/60)*1000;//总位移
            long double r = sqrt(s.fdx[i]*s.fdx[i]+s.fdy[i]*s.fdy[i]);
            long double dx = (s.fdx[i]/r)*dr;//计算分量
            long double dy = (s.fdy[i]/r)*dr;
            s.fx[i] += dx;
            s.fy[i] += dy;
            s.IsOn[i] = true;
        } else{
            if (Next==-1){
                Next = i;
            }
            s.IsOn[i] = false;
        }
    }
    return Next;
}




