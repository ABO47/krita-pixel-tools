#pragma once

#include <cmath>
#include <set>
#include <utility>

namespace pixeltools
{

using Point = std::pair<int, int>;

inline int roundHalfUp(double v)
{
    return static_cast<int>(std::floor(v + 0.5));
}

// 1px outline inscribed in a w*h box, 0 <= x < w, 0 <= y < h.
inline std::set<Point> ellipseOutline(int w, int h)
{
    std::set<Point> pts;
    if (w <= 0 || h <= 0)
        return pts;
    if (w == 1) {
        for (int y = 0; y < h; ++y)
            pts.emplace(0, y);
        return pts;
    }
    if (h == 1) {
        for (int x = 0; x < w; ++x)
            pts.emplace(x, 0);
        return pts;
    }

    const double cx = (w - 1) / 2.0;
    const double cy = (h - 1) / 2.0;
    const double rx = (w - 1) / 2.0;
    const double ry = (h - 1) / 2.0;

    // Mirror instead of rounding both sides: half-up rounding breaks
    // symmetry about half-integer centers (even sizes).
    for (int x = 0; x < w; ++x) {
        const double t = 1.0 - std::pow((x - cx) / rx, 2);
        if (t < 0.0)
            continue;
        const int yt = roundHalfUp(cy - ry * std::sqrt(t));
        const int yb = (h - 1) - yt;
        pts.emplace(x, yt);
        pts.emplace(x, yb);
    }
    // Second sweep closes the gaps the first leaves on the side arcs.
    for (int y = 0; y < h; ++y) {
        const double t = 1.0 - std::pow((y - cy) / ry, 2);
        if (t < 0.0)
            continue;
        const int xl = roundHalfUp(cx - rx * std::sqrt(t));
        const int xr = (w - 1) - xl;
        pts.emplace(xl, y);
        pts.emplace(xr, y);
    }

    // Rounding can step 1px outside on tiny boxes.
    std::set<Point> clean;
    for (const auto &p : pts) {
        if (p.first >= 0 && p.first < w && p.second >= 0 && p.second < h)
            clean.insert(p);
    }
    return clean;
}

// Center-based fill, so even sizes keep a symmetric 2px pole.
inline std::set<Point> filledEllipse(int w, int h)
{
    std::set<Point> pts;
    if (w <= 0 || h <= 0)
        return pts;

    const double cx = w / 2.0;
    const double cy = h / 2.0;
    const double rx = w / 2.0;
    const double ry = h / 2.0;

    for (int y = 0; y < h; ++y) {
        const double t = 1.0 - std::pow((y + 0.5 - cy) / ry, 2);
        if (t < 0.0)
            continue;
        const double dx = rx * std::sqrt(t);
        int x0 = std::max(0, static_cast<int>(std::ceil(cx - dx - 0.5)));
        int x1 = std::min(w - 1, static_cast<int>(std::floor(cx + dx - 0.5)));
        for (int x = x0; x <= x1; ++x)
            pts.emplace(x, y);
    }
    return pts;
}

} // namespace pixeltools
