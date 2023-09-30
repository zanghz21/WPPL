#pragma once
#include "util/SearchForHeuristics/SpatialState.h"
#include <iostream>

namespace RIVERS {
namespace SPATIAL {

class OpenList {
public:
    State ** heap;

    bool (* is_better)(const State * s1, const State * s2);
    int max_size;
    int size;

    OpenList(int _max_size, bool (*_is_better) (const State * s1, const State * s2), State ** _heap): max_size(_max_size), is_better(_is_better), size(0), heap(_heap) {
        // heap = new State * [max_size];
    };

    ~OpenList(){
        // delete [] heap;
    };

    inline void swap(State *s, State * p) {
        int i = s->heap_index;
        int j = p->heap_index;
        heap[i] = p;
        heap[j] = s;
        s->heap_index = j;
        p->heap_index = i;
    }

    inline void move_up(State *s) {
        int heap_index = s->heap_index;
        while (heap_index>0) {
            int parent_heap_index = (heap_index-1)/2;
            if (is_better(heap[parent_heap_index], heap[heap_index])) {
                break;
            }
            swap(s, heap[parent_heap_index]);
            heap_index = parent_heap_index;
        }
    }

    inline void move_down(State * s) {
        int heap_index = s->heap_index;
        while (heap_index*2+1<size) {
            int child_heap_index = heap_index*2+1;
            if (child_heap_index+1<size && is_better(heap[child_heap_index+1], heap[child_heap_index])) {
                ++child_heap_index;
            }
            if (is_better(heap[heap_index], heap[child_heap_index])) {
                break;
            }
            swap(s, heap[child_heap_index]);
            heap_index = child_heap_index;
        }

    }

    void push(State * s) {
        // if (full()) {
        //     std::cerr<<"the heap is full!"<<std::endl;
        //     exit(-1);
        // }

        heap[size] = s;
        s->heap_index = size;
        ++size;
        move_up(s);
    }

    State * pop() {
        // if (empty()) {
        //     std::cerr<<"the heap is empty!"<<std::endl;
        //     exit(-1);
        // }

        State * ret = heap[0];
        --size;
        heap[0] = heap[size];
        heap[0]->heap_index = 0;
        move_down(heap[0]);

        return ret;
    }

    State * top() {
        return heap[0];
    }

    void increase(State * s) {
        move_up(s);
    }   

    inline bool empty() {
        return size==0;
    }

    inline bool full() {
        return size==max_size;
    }

    void clear() {
        size = 0;
    }

};

}
}