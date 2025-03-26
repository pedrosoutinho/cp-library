#pragma once
#include "primetable.hpp"

// cnt[i] = number of i's -> cnt[i] = how many numbers divide i
template <typename T> void divisor_zeta(V<T>& A) {
    assert(A[0] == 0);
    int N = sz(A) - 1;
    auto P = primetable(N);
    for (auto&& p : P) {
        rep(x, 1, N / p + 1) A[p * x] += A[x];
    }
}

template <typename T> void divisor_mobius(V<T>& A) {
    assert(A[0] == 0);
    int N = sz(A) - 1;
    auto P = primetable(N);
    for (auto&& p : P) {
        per(x, 1, N / p + 1) A[p * x] -= A[x];
    }
}

// cnt[i] = number of i's -> cnt[i] = how many numbers are divisible by i
template <typename T> void multiplier_zeta(V<T>& A) {
    assert(A[0] == 0);
    int N = sz(A) - 1;
    auto P = primetable(N);
    for (auto&& p : P) {
        per(x, 1, N / p + 1) A[x] += A[p * x];
    }
}

template <typename T> void multiplier_mobius(V<T>& A) {
    assert(A[0] == 0);
    int N = sz(A) - 1;
    auto P = primetable(N);
    for (auto&& p : P) {
        rep(x, 1, N / p + 1) A[x] -= A[p * x];
    }
}
