
#ifndef ROCKETCORECONTAINERWRAPPER_H
#define ROCKETCORECONTAINERWRAPPER_H

#ifdef LIBROCKET_CUSTOM_CONTAINER_WRAPPER
   #include LIBROCKET_CUSTOM_CONTAINER_WRAPPER
#else

   #include <map>
   #include <set>
   #include <vector>
   #include <list>
   #include <queue>
   #include <stack>
   #include <iterator>

   namespace Rocket {
   namespace Core {
   namespace Container {

   template<typename T> struct vector {
      typedef std::vector<T> Type;
   };
   template<typename Key> struct set {
      typedef std::set<Key> Type;
   };
   template<typename Key, typename T, class compare=std::less<Key> > struct map {
      typedef std::map<Key, T> Type;
   };
   template<typename T> struct list {
      typedef std::list<T> Type;
   };
   template<typename T> struct queue {
      typedef std::queue<T> Type;
   };
   template<typename T> struct deque {
      typedef std::deque<T> Type;
   };
   template<typename T> struct stack {
      typedef std::stack<T> Type;
   };
   template<class A, class B> struct pair {
      typedef std::pair<A, B> Type;
   };

   template <typename InputIterator, typename T> inline InputIterator
         find(InputIterator first, InputIterator last, const T& value) {
      return std::find(first, last, value);
   }

   template <typename InputIterator, typename Function> inline Function
         for_each(InputIterator first, InputIterator last, Function function) {
      return std::for_each(first, last, function);
   }

   template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
   OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                                 InputIterator2 first2, InputIterator2 last2,
                                 OutputIterator result) {
      return std::set_difference(first1, last1, first2, last2, result);
   };

   template <typename ContainerType> inline std::back_insert_iterator<ContainerType>
         back_inserter(ContainerType& x){
      return std::back_inserter<ContainerType>(x);
   }

   template <typename RandomAccessIterator>
   inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
       std::sort<RandomAccessIterator>(first, last);
   }

   template <typename RandomAccessIterator, typename Compare>
   inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare compare)
   {
       std::sort<RandomAccessIterator, Compare>(first, last, compare);
   }

   template <typename RandomAccessIterator, typename StrictWeakOrdering>
   void stable_sort(RandomAccessIterator first, RandomAccessIterator last, StrictWeakOrdering compare) {
       std::stable_sort(first, last, compare);
   }


   }  //namespace Container
   }  //namespace Core
   }  //namespace Rocket

#endif   //#ifdef LIBROCKET_CUSTOM_CONTAINER_WRAPPER

#endif   //#ifndef ROCKETCORECONTAINERWRAPPER_H
