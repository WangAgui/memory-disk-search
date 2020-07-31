#include <iostream>
#include "file_operation.h"
using namespace std;


#if 0
static int block_seek(vector<Kline> &vec, unsigned int target, bool &sign)
{
    
    int low = 0;
    int high = vec.size() - 1;
    
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (vec[mid].iDate > target)
        {
            high = mid - 1;
        }
        else if (vec[mid].iDate < target)
        {
            low = mid + 1;
        }
        else
        {
            sign = true;
            return mid;
        }
    }
    sign = false;
    int notFound = high + 1;
    return notFound;
}

static int index_seek(vector<unsigned int> &index, unsigned int target, bool &sign)
{
    int low = 0;
    int high = index.size() - 1;
    
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (index[mid] > target)
        {
            high = mid - 1;
        }
        else if (index[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            sign = true;
            return mid;
        }
    }
    sign = false;
    int notFound = high + 1;
    return notFound;
}
#endif

/************���캯�������ļ�����ȫ������**************/
//file_name->�ļ���
File::File(std::string file_name)
{
    write_sign = Not_Write_to_Disk;
    filename = file_name;
    index = vector<int>(TimeMAX, -1);//��ʼ��������index
    stock_num = vector<int>(TimeMAX, 0);//��ʼ��stock��Ŀ��
    
    open_file.open(file_name, ios::binary|ios::in);//���ļ�
    if(open_file.is_open())
    {
        cout<<filename<<"->�ļ����ڣ��Ѿ���"<<endl;
        file_exist_sign = File_Exist;
        open_file.read( (char *) &block_num, sizeof(block_num));//��ȡ�洢��ʱ�������
        open_file.read( (char *) &time_max, sizeof(time_max));//��ȡ�洢��ʱ�������ʱ��

        for(int i = 0; i < TimeMAX; i++)//����������
        {
            open_file.read( (char *) &index[i], sizeof(int));
        }
        for(int i = 0; i < TimeMAX; i++)//����stock��Ŀ��
        {
            open_file.read( (char *) &stock_num[i], sizeof(int));
        }

        vector<Kline> data_block(10000);//��ʼ�����ݻ�����size
        day_block.resize(block_num, data_block);

        /*��������*/
        for(int i = 0; i < block_num; i++)
        {
            for(int j = 0; j < 10000; j++)
            {
                open_file.read( (char *) &day_block[i][j], sizeof(Kline));
            }
        }

        cout<<"�Ѷ����ļ�"<<endl;
    }
    else
    {
        block_num = 0;
        time_max = -1;
        file_exist_sign = File_not_Exist;

        cout<<filename<<"->�ļ�������"<<endl;
    }
    open_file.close();

    
}

File::~File()
{
    if(write_sign)
    {
        write_file.open(filename, ios::binary|ios::out);//���ļ�

        write_file.write( (char *) &block_num, sizeof(block_num));//��block_num�����ļ�
        write_file.write( (char *) &time_max, sizeof(time_max));//��time_max�����ļ�

        for(int i = 0; i < TimeMAX; i++)//����������ļ�
        {
            write_file.write( (char *) &index[i], sizeof(int));
        }

        for(int i = 0; i < TimeMAX; i++)//stock��Ŀ������ļ�
        {
            write_file.write( (char *) &stock_num[i], sizeof(int));
        }

        /*���ݴ����ļ�*/
        for(int i = 0; i < block_num; i++)
        {
            for(int j = 0; j < 10000; j++)
            {
                write_file.write( (char *) &day_block[i][j], sizeof(Kline));
            }
        }

        write_file.close();
        cout<<filename<<"->��д���ļ�"<<endl;
    }
    else
    {
        cout<<filename<<"->����δ���ģ�����д��"<<endl;
    }

    /*�ͷ�vector*/
    vector<vector<Kline>>().swap(day_block);
    vector<int>().swap(index);
    vector<int>().swap(stock_num); 
    
}

/************��ѯ���ݺ���������λ��Ϊ�ڴ滺����**************/
//data->��������
//time->Ҫ��ѯ���������
//stock_id->��Ʊ��Դ���

int File::file_read_data(Kline &data, const int time, const int stock_id)
{
    assert(file_exist_sign);
    assert(time >= 0 && time < 400);
    assert(stock_id >= 0 && stock_id < 10000);

    int pos = index[time];
    if(pos != -1)
    {
        if(day_block[pos][stock_id].iDate != NotExist)
        {
            data = day_block[pos][stock_id];
            return 1;
        }
        else
        {
            cout<<"Ŀ������"<<endl;
            return 0;
        }  
    }
    else
    {
        cout<<"���ڲ�����"<<endl;
        return 0;
    } 
}

int File::file_read_data(std::vector<Kline> &data, const int time_start, const int time_end, const int stock_id)
{
    assert(file_exist_sign);
    assert(time_start < time_end);
    assert(time_start >= 0 && time_start < 400);
    assert(time_end >= 0 && time_end < 400);
    assert(stock_id >= 0 && stock_id < 10000);

    int pos;
    int count = 0;
    data.clear();
    for(int time_now = time_start; time_now <= time_end; time_now++)//���α�����ʱ��ε�����
    {
        pos = index[time_now];//���������ҵ���Ӧ���к�
        if(pos != -1)
        {
            if(day_block[pos][stock_id].iDate != NotExist)
            {
                data.push_back(day_block[pos][stock_id]);
                count++;
            }
        }
    }
    return count;
}

int File::file_insert_data(Kline &data, const int time, const int stock_id)
{
    assert(time >= 0 && time < 400);
    assert(stock_id >= 0 && stock_id < 10000);

    write_sign = Need_Write_to_Disk;
    file_exist_sign = File_Exist;

    if(time > time_max)/*��β�˲���*/
    {
        cout<<"��β�˲���"<<endl;

        time_max = time;
        block_num++;

        index[time] = block_num - 1;//����������
        stock_num[time]++;//����stock��Ŀ��

        /*��������*/
        vector<Kline> data_block_temp(10000);
        data_block_temp[stock_id] = data;
        day_block.push_back(data_block_temp);
    }
    else if(index[time] == -1)/*�ڷ�β�˲���*/
    {
        cout<<"�ڷ�β�˲���"<<endl;

        block_num++;

        int cur_time;
        for(cur_time = time+1; time < time_max; cur_time++)//Ѱ�Ҳ�����±�
        {
            if(index[cur_time] != -1)
            {
                break;
            }
        }

        index[time] = index[cur_time];//���²�����±�
        for(int i = cur_time; i <= time_max; i++)//���º��������
        {
            if(index[i] != -1)
            {
                index[i]++;
            }
        }

        stock_num[time]++;//����stock��Ŀ��

        /*��������*/
        vector<Kline> data_block_temp(10000);
        data_block_temp[stock_id] = data;
        day_block.insert((day_block.begin()+index[time]), data_block_temp);
    }
    else/*�����е��������*/
    {
        cout<<"�����е��������"<<endl;
        
        int ret_sign = 1;
        int pos = index[time];
        if(day_block[pos][stock_id].iDate != NotExist)
        {
            ret_sign = 0;
        }

        stock_num[time]++;

        day_block[pos][stock_id] = data;
        return ret_sign;
    }
    return 1;
}

int File::file_delete_data(const int time, const int stock_id)
{
    assert(file_exist_sign);
    assert(time >= 0 && time < 400);
    assert(stock_id >= 0 && stock_id < 10000);

    int del_sign = stock_num[time];

    if(del_sign > 1)
    {
        if(day_block[index[time]][stock_id].iDate != NotExist)
        {
            day_block[index[time]][stock_id].iDate = NotExist;
            stock_num[time]--;
            write_sign = Need_Write_to_Disk;
            cout<<"ɾ����������"<<endl;
            return 1;
        }
        else
        {
            cout<<"���ڴ��ڣ�Ŀ�겻����"<<endl;
            return 0;
        }
        
    }
    else if(del_sign == 1)
    {
        if(day_block[index[time]][stock_id].iDate != NotExist)
        {
            day_block.erase(day_block.begin()+index[time]);

            index[time] = -1;//����������
            for(int i = time; i <= time_max; i++)//���º��������
            {
                if(index[i] != -1)
                {
                    index[i]--;
                }
            }
            
            stock_num[time]--;
            block_num--;
            write_sign = Need_Write_to_Disk;
            cout<<"ɾ����������"<<endl;
            return 1;
        }
        else
        {
            cout<<"���ڴ��ڣ�Ŀ�겻����"<<endl;
            return 0;
        } 
    }
    else
    {
        cout<<"���ڲ�����"<<endl;
        return 0;
    }  
}

int File::get_file_size()
{
    return 802*sizeof(int) + block_num*10000*sizeof(Kline);
}


