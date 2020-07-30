#pragma once

#include <time.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <assert.h>

#include "common.h"

/*宏定义区域*/
#define TimeMAX 400
#define NotExist UINT_MAX

#define Need_Write_to_Disk 1
#define Not_Write_to_Disk 0

#define File_Exist 1
#define File_not_Exist 0

class File
{
public:
    
    File(std::string file_name);
    ~File();

    /*读取一条数据，time为相对日期，stock_id为股票相对代码；返回值：1->读到数据、0->没有对应数据*/
    int file_read_data(Kline &data, const int time, const int stock_id);

    /*范围读取数据，time为相对日期，stock_id为股票相对代码；返回值：读到的数据个数*/
    int file_read_data(std::vector<Kline> &data, const int time_start, const int time_end, const int stock_id);

    /*插入一条数据，time为相对日期，stock_id为股票相对代码；返回值：0->有相同数据产生了覆盖、1->插入成功*/
    int file_insert_data(Kline &data, const int time, const int stock_id);

    /*删除一条数据，time为相对日期，stock_id为股票相对代码；返回值：0->没有对应的数据、1->删除成功*/
    int file_delete_data(const int time, const int stock_id);

    /*返回文件（内存）大小，单位为字节*/
    int get_file_size();


private:

    int block_num;//日数据块数量
    int time_max;//已存数据中time最大值

    std::vector<int> index;//索引表
    std::vector<int> stock_num;//股票数目表
    std::vector< std::vector<Kline> > day_block;//数据缓冲区

    /*以上的成员信息会存入文件*/

    int write_sign;//写入磁盘标识
    int file_exist_sign;//文件是否存在标识
    std::string filename;

    std::ifstream open_file;
    std::ofstream write_file;


};

