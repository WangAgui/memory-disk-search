#pragma once

#include <time.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <assert.h>

#include "common.h"

/*�궨������*/
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

    /*��ȡһ�����ݣ�timeΪ������ڣ�stock_idΪ��Ʊ��Դ��룻����ֵ��1->�������ݡ�0->û�ж�Ӧ����*/
    int file_read_data(Kline &data, const int time, const int stock_id);

    /*��Χ��ȡ���ݣ�timeΪ������ڣ�stock_idΪ��Ʊ��Դ��룻����ֵ�����������ݸ���*/
    int file_read_data(std::vector<Kline> &data, const int time_start, const int time_end, const int stock_id);

    /*����һ�����ݣ�timeΪ������ڣ�stock_idΪ��Ʊ��Դ��룻����ֵ��0->����ͬ���ݲ����˸��ǡ�1->����ɹ�*/
    int file_insert_data(Kline &data, const int time, const int stock_id);

    /*ɾ��һ�����ݣ�timeΪ������ڣ�stock_idΪ��Ʊ��Դ��룻����ֵ��0->û�ж�Ӧ�����ݡ�1->ɾ���ɹ�*/
    int file_delete_data(const int time, const int stock_id);

    /*�����ļ����ڴ棩��С����λΪ�ֽ�*/
    int get_file_size();


private:

    int block_num;//�����ݿ�����
    int time_max;//�Ѵ�������time���ֵ

    std::vector<int> index;//������
    std::vector<int> stock_num;//��Ʊ��Ŀ��
    std::vector< std::vector<Kline> > day_block;//���ݻ�����

    /*���ϵĳ�Ա��Ϣ������ļ�*/

    int write_sign;//д����̱�ʶ
    int file_exist_sign;//�ļ��Ƿ���ڱ�ʶ
    std::string filename;

    std::ifstream open_file;
    std::ofstream write_file;


};

