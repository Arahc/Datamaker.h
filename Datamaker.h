#ifndef _DATAMAKER_H_
#define _DATAMAKER_H_
#define VERSION "BETA0.3.2"
#endif

#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NO_VA_START_VALIDATION
#endif

/* Overrides random() for Borland C++. */
#define random __random_deprecated
#include <stdlib.h>
#include <cstdlib>
#include <climits>
#include <algorithm>
#undef random

#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <cstring>
#include <iostream>

const long long __TESTLIB_LONGLONG_MAX = 9223372036854775807LL;

template <typename T>
static inline T __testlib_min(const T &a, const T &b)
{
    return a < b ? a : b;
}

template <typename T>
static inline T __testlib_max(const T &a, const T &b)
{
    return a > b ? a : b;
}

class Random_t
{
private:
    unsigned long long seed;
    static const unsigned long long multiplier = 0x5DEECE66DLL;
    static const unsigned long long addend = 0xBLL;
    static const unsigned long long mask = (1LL << 48) - 1;
    static const int lim = 25;

    long long nextBits(int bits)
    {
        if (bits <= 48)
        {
            seed = (seed * multiplier + addend) & mask;
            return (long long)(seed >> (48 - bits));
        }
        else
        {
            long long left = (nextBits(31) << 32);
            long long right = nextBits(32);
            return left ^ right;
        }
    }

public:
    Random_t()
        : seed(3905348978240129619LL)
    {
    }

    void setSeed(int argc, char *argv[])
    {
        Random_t p;

        seed = 3905348978240129619LL;
        for (int i = 1; i < argc; i++)
        {
            std::size_t le = std::strlen(argv[i]);
            for (std::size_t j = 0; j < le; j++)
                seed = seed * multiplier + (unsigned int)(argv[i][j]) + addend;
            seed += multiplier / addend;
        }

        seed = seed & mask;
    }

    double rand(double from, double to)
    {
        long long left = ((long long)(nextBits(26)) << 27);
        long long right = nextBits(27);
        return ((double)(left + right) / (double)(1LL << 53)) * (to - from) + from;
    }

    template <typename T>
    T rand(T from, T to)
    {

        T n = to - from + 1;
        const long long limit = __TESTLIB_LONGLONG_MAX / n * n;

        long long bits;
        do
        {
            bits = nextBits(63);
        } while (bits >= limit);

        return from + T(bits % n);
    }

    template <typename Container>
    typename Container::value_type any(const Container &c)
    {
        size_t size = c.size();
        return *(c.begin() + rand((size_t)0, size - 1));
    }

    template <typename T>
    T rand(T from, T to, int type)
    {
        T n = to - from;
        if (abs(type) < Random_t::lim)
        {
            int result = rand(T(0), n);

            for (int i = 0; i < +type; i++)
                result = __testlib_max(result, rand(T(0), n));

            for (int i = 0; i < -type; i++)
                result = __testlib_min(result, rand(T(0), n));

            return from + result;
        }
        else
        {
            double p;

            if (type > 0)
                p = std::pow(rand(0.0, 1.0) + 0.0, 1.0 / (type + 1));
            else
                p = 1 - std::pow(rand(0.0, 1.0) + 0.0, 1.0 / (-type + 1));

            return from + int(n * p);
        }
    }

    template <typename Container>
    typename Container::value_type any(const Container &c, int type)
    {
        size_t size = c.size();
        return *(c.begin() + rand((size_t)0, size - 1, type));
    }

    template <typename T>
    std::vector<T> permutation(T first, T last)
    {
        T size = last - first + 1;
        std::vector<T> p(size);
        T current = first;
        for (T i = 0; i < size; i++)
            p[i] = current++;
        if (size > 1)
            for (T i = 1; i < size; i++)
                std::swap(p[i], p[rand(0, i - 1)]);
        return p;
    }

    template <typename T>
    std::vector<T> distinct(int size, T from, T to)
    {

        std::vector<T> result;
        if (size == 0)
            return result;

        uint64_t n = to - from + 1;

        double expected = 0.0;
        for (int i = 1; i <= size; i++)
            expected += double(n) / double(n - i + 1);

        if (expected < double(n))
        {
            std::set<T> vals;
            while (int(vals.size()) < size)
            {
                T x = T(rand(from, to));
                if (vals.insert(x).second)
                    result.push_back(x);
            }
        }
        else
        {
            std::vector<T> p(permutation(from, (T)(from + n - 1)));
            result.insert(result.end(), p.begin(), p.begin() + size);
        }
        return result;
    }

    template <typename T>
    std::vector<T> partition(int size, T sum, T min_part)
    {
        if (size == 0 && sum == 0)
            return std::vector<T>();

        T sum_ = sum;
        sum -= min_part * size;

        std::vector<T> septums(size);
        std::vector<T> d = distinct(size - 1, T(1), T(sum + size - 1));
        for (int i = 0; i + 1 < size; i++)
            septums[i + 1] = d[i];
        std::sort(septums.begin(), septums.end());

        std::vector<T> result(size);
        for (int i = 0; i + 1 < size; i++)
            result[i] = septums[i + 1] - septums[i] - 1;
        result[size - 1] = sum + size - 1 - septums.back();

        for (std::size_t i = 0; i < result.size(); i++)
            result[i] += min_part;

        T result_sum = 0;
        for (std::size_t i = 0; i < result.size(); i++)
            result_sum += result[i];

        return result;
    }
    template <typename _RandomAccessIter>
    void shuffle(_RandomAccessIter __first, _RandomAccessIter __last)
    {
        if (__first == __last)
            return;
        for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
            std::iter_swap(__i, __first + rand(0, int(__i - __first)));
    }
} rnd;

void registerGen(int argc, char *argv[])
{
    rnd.setSeed(argc, argv);
}

FILE *startTestIn(std::string filename)
{
    return freopen(filename.c_str(), "w", stdout);
}
void startTestOut(std::string filename)
{
    freopen(filename.c_str(), "w", stdout);
}

void autoGenerate(std::string proname, std::vector<int> subtasks, void (*Generate)(int, int), std::string stdname = "", std::string valname = "", std::string inputextension = "in", std::string outputextension = "out")
{
    std::string startcommand = "./";
    std::string stdrname = startcommand + stdname;
    std::string valiname = startcommand + valname;
    for (size_t i = 0; i < subtasks.size(); ++i)
        for (int j = 1; j <= subtasks[i]; ++j)
        {
            std::string filename = proname + (subtasks.size() == (size_t)1 ? "" : std::to_string(i + 1) + "-") + std::to_string(j);
            startTestIn(filename + "." + inputextension);
            Generate(i + 1, j);
            std::fflush(stdout);
            if (valiname != startcommand)
            {
                valname = valiname + " < " + filename + "." + inputextension;
                #ifdef _WIN64
                    valname = "powershell \"Get-Content " + filename + "." + inputextension + " | " + valiname + "\"";
                #endif
                if (system(valname.c_str()))
                {
                    if (subtasks.size() == (size_t)1)
                        std::cerr << "Error(illegal data) on : " << proname << j << std::endl;
                    else
                        std::cerr << "Error(illegal data) on : " << proname << i + 1 << "-" << j << std::endl;
                    return;
                }
            }
            if (stdrname != startcommand)
            {
                stdname = stdrname + " < " + filename + "." + inputextension + " > " + filename + "." + outputextension;
                #ifdef _WIN64
                    startTestOut(filename + "." + outputextension);
                    stdname = "powershell \"Get-Content " + filename + "." + inputextension + " | " + stdrname + "\"";
                #endif
                if (system(stdname.c_str()))
                {
                    if (subtasks.size() == (size_t)1)
                        std::cerr << "Error(std failed) on : " << proname << j << std::endl;
                    else
                        std::cerr << "Error(std failed) on : " << proname << i + 1 << "-" << j << std::endl;
                    return;
                }
            }
        }
}

template <typename T = int>
struct Edge
{
    int u, v;
    T w;
    Edge(int a = 0, int b = 0, T c = T(0)) : u(a), v(b), w(c) {}
    ~Edge() {}

    bool operator==(Edge<T> b) const
    {
        Edge a = *this;
        return (a.u == b.u && a.v == b.v && a.w == b.w);
    }
    bool operator<(Edge<T> b) const
    {
        Edge a = *this;
        if (a.w == b.w)
            return a.u == b.u ? a.v < b.v : a.u < b.u;
        return a.w < b.w;
    }
    void reverse()
    {
        u ^= v ^= u ^= v;
    }
};

template <typename T = int>
class Tree
{
private:
    size_t n;
    std::vector<Edge<T>> edge;
    void swapPoint(int a, int b)
    {
        if (a == b)
            return;
        for (int i = 0; i < n - 1; ++i)
        {
            if (edge[i].u == a)
                edge[i].u = b;
            else if (edge[i].u == b)
                edge[i].u = a;
            if (edge[i].v == a)
                edge[i].v = b;
            else if (edge[i].v == b)
                edge[i].v = a;
        }
    }
    std::vector<std::vector<int>> vec_g;
    void dir_dfs(int u, int fa, bool op)
    {
        for (auto i : vec_g[u])
        {
            if (edge[i].u == fa || edge[i].v == fa)
                continue;
            if (op)
            {
                if (edge[i].v == u)
                    edge[i].reverse();
            }
            else
            {
                if (edge[i].u == u)
                    edge[i].reverse();
            }
            dir_dfs(op ? edge[i].v : edge[i].u, u, op);
        }
    }

public:
    Tree(size_t _n = 1)
    {
        n = _n;
        edge.clear();
        vec_g.clear();
        edge.resize(_n - 1);
    }
    ~Tree()
    {
        vec_g.clear();
        edge.clear();
    }
    Edge<T> &operator[](int i)
    {
        return edge[i];
    }
    size_t size()
    {
        return n;
    }
    std::vector<int> shuffle(bool fl = 1, bool op = 1)
    {
        rnd.shuffle(edge.begin(), edge.end());
        std::vector<int> pos;
        for (int i = 0; i < n; ++i)
            pos.emplace_back(i);
        if (fl)
        {
            rnd.shuffle(pos.begin(), pos.end());
            for (int i = 0; i < n - 1; ++i)
                edge[i].u = pos[edge[i].u], edge[i].v = pos[edge[i].v];
        }
        for (int i = 0; i < n - 1; ++i)
            edge[i].u = pos[edge[i].u], edge[i].v = pos[edge[i].v];
        if (!op)
            for (int i = 0; i < n - 1; ++i)
                if (rnd.rand(0, 1) & 1)
                    edge[i].reverse();
        return pos;
    }
    void random1(
        int t1 = 0, T (*Weight)() = []
                    { return T(0); })
    {
        for (int i = 0; i < n - 1; ++i)
            edge[i] = Edge<T>(i + 1, rnd.rand(0, i, t1), Weight());
    }
    void random2(
        double L1 = 0, double R1 = 1, T (*Weight)() = []
                                      { return T(0); })
    {
        for (int i = 0; i < n - 1; ++i)
            edge[i] = Edge<T>(i + 1, rnd.rand((int)floor(L1 * i), (int)ceil(R1 * i)), Weight());
    }
    void random3(
        int k = 1, T (*Weight)() = []
                   { return T(0); })
    {
        for (int i = 0; i < n - 1; ++i)
            edge[i] = Edge<T>(i + 1, rnd.rand(i < k - 1 ? 0 : i - k + 1, i), Weight());
    }
    void chain(
        int rt = 0, T (*Weight)() = []
                    { return T(0); })
    {
        for (int i = 0; i < n - 1; ++i)
            edge[i] = Edge<T>((rt + i) % n, (rt + i + 1) % n, Weight());
    }
    void star(
        int rt = 0, T (*Weight)() = []
                    { return T(0); })
    {
        for (int i = 0, j = 0; i < n; ++i)
            if (i != rt)
                edge[j++] = Edge<T>(rt, i, Weight());
    }
    void flower(
        int rt = 0, T (*Weight)() = []
                    { return T(0); })
    {
        for (int i = 0, j = 0; i < n; ++i)
            if (i != rt)
                edge[j++] = Edge<T>(rt, i, Weight());
    }
    void limchild(
        int rt = 0, int K = 2, T (*Weight)() = []
                               { return T(0); })
    {
        int *s = new int[n * K], tp = 0;
        for (int i = 0; i < K; ++i)
            s[tp++] = rt;
        for (int i = 1; i < n; ++i)
        {
            int ps = s[rnd.rand(0, tp - 1)];
            int fa = s[ps];
            edge[i - 1] = Edge<T>(fa, i, Weight());
            s[ps] ^= s[tp - 1] ^= s[ps] ^= s[tp - 1];
            s[--tp] = 0;
            for (int j = 0; j < K; ++j)
                s[tp++] = i;
        }
        delete[] s;
        swapPoint(0, rt);
    }
    void complete(
        int rt = 0, int K = 2, T (*Weight)() = []
                               { return T(0); })
    {
        for (int i = 1; i < n; ++i)
            edge[i - 1] = Edge<T>((i - 1) / K, i, Weight());
        swapPoint(0, rt);
    }
    void silkworm(
        int rt = 0, T (*Weight)() = []
                    { return T(0); })
    {
        int m = (n + 1) >> 1;
        for (int i = 0; i < m - 1; ++i)
            edge[i] = Edge<T>(i, i + 1, Weight());
        for (int i = m, j = 0; i < n; ++i, ++j)
            edge[i - 1] = Edge<T>(j, i, Weight());
        swapPoint(0, rt);
    }
    void redirect(int rt = 0, bool op = 1)
    {
        vec_g.clear();
        vec_g.resize(n);
        for (int i = 0; i < n - 1; ++i)
        {
            vec_g[edge[i].u].emplace_back(i);
            vec_g[edge[i].v].emplace_back(i);
        }
        dir_dfs(rt, -1, op);
        vec_g.clear();
    }
    void addpoint(int rt = 0, T w = T(0))
    {
        edge.emplace_back(rt, n, w);
        ++n;
    }
    void merge(Tree<T> b, int conA = 0, int conB = 0, T w = T(0))
    {
        for (int i = 0; i < b.n - 1; ++i)
            edge.emplace_back(b.edge[i].u + n, b.edge[i].v + n, b.edge[i].w);
        edge.emplace_back(conA, conB + n, w);
        n += b.n;
    }
    void expand(Tree<T> b, int con1 = 0, int con2 = 0)
    {
        for (int i = 0; i < n - 1; ++i)
            edge[i].u *= b.n, edge[i].v *= b.n;
        for (int i = 0; i < n - 1; ++i)
        {
            if (con1 == -1)
                edge[i].u += rnd.rand(0, b.n - 1);
            else
                edge[i].u += con1;
            if (con2 == -1)
                edge[i].v += rnd.rand(0, b.n - 1);
            else
                edge[i].v += con2;
        }
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < b.n - 1; ++j)
                edge.emplace_back(b.edge[j].u + i * b.n, b.edge[j].v + i * b.n, b.edge[j].w);
        n *= b.n;
    }
};

template <typename T = int>
class Graph
{
private:
    size_t n, m;
    std::vector<Edge<T>> edge;
    std::vector<std::vector<int>> vec_g;

public:
    Graph(size_t _n = 1, size_t _m = 0)
    {
        n = _n, m = _m;
        edge.clear();
        vec_g.clear();
        edge.resize(m);
    }
    ~Graph()
    {
        vec_g.clear();
        edge.clear();
    }
    Edge<T> &operator[](int i)
    {
        return edge[i];
    }
    std::pair<size_t, size_t> size()
    {
        return std::make_pair(n, m);
    }
    std::vector<int> shuffle(bool fl = 1, bool op = 1)
    {
        rnd.shuffle(edge.begin(), edge.end());
        std::vector<int> pos;
        for (int i = 0; i < n; ++i)
            pos.emplace_back(i);
        if (fl)
        {
            rnd.shuffle(pos.begin(), pos.end());
            for (int i = 0; i < m; ++i)
                edge[i].u = pos[edge[i].u], edge[i].v = pos[edge[i].v];
        }
        if (!op)
            for (int i = 0; i < m; ++i)
                if (rnd.rand(0, 1) & 1)
                    edge[i].reverse();
        return pos;
    }
    void random1(
        bool op = 0, T (*Weight)() = []
                     { return T(0); })
    {
        std::set<std::pair<int, int>> mp;
        if (op)
        {
            for (int i = 0; i < m; ++i)
            {
                int u = rnd.rand(0, n - 1), v = rnd.rand(0, n - 1);
                while (u == v || mp.count(std::make_pair(u, v)))
                    u = rnd.rand(0, n - 1), v = rnd.rand(0, n - 1);
                mp.insert(std::make_pair(u, v));
                edge[i] = Edge<T>(u, v, Weight());
            }
        }
        else
        {
            for (int i = 0; i < m; ++i)
            {
                int u = rnd.rand(0, n - 1), v = rnd.rand(0, n - 1);
                while (u == v || mp.count(std::minmax(u, v)))
                    u = rnd.rand(0, n - 1), v = rnd.rand(0, n - 1);
                mp.insert(std::minmax(u, v));
                edge[i] = Edge<T>(u, v, Weight());
            }
        }
        mp.clear();
    }
    void random2(
        bool op = 0, T (*Weight)() = []
                     { return T(0); })
    {
        std::set<std::pair<int, int>> mp;
        for (int i = 1; i < n; ++i)
        {
            int fa = rnd.rand(0, i - 1);
            edge[i - 1] = Edge<T>(fa, i, Weight());
            mp.insert(std::make_pair(fa, i));
        }
        if (op)
        {
            for (int i = n - 1; i < m; ++i)
            {
                int u = rnd.rand(0, n - 1), v = rnd.rand(0, n - 1);
                while (u == v || mp.count(std::make_pair(u, v)))
                    u = rnd.rand(0, n - 1), v = rnd.rand(0, n - 1);
                mp.insert(std::make_pair(u, v));
                edge[i] = Edge<T>(u, v, Weight());
            }
        }
        else
        {
            for (int i = n - 1; i < m; ++i)
            {
                int u = rnd.rand(0, n - 1), v = rnd.rand(0, n - 1);
                while (u == v || mp.count(std::minmax(u, v)))
                    u = rnd.rand(0, n - 1), v = rnd.rand(0, n - 1);
                mp.insert(std::minmax(u, v));
                edge[i] = Edge<T>(u, v, Weight());
            }
        }
        mp.clear();
    }
    void DAG(
        std::vector<int> vec = {-1}, T (*Weight)() = []
                                     { return T(0); })
    {
        std::set<std::pair<int, int>> mp;
        if (vec[0] == -1)
        {
            vec.clear();
            for (int i = 0; i < n; ++i)
                vec.emplace_back(i);
            rnd.shuffle(vec.begin(), vec.end());
        }
        else
            n = vec.size();
        for (int i = 1; i < n; ++i)
        {
            int fa = rnd.rand(0, i - 1);
            edge[i - 1] = Edge<T>(vec[fa], vec[i], Weight());
            mp.insert(std::make_pair(fa, i));
        }
        for (int i = n - 1; i < m; ++i)
        {
            int u = rnd.rand(0, n - 1), v = rnd.rand(0, n - 1);
            while (u == v || mp.count(std::minmax(u, v)))
                u = rnd.rand(0, n - 1), v = rnd.rand(0, n - 1);
            if (u > v)
                u ^= v ^= u ^= v;
            mp.insert(std::minmax(u, v));
            edge[i] = Edge<T>(vec[u], vec[v], Weight());
        }
    }
    void complete(T (*Weight)() = []
                  { return T(0); })
    {
        m = n * (n - 1) / 2;
        edge.clear();
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
            {
                edge.emplace_back(i, j, Weight());
                if (rnd.rand(0, 1) & 1)
                    edge.back().reverse();
            }
    }
    void limdegree1(
        std::vector<int> vec, bool op = 1, T (*Weight)() = []
                                           { return T(0); })
    {
        n = vec.size();
        edge.clear();
        bool *vis = new bool[n];
        for (int i = 0; i < n; ++i)
            vis[i] = 0;
        for (int i = 0; i < n; ++i)
            if (vec[i])
            {
                std::vector<int> res = rnd.distinct(vec[i], 0, (int)n - 1);
                for (auto j : res)
                    vis[j] = 1;
                if (vis[i])
                {
                    for (int j = 0; j < n; ++j)
                        if (!vis[j])
                        {
                            vis[j] = 1;
                            res.emplace_back(j);
                            break;
                        }
                }
                for (auto j : res)
                {
                    if (j == i)
                        continue;
                    if (op)
                        edge.emplace_back(i, j, Weight());
                    else
                        edge.emplace_back(j, i, Weight());
                    vis[j] = 0;
                }
            }
        m = edge.size();
        delete[] vis;
    }
    void limdegree2(
        std::vector<int> vec, T (*Weight)() = []
                              { return T(0); })
    {
        n = vec.size();
        edge.clear();
        std::vector<int> del;
        std::set<std::pair<int, int>> st;
        for (int i = 0; i < n; ++i)
            st.emplace(vec[i], i);
        while (!st.empty())
        {
            int u = st.begin()->second;
            st.erase(st.begin());
            if (!vec[u])
                continue;
            del.clear();
            for (auto it = st.rbegin(); it != st.rend(); ++it)
            {
                int v = it->second;
                edge.emplace_back(u, v, Weight());
                --vec[u], --vec[v];
                del.emplace_back(v);
                if (!vec[u])
                    break;
            }
            for (auto v : del)
            {
                st.erase(std::make_pair(vec[v] + 1, v));
                st.emplace(vec[v], v);
            }
        }
        m = edge.size();
        st.clear();
        del.clear();
    }
    void circle(T (*Weight)() = []
                { return T(0); })
    {
        if (n == 1)
        {
            m = 0;
            return;
        }
        m = n;
        edge.clear();
        for (int i = 0; i < n; ++i)
            edge.emplace_back(i, (i + 1) % n, Weight());
    }
    void bipartite(
        int M, T (*Weight)() = []
               { return T(0); })
    {
        std::set<std::pair<int, int>> mp;
        for (int i = 0; i < m; ++i)
        {
            int u = rnd.rand(0, M - 1), v = rnd.rand(M, n - 1);
            while (mp.count(std::make_pair(u, v)))
                u = rnd.rand(0, M - 1), v = rnd.rand(M, n - 1);
            mp.insert(std::make_pair(u, v));
            edge[i] = Edge<T>(u, v, Weight());
        }
    }
    void hackspfa(T L = T(0), T R = T(0))
    {
        int rw, cl, cnt = 0;
        for (rw = 1; rw < 10; ++rw)
            if (n * 2 - (n - 1) / rw - 1 - rw > m)
            {
                --rw;
                break;
            }
        cl = (n - 1) / rw + 1;
        std::vector<std::vector<int>> id;
        id.resize(rw);
        for (int i = 0; i < rw; ++i)
        {
            id[i].resize(cl);
            for (int j = 0; j < cl; ++j)
            {
                id[i][j] = (cnt >= n ? n - 1 : cnt);
                ++cnt;
            }
        }
        cnt = 0;
        for (int i = 0; i < rw; ++i)
            for (int j = 0; j < cl; ++j)
            {
                if (i < rw - 1)
                    edge[cnt++] = Edge<T>(id[i][j], id[i + 1][j], L);
                if (j < cl - 1)
                    edge[cnt++] = Edge<T>(id[i][j], id[i][j + 1], rnd.rand(L, R, 20));
            }
        if (cnt < m)
            for (int x = 1; cnt < m; ++x)
                for (int y = 1; cnt < m; ++y)
                    for (int i = 0; i < rw && cnt < m; ++i)
                        for (int j = 0; j < cl && cnt < m; ++j)
                            if (i + x < rw && j + y < cl)
                                edge[cnt++] = Edge<T>(id[i][j], id[i + x][j + y], rnd.rand(L, R, 20 + x + y));
        id.clear();
    }
    void redirect(int rt = 0, bool op = 1)
    {
        vec_g.clear();
        vec_g.resize(n);
        for (int i = 0; i < m; ++i)
        {
            vec_g[edge[i].u].emplace_back(i);
            vec_g[edge[i].v].emplace_back(i);
        }
        int L = 0, R = 0;
        int *q = new int[n];
        bool *vis = new bool[n];
        for (int i = 0; i < n; ++i)
            vis[i] = 0;
        q[0] = rt;
        vis[rt] = 1;
        while (L <= R)
        {
            int u = q[L++];
            for (auto i : vec_g[u])
            {
                int v = edge[i].u == u ? edge[i].v : edge[i].u;
                if (vis[v])
                    continue;
                if (op)
                {
                    if (edge[i].v == u)
                        edge[i].reverse();
                }
                else
                {
                    if (edge[i].u == u)
                        edge[i].reverse();
                }
                vis[v] = 1;
                q[++R] = v;
            }
        }
        vec_g.clear();
        delete[] q;
        delete[] vis;
        q = nullptr;
        vis = nullptr;
    }
    void merge(Graph<T> b, std::vector<Edge<T>> vec1 = {}, std::vector<Edge<T>> vec2 = {})
    {
        for (Edge<T> p : b.edge)
            edge.emplace_back(p.u + n, p.v + n, p.w);
        for (Edge<T> p : vec1)
            edge.emplace_back(p.u, p.v + n, p.w);
        for (Edge<T> p : vec2)
            edge.emplace_back(p.u + n, p.v, p.w);
        n += b.n, m += b.m + vec1.size() + vec2.size();
    }
    void merge(Tree<T> b, std::vector<Edge<T>> vec1 = {}, std::vector<Edge<T>> vec2 = {})
    {
        for (Edge<T> p : b.edge)
            edge.emplace_back(p.u + n, p.v + n, p.w);
        for (Edge<T> p : vec1)
            edge.emplace_back(p.u, p.v + n, p.w);
        for (Edge<T> p : vec2)
            edge.emplace_back(p.u + n, p.v, p.w);
        n += b.n, m += b.n - 1 + vec1.size() + vec2.size();
    }
};

template <typename T = int>
struct Vector
{
    T x, y;
    Vector(T X = T(0), T Y = T(0)) : x(X), y(Y) {}
    ~Vector() {}

    bool operator==(Vector b)
    {
        return x == b.x && y == b.y;
    }
    bool operator<(Vector b)
    {
        return atan2(y, x) < atan2(b.y, b.x);
    }
};

template <typename T = int>
class Polygon
{
private:
    size_t n;
    std::vector<Vector<T>> points;

public:
    Polygon(size_t _n = 0)
    {
        n = _n;
        points.clear();
        points.resize(n);
    }
    Polygon(std::vector<T> _points)
    {
        n = _points.size();
        points = _points;
    }
    ~Polygon()
    {
        points.clear();
    }
    Vector<T> &operator[](int i)
    {
        return points[i];
    }
    size_t size()
    {
        return n;
    }
    void random(T L = T(0), T R = T(0))
    {
        std::set<T> posX, posY;
        while (posX.size() != n)
            posX.insert(rnd.rand(L, R));
        while (posY.size() != n)
            posY.insert(rnd.rand(L, R));
        std::vector<T> Xvec, Yvec;
        T smx = T(0), smy = T(0);
        while (!posX.empty() && !posY.empty())
        {
            T x = *posX.begin(), y = *posY.begin();
            posX.erase(posX.begin()), posY.erase(posY.begin());
            smx += x, smy += y;
            Xvec.push_back(x), Yvec.push_back(y);
        }
        rnd.shuffle(Yvec.begin(), Yvec.end());

        smx = T(1.0 * smx / n), smy = T(1.0 * smy / n);
        std::vector<Vector<T>> vect;
        while (!Xvec.empty() && !Yvec.empty())
        {
            T x = Xvec.back(), y = Yvec.back();
            Xvec.pop_back(), Yvec.pop_back();
            vect.emplace_back(x - smx, y - smy);
        }
        std::sort(vect.begin(), vect.end());
        points.clear();
        while (!vect.empty())
        {
            points.emplace_back(vect.back().x + smx, vect.back().y + smy);
            vect.pop_back();
        }
    }
    void convex(T L = T(0), T R = T(0))
    {
        std::set<T> posX, posY;
        while (posX.size() != n)
            posX.insert(rnd.rand(L, R));
        while (posY.size() != n)
            posY.insert(rnd.rand(L, R));
        std::vector<T> Xvec, Yvec;
        T minX = *posX.begin(), maxX = *posX.rbegin();
        T minY = *posY.begin(), maxY = *posY.rbegin();
        posX.erase(prev(posX.end())), posX.erase(posX.begin());
        posY.erase(prev(posY.end())), posY.erase(posY.begin());
        T lastTop = minX, lastBot = minX, lastLeft = minY, lastRight = minY;
        while (!posX.empty() && !posY.empty())
        {
            T x = *posX.begin(), y = *posY.begin();
            posX.erase(posX.begin()), posY.erase(posY.begin());
            if (rnd.rand(0, 1))
            {
                Xvec.push_back(x - lastTop);
                lastTop = x;
            }
            else
            {
                Xvec.push_back(lastBot - x);
                lastBot = x;
            }
            if (rnd.rand(0, 1))
            {
                Yvec.push_back(y - lastLeft);
                lastLeft = y;
            }
            else
            {
                Yvec.push_back(lastRight - y);
                lastRight = y;
            }
        }
        Xvec.push_back(maxX - lastTop);
        Xvec.push_back(lastBot - maxX);
        Yvec.push_back(maxY - lastLeft);
        Yvec.push_back(lastRight - maxY);
        rnd.shuffle(Yvec.begin(), Yvec.end());

        std::vector<Vector<T>> vect;
        while (!Xvec.empty() && !Yvec.empty())
        {
            vect.emplace_back(Xvec.back(), Yvec.back());
            Xvec.pop_back(), Yvec.pop_back();
        }
        std::sort(vect.begin(), vect.end());

        T nowX = T(0), nowY = T(0);
        T minPolX = T(0), minPolY = T(0);
        points.clear();

        while (!vect.empty())
        {
            points.emplace_back(nowX, nowY);
            nowX += vect.back().x, nowY += vect.back().y;
            if (nowX < minPolX)
                minPolX = nowX;
            if (nowY < minPolY)
                minPolY = nowY;
            vect.pop_back();
        }
        vect.clear();
        T Xshift = minX - minPolX, Yshift = minY - minPolY;
        for (int i = 0; i < n; ++i)
            points[i].x += Xshift, points[i].y += Yshift;
    }
};

const struct Consts
{
    std::string Number = "0123456789";
    std::string letter = "abcdefghijklmnopqrstuvwxyz";
    std::string Letter = "ABCDEFGHIJKLMNOPQRSTUVWXTZ";
    std::string LETTERS = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxTyZz";
    std::string Character = "!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    std::string CharaSpace = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    int _1e4 = 10000;
    int _1e5 = 100000;
    int _1e6 = 1000000;
    int _1e7 = 10000000;
    int _1e8 = 100000000;
    int _1e9 = 1000000000;
    long long _1e10 = 10000000000LL;
    long long _1e11 = 100000000000LL;
    long long _1e12 = 1000000000000LL;
    long long _1e13 = 10000000000000LL;
    long long _1e14 = 100000000000000LL;
    long long _1e15 = 1000000000000000LL;
    long long _1e16 = 10000000000000000LL;
    long long _1e17 = 100000000000000000LL;
    long long _1e18 = 1000000000000000000LL;
    double Pi = acos(-1);
    double E = exp(1);
    int HackUnMap = 107897;
} cst;