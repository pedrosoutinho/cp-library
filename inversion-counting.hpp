#pragma once

#include "binary-indexed-tree.hpp"

// 転倒数
template <typename T> long long inversion_counting(const vector<T>& v) {
    vector<T> xs{v};
    sort(begin(xs), end(xs));
    xs.erase(unique(begin(xs), end(xs)), end(xs));
    int s = xs.size();
    BinaryIndexedTree<long long> bit(s + 1);
    long long ans = 0;
    for (auto& x : v) {
        int i = lower_bound(begin(xs), end(xs), x) - begin(xs);
        if (i + 1 != s) ans += bit.sum(i + 1, s - 1);
        bit.add(i, 1);
    }
    return ans;
}

// 隣接 swap によって v を w に変えるのにかかる手数 (不可能 : -1)
template <typename T>
long long swap_distance(const vector<T>& v, const vector<T>& w) {
    if (v.size() != w.size()) return -1;
    int N = v.size();
    vector<pair<T, int>> VV(N), WW(N);
    for (int i = 0; i < N; i++) {
        VV[i] = make_pair(v[i], i);
        WW[i] = make_pair(w[i], i);
    }
    sort(begin(VV), end(VV));
    sort(begin(WW), end(WW));
    for (int i = 0; i < N; i++) {
        if (VV[i].first != WW[i].first) return -1;
    }
    vector<int> order(N);
    for (int i = 0; i < N; i++) {
        order[VV[i].second] = WW[i].second;
    }
    return inversion_counting(order);
}
