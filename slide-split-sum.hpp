#pragma once

#include "removable-queue.hpp"

/*
・多重集合を扱う
・[0,k) 番目と [k,N) 番目の sum がとれる
・O(k の変化量の総和 x log N)
*/

template <typename T, typename SUM_T = T> struct Slide_Split_Sum {
    Removable_Queue<pq<T>> ql;
    Removable_Queue<pqg<T>> qr;
    SUM_T sl, sr;
    Slide_Split_Sum() : sl(0), sr(0) {}

    inline int size() { return sz(ql) + sz(qr); }
    void clear() {
        ql = Removable_Queue<pq<T>>();
        qr = Removable_Queue<pqg<T>>();
        sl = sr = 0;
    }
    void insert(T x) { (x <= lmax() ? push_l(x) : push_r(x)); }
    void erase(T x) { (x <= lmax() ? erase_l(x) : erase_r(x)); }

    // L = sum of the k smallest elements, R = sum of the rest
    pair<SUM_T, SUM_T> query(int k) {
        assert(0 <= k && k <= size());
        while (sz(ql) < k) {
            push_l(pop_r());
        }
        while (sz(ql) > k) {
            push_r(pop_l());
        }
        return {sl, sr};
    }
    // 下位 k 個
    SUM_T query_l(int k) { return query(k).fi; }
    // 上位 k 個
    SUM_T query_r(int k) { return query(size() - k).se; }

  private:
    inline T lmax() { return (ql.empty() ? -infty<T> : ql.top()); }
    inline T rmin() { return (qr.empty() ? infty<T> : qr.top()); }
    inline T pop_l() {
        T x = ql.pop();
        sl -= x;
        return x;
    }
    inline T pop_r() {
        T x = qr.pop();
        sr -= x;
        return x;
    }
    inline void push_l(T x) { ql.push(x), sl += x; }
    inline void push_r(T x) { qr.push(x), sr += x; }
    inline void erase_l(T x) { ql.remove(x), sl -= x; }
    inline void erase_r(T x) { qr.remove(x), sr -= x; }
};
