#ifndef COMMON_H_
#define COMMON_H_
#include <stdint.h>
#include <limits.h>
#include <unordered_map>
#include <cstring>
#include <vector>
#include "stock.h"
#include "file_operation.h"
#define BEGIN_YEAR 1990
#define CURRENT_YEAR 2020
#define LRU_MAX_SIZE (1024*1024*1024)

//unordered_map<int,File*> year2file;
struct Kline 
{
    unsigned int iDate;      // YYYYMMDD
    unsigned int fOpen_l;      // ���̼�
    unsigned int fClose_l;     // ���̼�
    unsigned int fHigh_l;       // ��߼�
    unsigned int fLow_l;        // ��ͼ�
    unsigned int lVolume_l;   // �ɽ���

    unsigned short shtTime;      // YYYYMMDD
    unsigned short fOpen_r;      // ���̼�
    unsigned short fClose_r;     // ���̼�
    unsigned short fHigh_r;       // ��߼�
    unsigned short fLow_r;        // ��ͼ�
    unsigned short lVolume_r;   // �ɽ���

    double fAmount;     // ��λ�ɽ����

    Kline()
    {
        iDate = UINT_MAX;
    }
    Kline(unsigned int date,unsigned int time,unsigned int fo_l, unsigned int fo_r,unsigned int fc_l, unsigned int fc_r,
            unsigned int fh_l, unsigned int fh_r,unsigned int fl_l, unsigned int fl_r,unsigned int lv_l, unsigned int lv_r,double fa)
            :iDate(date),shtTime(time),fOpen_l(fo_l),fOpen_r(fo_r),fClose_l(fc_l),fClose_r(fc_r),
            fHigh_l(fh_l),fHigh_r(fh_r),fLow_l(fl_l),fLow_r(fl_r),lVolume_l(lv_l),lVolume_r(lv_r),fAmount(fa){}
};

struct tbtDate {
    short Minute;      // �������ķ�����
    int   iTime;       //  hhmmss
    double Now;        // �ּ�*1000

    int64_t NowVol;    // ����
    
    short InOutFlag;   // ������־
    tbtDate(int m,int it,int no,int nov,int iof):Minute(m),iTime(it),Now(no),NowVol(nov),InOutFlag(iof){}
};

typedef struct times
{
	int Year;
	int Mon;
	int Day;
	int Hour;
	int Min;
	int Second;
}Times;
 
Times stamp_to_standard(int stampTime)
{
	time_t tick = (time_t)stampTime;
	struct tm tm; 
	char s[100];
	Times standard;
 
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	printf("%d: %s\n", (int)tick, s); 
 
	standard.Year = atoi(s);
	standard.Mon = atoi(s+5);
	standard.Day = atoi(s+8);
	standard.Hour = atoi(s+11);
	standard.Min = atoi(s+14);
	standard.Second = atoi(s+17);
 
	return standard;
}
#endif /*COMMON_H_*/
