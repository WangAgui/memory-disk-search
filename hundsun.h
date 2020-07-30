#ifndef HUNDSUN_H_
#define HUNDSUN_H_
#include"common.h"
class hundsun {
public:
    hundsun();
    // 根据股票代码，时间范围查日Kline数据 //
    vector<Kline> queryDayKline(unsigned int stockId,string stockName,int startDate,int endDate);
    
    
private:

};

#endif