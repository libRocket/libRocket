
#ifndef LIBROCKETCONTAINERWRAPPER_H
#define LIBROCKETCONTAINERWRAPPER_H

#include "eastl/map.h"
#include "eastl/set.h"
#include "eastl/vector.h"
#include "eastl/list.h"
#include "eastl/stack.h"
#include "eastl/queue.h"
#include "eastl/iterator.h"
#include "eastl/algorithm.h"
#include "eastl/sort.h"
#include "eastl/extra/foreach.h"

namespace Rocket {
namespace Core {
namespace Container {

//be careful about performance, is also used in the queues...
template<typename T> class dequeReplacement : public eastl::vector<T> {
public:
   void pop_front() {
      eastl::vector<T>::erase(eastl::vector<T>::begin());
   }
   void popFront() {
      eastl::vector<T>::erase(eastl::vector<T>::begin());
   }
   void push_back(const T& v) {
      eastl::vector<T>::pushBack(v);
   }
};

template<typename T> class vectorReplacement : public eastl::vector<T> {
public:
   vectorReplacement()
      : eastl::vector<T>()
   {}
   explicit vectorReplacement(size_type n)
      : eastl::vector<T>(n)
   {}
   vectorReplacement(size_type n, const T& value)
      : eastl::vector<T>(n, value)
   {}

   void push_back(const value_type& val) {
      eastl::vector<T>::pushBack(val);
   }
   void pop_back() {
      eastl::vector<T>::popBack();
   }
};

template<typename T> class listReplacement : public eastl::list<T> {
public:
   void push_back(const T& val) {
      eastl::list<T>::pushBack(val);
   }
};

template<typename T> struct vector {
   typedef vectorReplacement<T> Type;
};
template<typename Key> struct set {
   typedef eastl::set<Key> Type;
};
template<typename Key, typename T, class compare=eastl::less<Key> > struct map {
   typedef eastl::map<Key, T> Type;
};
template<typename T> struct list {
   typedef listReplacement<T> Type;
};
template<typename T> struct queue {
   typedef eastl::queue<T, dequeReplacement<T> > Type;
};
template<typename T> struct deque {
   typedef dequeReplacement<T> Type;
};
template<typename T> struct stack {
   typedef eastl::stack<T> Type;
};
template<class A, class B> struct pair {
   typedef eastl::pair<A, B> Type;
};

template <typename InputIterator, typename T> inline InputIterator
      find(InputIterator first, InputIterator last, const T& value) {
   return eastl::find(first, last, value);
}

template <typename InputIterator, typename Function> inline Function
      for_each(InputIterator first, InputIterator last, Function function) {
   return eastl::forEach(first, last, function);
}

template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result) {
   return eastl::setDifference(first1, last1, first2, last2, result);
};

template <typename ContainerType> inline eastl::back_insert_iterator<ContainerType>
      back_inserter(ContainerType& x){
   return eastl::back_inserter<ContainerType>(x);
}

template <typename RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
    eastl::quickSort<RandomAccessIterator>(first, last);
}
template <typename RandomAccessIterator, typename Compare>
inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare compare)
{
    eastl::quickSort<RandomAccessIterator, Compare>(first, last, compare);
}

template <typename RandomAccessIterator, typename StrictWeakOrdering>
void stable_sort(RandomAccessIterator first, RandomAccessIterator last, StrictWeakOrdering compare) {
    eastl::mergeSort<RandomAccessIterator, EASTLAllocatorType, StrictWeakOrdering>
                     (first, last, *eastl::getDefaultAllocator(0), compare);
}


}  //namespace Container
}  //namespace Core
}  //namespace Rocket

#endif   //LIBROCKETCONTAINERWRAPPER_H
