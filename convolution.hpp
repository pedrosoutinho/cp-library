#pragma once

#include "mod-int.hpp"

#include "mod-inv.hpp"

#include "crt3.hpp"

#include "convolution-naive.hpp"

#include "convolution-karatsuba.hpp"

#include "ntt-maspy.hpp"

template <class mint>
vector<mint> convolution_ntt(vector<mint> a, vector<mint> b) {
    if (a.empty() || b.empty()) return {};
    int n = sz(a), m = sz(b);
    int sz = 1;
    while (sz < n + m - 1) sz *= 2;

    // sz = 2^k のときの高速化。分割統治的なやつで損しまくるので。

    if ((n + m - 3) <= sz / 2) {
        auto a_last = a.back(), b_last = b.back();
        a.pop_back(), b.pop_back();
        auto c = convolution(a, b);
        c.resize(n + m - 1);
        c[n + m - 2] = a_last * b_last;
        rep(i, sz(a)) c[i + sz(b)] += a[i] * b_last;
        rep(i, sz(b)) c[i + sz(a)] += b[i] * a_last;
        return c;
    }

    a.resize(sz), b.resize(sz);
    bool same = a == b;
    ntt(a, 0);
    if (same) {
        b = a;
    } else {
        ntt(b, 0);
    }
    rep(i, sz) a[i] *= b[i];
    ntt(a, 1);
    a.resize(n + m - 1);
    return a;
}

template <typename mint>
vector<mint> convolution_garner(const vector<mint>& a, const vector<mint>& b) {
    int n = sz(a), m = sz(b);
    if (!n || !m) return {};
    static constexpr int p0 = 167772161;
    static constexpr int p1 = 469762049;
    static constexpr int p2 = 754974721;
    using mint0 = modint<p0>;
    using mint1 = modint<p1>;
    using mint2 = modint<p2>;
    V<mint0> a0(n), b0(m);
    V<mint1> a1(n), b1(m);
    V<mint2> a2(n), b2(m);
    rep(i, n) a0[i] = a[i].val, a1[i] = a[i].val, a2[i] = a[i].val;
    rep(i, m) b0[i] = b[i].val, b1[i] = b[i].val, b2[i] = b[i].val;
    auto c0 = convolution_ntt<mint0>(a0, b0);
    auto c1 = convolution_ntt<mint1>(a1, b1);
    auto c2 = convolution_ntt<mint2>(a2, b2);
    V<mint> c(sz(c0));
    rep(i, n + m - 1) {
        c[i] = CRT3<mint, p0, p1, p2>(c0[i].val, c1[i].val, c2[i].val);
    }
    return c;
}

vector<ll> convolution(vector<ll> a, vector<ll> b) {
    int32_t n = sz(a), m = sz(b);
    if (!n || !m) return {};
    if (min(n, m) <= 2500) return convolution_naive(a, b);

    ll mi_a = Min(a), mi_b = Min(b);
    for (auto& x : a) x -= mi_a;
    for (auto& x : b) x -= mi_b;
    assert(Max(a) * Max(b) <= 1e18);

    auto Ac = prefSum(a), Bc = prefSum(b);
    vi res(n + m - 1);
    for (int32_t k = 0; k < n + m - 1; ++k) {
        int s = max(0, k - m + 1);
        int t = min(n, k + 1);
        res[k] += (t - s) * mi_a * mi_b;
        res[k] += mi_a * (Bc[k - s + 1] - Bc[k - t + 1]);
        res[k] += mi_b * (Ac[t] - Ac[s]);
    }

    static constexpr u32 MOD1 = 1004535809;
    static constexpr u32 MOD2 = 1012924417;
    using mint1 = modint<MOD1>;
    using mint2 = modint<MOD2>;

    V<mint1> a1(n), b1(m);
    V<mint2> a2(n), b2(m);
    rep(i, n) a1[i] = a[i], a2[i] = a[i];
    rep(i, m) b1[i] = b[i], b2[i] = b[i];

    auto c1 = convolution_ntt<mint1>(a1, b1);
    auto c2 = convolution_ntt<mint2>(a2, b2);

    rep(i, n + m - 1) { res[i] += CRT2<u64, MOD1, MOD2>(c1[i].val, c2[i].val); }
    return res;
}

template <typename mint>
V<mint> convolution(const V<mint>& a, const V<mint>& b) {
    int n = sz(a), m = sz(b);
    if (!n || !m) return {};
    if (mint::can_ntt()) {
        if (min(n, m) <= 50) return convolution_karatsuba<mint>(a, b);
        return convolution_ntt(a, b);
    }
    if (min(n, m) <= 200) return convolution_karatsuba<mint>(a, b);
    return convolution_garner(a, b);
}
