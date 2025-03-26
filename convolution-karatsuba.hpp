#pragma once

#include "convolution-naive.hpp"

// 任意の環でできる
template <typename T>
V<T> convolution_karatsuba(const V<T>& f, const V<T>& g) {
    const int thresh = 30;
    if (min(len(f), len(g)) <= thresh) return convolution_naive(f, g);
    int n = max(len(f), len(g));
    int m = cdiv(n, 2);
    V<T> f1, f2, g1, g2;
    if (len(f) < m) f1 = f;
    if (len(f) >= m) f1 = {f.begin(), f.begin() + m};
    if (len(f) >= m) f2 = {f.begin() + m, f.end()};
    if (len(g) < m) g1 = g;
    if (len(g) >= m) g1 = {g.begin(), g.begin() + m};
    if (len(g) >= m) g2 = {g.begin() + m, g.end()};
    V<T> a = convolution_karatsuba(f1, g1);
    V<T> b = convolution_karatsuba(f2, g2);
    rep(i, len(f2)) f1[i] += f2[i];
    rep(i, len(g2)) g1[i] += g2[i];
    V<T> c = convolution_karatsuba(f1, g1);
    V<T> F(len(f) + len(g) - 1);
    assert(2 * m + len(b) <= len(F));
    rep(i, len(a)) F[i] += a[i], c[i] -= a[i];
    rep(i, len(b)) F[2 * m + i] += b[i], c[i] -= b[i];
    if (c.back() == T(0)) c.pop_back();
    rep(i, len(c)) if (c[i] != T(0)) F[m + i] += c[i];
    return F;
}
