#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <iostream>

struct LLHead
{
    // Linked List Head Node
    LLHead *next;
    LLHead *prev;
};



class Node
{
    private:
        int startAdr;
        int endAdr;
        int blockSize;
        int refCount;
        LLHead node;
    public:
    
};

