#pragma once
#include "primetable.hpp"

// [0, LIM], 0, 1 には -1 が入る。
V<int> lpf_table(ll LIM) {
    auto primes = primetable(LIM);
    V<int> res(LIM + 1, -1);
    per(i, sz(primes)) {
        auto p = primes[i];
        rep(j, 1, LIM / p + 1) res[p * j] = p;
    }
    return res;
}

V<pair<ll, int>> factor_by_lpf(ll n, V<int>& lpf) {
    V<pair<ll, int>> res;
    while (n > 1) {
        int p = lpf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        res.eb(p, e);
    }
    return res;
}
