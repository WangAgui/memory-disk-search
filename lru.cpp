#include"common.h"
#include"lru.h"

LRUcache::LRUcache(int capacity) {
    this->capacity = capacity;
    this->count = 0;

    head = new Node(-1,nullptr);
    tail = new Node(-1,nullptr);
    
    head->next = tail;
    tail->prev = head;

    return ;
}

void LRUcache::deleteNode(Node* node) {
    Node* a = node->prev;
    Node* b = node->next;

    node->prev = nullptr;
    node->next = nullptr;

    a->next = b;
    b->prev = a;

    return ;
}

void LRUcache::addToHead(Node* node) {
    Node* b = head->next;

    node->next = b;
    node->prev = head;

    head->next = node;
    b->prev = node;

    return ;
}
void LRUcache::insert(int year,File* file) {
    if(count+file->get_file_size()>capacity) {
        Node* t = tail->prev;
        if(t==head) {
            cout<<"文件太大或者LRU分配内存太小！"<<endl;
            return ;
        }

        deleteNode(t);
        year2file[t->year] = nullptr;
        delete t->file;    //释放内存写回磁盘
        insert(year,file);
    }

    Node* newnode = new Node(year,file);
    year2file[year] = newnode;
    addToHead(newnode);

    return ;
}

File* get(int year) {
    if(year2file[year]==nullptr) {
        string filename = to_string(year)+".dat";
        File* file = new File(filename);
        insert(year,file);
    }

    Node* t = year2file[year];

    this->deleteNode(t);
    this->addToHead(t);

    return t->file;
}
