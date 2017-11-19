#include"L1_cache.h"






Way_node::Way_node(): address(0), LRU(0), MESI(0), next(NULL)
{
}

//constructor to set member values. leaves next NULL
Way_node::Way_node(int make_add, int make_LRU, int make_MESI): address(make_add), LRU(make_LRU), MESI(make_MESI), next(NULL)
{
}

//constructor to set member values, including next
Way_node::Way_node(int make_add, int make_LRU, int make_MESI, Way_node * make_next): address(make_add), LRU(make_LRU), MESI(make_MESI), next(make_next)
{
}

Way_node::Way_node(const Way_node& to_copy): address(to_copy.address), LRU(to_copy.LRU), MESI(to_copy.MESI), next(NULL)
{
}

Way_node * Way_node::get_next() const
{
    return next;
}

Way_node * Way_node::set_next(Way_node * to_set)
{
    next = to_set;
    return next;
}

Cache_set::Cache_set(): tag(0), MESI(0), associativity(0), ways_head(NULL)
{
}

Cache_set::Cache_set(const Cache_set& to_copy)
{

    tag = to_copy.get_tag();
    MESI = to_copy.get_MESI();
    associativity = to_copy.get_associativity();
    
    ways_head = to_copy.copy_list(ways_head);

}

//insert new value to set. returns 1 if successful. returns
//0 if set is full.
int Cache_set::insert(Way_node& to_insert)
{
    int i = 0;
    Way_node * current;

    if(!ways_head)
    {
        ways_head = new Way_node(to_insert);
        return 1;
    }

    //if(insert(&to_insert, ways_head))
    //    return 1;
    current = ways_head->get_next();

    while(i < associativity && current)
    {
        current = current->get_next();
    }

    if(i < associativity)
    {
        current = new Way_node(to_insert);
        return 1;
    }
    
    else return 0;
}

//returns the number of valid ways in the set as int.
int Cache_set::num_ways() const
{
    int count = 0;
    Way_node * current = ways_head;

    while(current)
    {
        current = current->get_next();
        ++count;
    }

    return count;
}



//getter for tag member.
int Cache_set::get_tag() const
{
    return tag;
}

//getter for MESI member.
int Cache_set::get_MESI() const
{
    return MESI;
}

//getter for associativity member
int Cache_set::get_associativity() const
{
    return associativity;
}

//setter for tag member. returns tag value
int Cache_set::set_tag(int to_set)
{
    tag = to_set;
    return tag;
}

//setter for MESI member. returns MESI value
int Cache_set::set_MESI(int to_set)
{
    MESI = to_set;
    return MESI;
}

//setter for associativity member. returns associativity value
int Cache_set::set_associativity(int to_set)
{
    associativity = to_set;
    return associativity;
}

//protected function to copy LLL of Way_node. should be passed a NULL
//pointer. returns NULL pointer if not passed a NULL pointer.
Way_node * Cache_set::copy_list(Way_node * head) const
{
    Way_node * current = NULL;
    Way_node * temp = ways_head;

    //if passed node is not NULL, return NULL pointer
    if(head)
        return current;

    if(!ways_head)
        return head;

    head = new Way_node(*ways_head);
    current = head;

    while (temp->get_next())
    {
        current->next = new Way_node(*temp->next);
        current = current->next;
        temp = temp->next;
    }

    return head;
}



L1_cache::L1_cache(): sets(NULL), associativity(0), size(0)
{
}

L1_cache::L1_cache(int set_assoc, int set_size): associativity(set_assoc), size(set_size)
{
    sets = new Cache_set*[size];
}

L1_cache::L1_cache(const L1_cache& to_copy)
{
    Cache_set ** make_copy = NULL;
    associativity = to_copy.get_associativity();
    size = to_copy.get_size();

    sets = to_copy.copy_sets(make_copy);
}

int L1_cache::get_associativity() const
{
    return associativity;
}

int L1_cache::get_size() const
{
    return size;
}

int L1_cache::set_associativity(int to_set)
{
    associativity = to_set;
    return associativity;
}

int L1_cache::set_size(int to_set)
{
    size = to_set;
    return size;
}


//protected function to copy sets array. should be passed a NULL
//pointer. returns NULL pointer if not passed a NULL pointer.
Cache_set ** L1_cache::copy_sets(Cache_set ** to_copy) const
{
    int i = 0;
    Cache_set ** current = NULL;

    //if passed node is not NULL, return NULL pointer
    if(to_copy)
        return current;

    if(!sets)
        return to_copy;

    to_copy = new Cache_set*[size];

    for(i = 0; i < size; ++i)
    {
        to_copy[i] = new Cache_set(*sets[i]);
    }

    return to_copy;
}



