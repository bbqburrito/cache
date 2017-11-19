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

Cache_set::Cache_set(): tag(0), MESI(0), associativity(0), ways_head(NULL)
{
}

Cache_set::Cache_set(const Cache_set& to_copy)
{
    tag = to_copy.get_tag();
    MESI = to_copy.get_MESI();
    associativity = to_copy.get_associativity();
    
    ways_head = to_copy.copy_list();

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
    current = ways_head->next;

    while(i < associativity && current)
    {
        current = current->next;
    }

    if(i < associativity)
    {
        current = new Ways_node(to_insert);
        return 1;
    }
    
    else return 0;
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
int Cache_set::get_tag(int to_set)
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
Way_node * Cache_set::copy_list(Way_node * head)
{
    Way_node * current = NULL;
    Way_node * temp = ways_head;

    //if passed node is not NULL, return NULL pointer
    if(head)
        return current;

    if(!ways_head)
        return head;

    head = new Way_node(ways_head);
    current = head;

    while (temp->next)
    {
        current->next = new Way_node(temp->next);
        current = current->next;
        temp = temp->next;
    }

    return head;
}



L1_cache::L1_cache(): sets(NULL), associativity(0)
{
}








