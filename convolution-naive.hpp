#pragma once

template <class T, typename enable_if<!has_mod<T>::value>::type* = nullptr>
V<T> convolution_naive(const V<T>& a, const V<T>& b) {
    int n = sz(a), m = sz(b);
    if (n > m) return convolution_naive<T>(b, a);
    if (n == 0) return {};
    vector<T> ans(n + m - 1);
    rep(i, n) rep(j, m) ans[i + j] += a[i] * b[j];
    return ans;
}

template <class T, typename enable_if<has_mod<T>::value>::type* = nullptr>
V<T> convolution_naive(const V<T>& a, const V<T>& b) {
    int32_t n = sz(a), m = sz(b);
    if (n > m) return convolution_naive<T>(b, a);
    if (n == 0) return {};
    V<T> ans(n + m - 1);
    if (n <= 16 && (T::get_mod() < (1 << 30))) {
        for (int32_t k = 0; k < n + m - 1; ++k) {
            int32_t s = max(0, k - m + 1);
            int t = min(n, k + 1);
            u64 sm = 0;
            for (int i = s; i < t; ++i) {
                sm += u64(a[i].val) * (b[k - i].val);
            }
            ans[k] = sm;
        }
    } else {
        for (int32_t k = 0; k < n + m - 1; ++k) {
            int s = max(0, k - m + 1);
            int t = min(n, k + 1);
            u128 sm = 0;
            for (int i = s; i < t; ++i) {
                sm += u64(a[i].val) * (b[k - i].val);
            }
            ans[k] = T::raw(sm % T::get_mod());
        }
    }
    return ans;
}
