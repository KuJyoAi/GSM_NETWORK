#include "move.h"
#include "math.h"
#include "base.h"

Move::Move(vector<MoveSeg> m,BaseBlock* MoveMap) {
    this->seg = m;
    this->MoveData = MoveMap;
}

void Move::GoForward() {
    //GoDis(seg[7],INCDis);
    for(auto s : seg){
        //double r = sqrt((s.ex-s.sx)*(s.ex-s.sx)+(s.ey-s.sy)*(s.ey-s.sy));
        GoDis(s,INCDis);
    }
}

void Move::GoDis(MoveSeg s, int step) {
    // 定义一个"空"Base方便输出
    Base *NoBase = new Base();
    NoBase->num = 0;
    // 计算切换次数
    // 按照step计算x, y方向每个周期的增量
    unsigned long rx2 = (s.ex-s.sx)*(s.ex-s.sx);
    unsigned long ry2 = (s.ey-s.sy)*(s.ey-s.sy);
    long double r = sqrt(rx2+ry2);
    double dx = step*(s.ex-s.sx)/r;
    double dy = step*(s.ey-s.sy)/r;
    // 当前数据
    double cur_x = s.sx, cur_y = s.sy;
    unsigned long long total_t = r/step; //总距离周期
    unsigned long long cur_t = 0;//当前距离周期
    // 开始时的基站
    Base* cur_base = MoveData->GetPowerfulBase(cur_x, cur_y);
    // 无信号
    if (cur_base == nullptr){
        cur_base = NoBase;
        printf("t=%.2d:%.2d:%.2d (%.1lf,%.1lf) signal loss\n"
                ,s.sth,s.stm,0,cur_x,cur_y);
    } else{
        printf("t=%.2d:%.2d:%.2d (%.1lf,%.1lf) start at %d\n"
                ,s.sth,s.stm,0,cur_x,cur_y,cur_base->num);
    }

    while (cur_t <= total_t){
        cur_x += dx;
        cur_y += dy;
        Base* base_t = MoveData->GetPowerfulBase(cur_x, cur_y);
        // 无信号
        if (base_t == nullptr){
            base_t = NoBase;
        }
        if (cur_base != base_t){
            // 切换基站:
            // 计算时间
            double t_d = (cur_t * step)/(s.speed / 3.6);
            unsigned long t = t_d; //经过秒数
            int hour = s.sth + t / 3600;
            int minute = s.stm + (t%3600)/60;
            if (minute>60){
                // 对分钟进行修正
                hour++;
                minute = minute%60;
            }
            double seconds = t%60 + t_d-t;
            // 输出切换信息
            if (base_t == NoBase){
                // 从有切换到无(base_t)
                printf("t=%.2d:%.2d:%.1lf (%4.2lf,%4.2lf) signal loss\n"
                        ,hour,minute,seconds,cur_x,cur_y);
            } else{
                printf("t=%.2d:%.2d:%.1lf (%4.2lf,%4.2lf) switch to %.4d \n",
                       hour, minute, seconds,cur_x,cur_y, base_t->num);
            }
            // 切换基站
            cur_base = base_t;
        }
        cur_t++;
    }
}

void Move::FindCovers(double e) {
    // 第三段和第六段轨迹的重叠区
    long double period;
    period = FindCover(seg[2], e);
    cout<<"period:"<<period<<"s"<<endl;
    period = FindCover(seg[5], e);
    cout<<"period:"<<period<<"s"<<endl;
}

long double Move::FindCover(MoveSeg s, double e) {
    // 按照step计算x, y方向每个周期的增量
    unsigned long rx2 = (s.ex-s.sx)*(s.ex-s.sx);
    unsigned long ry2 = (s.ey-s.sy)*(s.ey-s.sy);
    long double r = sqrt(rx2 + ry2);
    long double dx = INCDis*(s.ex-s.sx)/r;
    long double dy = INCDis*(s.ey-s.sy)/r;
    // 当前数据
    long double cur_x = s.sx, cur_y = s.sy;
    unsigned long long total_t = r/INCDis; //总距离周期
    unsigned long long cur_t = 0;//当前距离周期
    // 标记是否在重叠区
    bool Covered = false;
    auto v = MoveData->GetEffBase(cur_x, cur_y);
    if (v.size()>=2){
        Covered = true;
    }
    // 记录重叠区的出入位置
    vector<pair<long double, long double>> standpoint;
    // 开始时是重叠区, 修正
    if (Covered){
        // 添加一个开始点
        standpoint.push_back(
                pair<long double, long double>{cur_x,cur_y});
    }
    while (cur_t <= total_t){
        cur_x += dx;
        cur_y += dy;
        // 获取附近的基站
        v = MoveData->GetEffBase(cur_x, cur_y);
        if (!Covered && v.size()>=2){
            // 进入重叠区
            // 二分计算
            pair<long double, long double> pos = BinarySearchForCover(
                    pair<long double, long double>{cur_x-dx,cur_y-dy},
                    pair<long double, long double>{cur_x,cur_y},
                    e);
            standpoint.push_back(pos);
            //printf("Enter Cover era:(%Lf,%Lf)\n",pos.first,pos.second);
            Covered = true;
        } else if (Covered && v.size()<2){
            // 记录基站
            // 退出重叠区
            pair<long double, long double> pos = BinarySearchForCover(
                    pair<long double, long double>{cur_x-dx,cur_y-dy},
                    pair<long double, long double>{cur_x,cur_y},
                    e);
            standpoint.push_back(pos);
            // printf("Leave Cover era:(%Lf,%Lf)\n",pos.first,pos.second);
            Covered = false;
        }
        cur_t++;
    }
    // 结束时是重叠区
    if (Covered){
        standpoint.push_back(
                pair<long double, long double>{cur_x, cur_y});
    }

    // 利用出入位置计算时间
    long double period = 0;
    for (int i = 0; i < standpoint.size()-1; i+=2) {
        dx = standpoint[i+1].first - standpoint[i].first;
        dy = standpoint[i+1].second - standpoint[i].second;
        long double dr = sqrt(dx*dx+dy*dy);
        long double dt = dr/(s.speed/3.6); //重叠区时间
        printf("Enter Cover era:(%Lf,%Lf)\n",
               standpoint[i].first,standpoint[i].second);
        printf("Leave Cover era:(%Lf,%Lf) period=%Lfs\n",
               standpoint[i+1].first,standpoint[i+1].second,dt);

        period += dt;
    }
    return period;
}

void Move::FindFirstEdge(double e, int seg_num) {
    const MoveSeg &s = seg[seg_num];
    // 按照step计算x, y方向每个周期的增量
    unsigned long rx2 = (s.ex-s.sx)*(s.ex-s.sx);
    unsigned long ry2 = (s.ey-s.sy)*(s.ey-s.sy);
    long double r = sqrt(rx2 + ry2);
    long double dx = INCDis*(s.ex-s.sx)/r;
    long double dy = INCDis*(s.ey-s.sy)/r;
    // 当前数据
    long double cur_x = s.sx, cur_y = s.sy;
    unsigned long long total_t = r/INCDis; //总距离周期
    unsigned long long cur_t = 0;//当前距离周期

    // 第一步: 寻找进入基站的点位
    while (cur_t <= total_t){
        cur_x += dx;
        cur_y += dy;
        Base* base_t = MoveData->GetPowerfulBase(cur_x, cur_y);
        if (base_t!= nullptr) break; //进入基站信号, 跳出
        cur_t++;
    }
    // 第二步: 二分探测边界
    long double lst_x = cur_x - dx, lst_y = cur_y - dy;
    long double nxt_x = cur_x, nxt_y = cur_y;
    long double mid_x, mid_y;
    long double dr = sqrt(dx*dx+dy*dy); //距离偏差
    Base* b = nullptr;
    while (dr>=e){
        dx = dx * 0.5;
        dy = dy * 0.5;
        dr = dr * 0.5;
        mid_x = lst_x + dx;
        mid_y = lst_y + dy;
        b = MoveData->GetPowerfulBase(mid_x, mid_y);
        if (b == nullptr){
            //为空, 说明还需要再前进
            lst_x = mid_x;
            lst_y = mid_y;
        } else{
            nxt_x = mid_x;
            nxt_y = mid_y;
        }
    }
    mid_x = (lst_x + nxt_x)/2;
    mid_y = (lst_y + nxt_y)/2;
    // 经过距离
    long double delta_r = sqrt((mid_x-s.sx)*(mid_x-s.sx)+(mid_y-s.sy)*(mid_y-s.sy));
    // 经过秒数
    long double t_double = delta_r / (s.speed/3.6);
    // 计算时间
    unsigned long t = t_double;
    int hour = s.sth + t / 3600;
    int minute = s.stm + (t%3600)/60;
    if (minute>60){
        // 对分钟进行修正
        hour++;
        minute = minute%60;
    }
    double seconds = t%60 + (t_double-t);
    printf("Switch to base %d: "
           "t=%.2d:%.2d:%lf \t"
           "position:(%Lf,%Lf)\n",
           b->num,
           hour,minute,seconds,
           mid_x,mid_y);
    // 第三步: 运行到离开的临界区
    dx = INCDis*(s.ex-s.sx)/r;
    dy = INCDis*(s.ey-s.sy)/r;
    while (cur_t <= total_t){
        cur_x += dx;
        cur_y += dy;
        Base* base_t = MoveData->GetPowerfulBase(cur_x, cur_y);
        if (base_t == nullptr) break; //退出基站信号, 跳出
        cur_t++;
    }
    // 第四步: 探测离开点
    lst_x = cur_x - dx, lst_y = cur_y - dy;
    nxt_x = cur_x, nxt_y = cur_y;
    dr = sqrt(dx*dx+dy*dy); //距离偏差
    while (dr>=e){
        dx = dx * 0.5;
        dy = dy * 0.5;
        dr = dr * 0.5;
        mid_x = lst_x + dx;
        mid_y = lst_y + dy;
        b = MoveData->GetPowerfulBase(mid_x, mid_y);
        if (b != nullptr){
            // 有信号, 说明还需要再前进
            lst_x = mid_x;
            lst_y = mid_y;
        } else{
            nxt_x = mid_x;
            nxt_y = mid_y;
        }
    }
    mid_x = (lst_x + nxt_x)/2;
    mid_y = (lst_y + nxt_y)/2;
    // 经过距离
    delta_r = sqrt((mid_x-s.sx)*(mid_x-s.sx)+(mid_y-s.sy)*(mid_y-s.sy));
    // 经过秒数
    t_double = delta_r / (s.speed/3.6);
    // 计算时间
    t = t_double;
    hour = s.sth + t / 3600;
    minute = s.stm + (t%3600)/60;
    if (minute>60){
        // 对分钟进行修正
        hour++;
        minute = minute%60;
    }
    seconds = t%60 + (t_double-t);
    printf("Leave base %d: "
           "t=%.2d:%.2d:%lf \t"
           "position:(%Lf,%Lf)",
           b->num,
           hour,minute,seconds,
           mid_x,mid_y);
}

pair<long double, long double>
Move::BinarySearchForCover(pair<long double, long double> l, pair<long double, long double> r, double e) {
    long double lx=l.first, ly=l.second;
    long double rx=r.first, ry=r.second;
    long double mx, my;
    long double dx = rx-lx, dy = ry-ly;
    long double dr = sqrt(dx*dx+dy*dy);
    bool Covered = false;
    vector<Base*> b = MoveData->GetEffBase(lx, ly);
    if (b.size()>=2){
        Covered = true;
    }
    // 开始二分
    while (dr>=e){
        dx = dx * 0.5;
        dy = dy * 0.5;
        dr = dr * 0.5;
        mx = lx + dx;
        my = ly + dy;
        b = MoveData->GetEffBase(mx,my);
        if (Covered){
            // 之前是Covered, 故寻找离开重叠区的位置
            if (b.size()<2){
                // 离开重叠区
                rx = mx;
                ry = my;
            } else{
                lx = mx;
                ly = my;
            }
        } else{
            // 之前非重叠
            if (b.size()>=2){
                rx = mx;
                ry = my;
            } else{
                lx = mx;
                ly = my;
            }
        }
    }
    mx = (lx + rx)/2;
    my = (ly + ry)/2;
    pair<long double, long double> ret{mx, my};
    return ret;
}

Move::~Move() {
    delete MoveData;
}

void Move::FindFirstCover(double e, int seg_num) {
    // 按照step计算x, y方向每个周期的增量
    auto s = seg[seg_num];
    unsigned long rx2 = (s.ex-s.sx)*(s.ex-s.sx);
    unsigned long ry2 = (s.ey-s.sy)*(s.ey-s.sy);
    long double r = sqrt(rx2 + ry2);
    long double dx = INCDis*(s.ex-s.sx)/r;
    long double dy = INCDis*(s.ey-s.sy)/r;
    // 当前数据
    long double cur_x = s.sx, cur_y = s.sy;
    unsigned long long total_t = r/INCDis; //总距离周期
    unsigned long long cur_t = 0;//当前距离周期
    // 标记是否在重叠区
    bool Covered = false;
    auto v = MoveData->GetEffBase(cur_x, cur_y);
    auto start_base = v[0]; // 起始基站
    vector<pair<long double, long double>> standpoint;
    while (cur_t <= total_t){
        cur_x += dx;
        cur_y += dy;
        // 获取附近的基站
        v = MoveData->GetEffBase(cur_x, cur_y);
        if (!Covered && v.size()>=2){
            // 进入重叠区
            // 二分计算
            pair<long double, long double> pos = BinarySearchForCover(
                    pair<long double, long double>{cur_x-dx,cur_y-dy},
                    pair<long double, long double>{cur_x,cur_y},
                    e);
            standpoint.push_back(pos);
            Covered = true;
        } else if (Covered && v.size()<2){
            // 记录基站
            // 退出重叠区
            pair<long double, long double> pos = BinarySearchForCover(
                    pair<long double, long double>{cur_x-dx,cur_y-dy},
                    pair<long double, long double>{cur_x,cur_y},
                    e);
            standpoint.push_back(pos);
            // 退出重叠区
            break;
        }
        cur_t++;
    }
    // 结束基站
    auto end_base = v[0];
    // 利用出入位置计算时间
    long double period = 0;
    pair<long double, long double> last_pos = standpoint[0];
    pair<long double, long double> cur_pos = standpoint[1];
    // 计算时间
    long double dx1 = last_pos.first - s.sx;
    long double dy1 = last_pos.second - s.sy;
    long double dr1 = sqrt(dx1*dx1+dy1*dy1);
    long double dx2 = cur_pos.first - s.sx;
    long double dy2 = cur_pos.second - s.sy;
    long double dr2 = sqrt(dx2*dx2+dy2*dy2);
    long double dt1 = dr1/(s.speed/3.6);
    long double dt2 = dr2/(s.speed/3.6);
    ::printf("Leave base %d: "
             "dt=%Lfs\t"
             "position:(%Lf,%Lf)\n",
                start_base->num,dt1,
                last_pos.first,last_pos.second);
    ::printf("Enter base %d: "
                "dt=%Lfs\t"
                "position:(%Lf,%Lf)\n",
                end_base->num,dt2,
                cur_pos.first,cur_pos.second);
    ::printf("Switch %d to %d, Total time: %Lfs\n",
             start_base->num, end_base->num,dt2-dt1);
}


