// g++ -std=c++17 -I src/pixel tests/test_raster.cpp -o tests/test_raster
#include "ellipse_raster.h"

#include <cassert>
#include <cstdio>
#include <set>

using pixeltools::Point;

static void checkSymmetry(int w, int h, const std::set<Point> &pts)
{
    for (const auto &p : pts) {
        assert(pts.count({w - 1 - p.first, p.second}) && "x-symmetry broken");
        assert(pts.count({p.first, h - 1 - p.second}) && "y-symmetry broken");
    }
}

int main()
{
    using namespace pixeltools;

    assert((ellipseOutline(1, 5) == std::set<Point>{{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}}));
    assert((ellipseOutline(5, 1) == std::set<Point>{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}}));
    assert(ellipseOutline(0, 5).empty());

    const int sizes[][2] = {{3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 5}, {8, 5}, {15, 15}, {16, 16}, {20, 12}, {6, 20}};
    for (const auto &s : sizes) {
        const int w = s[0], h = s[1];
        const auto o = ellipseOutline(w, h);
        const auto f = filledEllipse(w, h);
        assert(!o.empty() && "empty outline");
        for (const auto &p : o) {
            assert(p.first >= 0 && p.first < w && p.second >= 0 && p.second < h);
            assert(f.count(p) && "outline not subset of fill");
        }
        checkSymmetry(w, h, o);
        checkSymmetry(w, h, f);
    }

    const int sided[][2] = {{7, 5}, {8, 6}, {16, 16}};
    for (const auto &s : sided) {
        const auto o = ellipseOutline(s[0], s[1]);
        bool t = false, b = false, l = false, r = false;
        for (const auto &p : o) {
            if (p.second == 0)
                t = true;
            if (p.second == s[1] - 1)
                b = true;
            if (p.first == 0)
                l = true;
            if (p.first == s[0] - 1)
                r = true;
        }
        assert(t && b && l && r);
    }

    assert((ellipseOutline(5, 5) == std::set<Point>{
        {1, 0}, {2, 0}, {3, 0},
        {0, 1}, {4, 1},
        {0, 2}, {4, 2},
        {0, 3}, {4, 3},
        {1, 4}, {2, 4}, {3, 4},
    }));

    std::puts("OK - all C++ raster checks passed");
    return 0;
}
