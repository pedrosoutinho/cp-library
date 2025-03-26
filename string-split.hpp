#pragma once

V<string> split(string S, char sep = ',') {
    V<string> res = {""};
    for (auto&& s : S) {
        if (s == sep)
            res.eb("");
        else
            res.back() += s;
    }
    return res;
}

V<string> split(string S, string seps = " ,") {
    V<string> res = {""};
    for (auto&& s : S) {
        if (count(all(seps), s))
            res.eb("");
        else
            res.back() += s;
    }
    return res;
}
