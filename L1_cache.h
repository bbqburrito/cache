#include<iostream>
#include<cmath>
#include<ctime>
#include<cstdlib>
#include<cctype>


class Way_node {
    public:
        Way_node();
        Way_node(const Way_node&);
        Way_node(int make_add, int make_LRU, int make_MESI);
        Way_node(int make_add, int make_LRU, int make_MESI, Way_node * make_next);
        ~Way_node();

    protected:
        int address;
        int LRU;
        int MESI;
        Way_node * next;
};



class Cache_set {
    public:
        Cache_set();
        Cache_set(const Cache_set&);
        ~Cache_set();
        int insert(Way_node & to_insert);
        int get_tag() const;
        int get_MESI() const;
        int get_associativity() const; 
        int set_tag(int to_set);
        int set_MESI(int to_set);
        int set_associativity(int to_set); 
        Way_node * copy_list();




    protected:
        int tag;
        int MESI;
        int associativity;
        Way_node * ways_head;
};

class L1_cache {
    public:
        L1_cache();
        L1_cache(const L1_cache&);
        ~L1_cache();





    protected:
        Cache_set * sets;
        int associativity;



};



