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
        Way_node * get_next() const;
        Way_node * set_next(Way_node * to_set);

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
        int num_ways() const;




    protected:
        int tag;
        int MESI;
        int associativity;
        Way_node * ways_head;
        Way_node * copy_list(Way_node * head) const;
};

class L1_cache {
    public:
        L1_cache();
        L1_cache(int set_assoc, int set_size);
        L1_cache(const L1_cache&);
        ~L1_cache();
        int get_associativity() const;
        int get_size() const;
        int set_associativity(int to_set);
        int set_size(int to_set);
        





    protected:
        Cache_set ** sets;
        int associativity;
        int size;
        Cache_set ** copy_sets(Cache_set ** to_copy) const;



};



