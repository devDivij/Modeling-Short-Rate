#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

using namespace std;

struct MarketQuote
{
    double time;
    double rate;
};

vector<MarketQuote> yield_curve = {
    {0.0, 0.040},
    {0.5, 0.040},
    {1.0, 0.045},
    {2.0, 0.050},
    {5.0, 0.055}};

double get_interpolated_spot(double t)
{
    if (t <= 0.0001)
        return yield_curve[0].rate;
    for (size_t i = 0; i < yield_curve.size() - 1; i++)
    {
        double t1 = yield_curve[i].time;
        double t2 = yield_curve[i + 1].time;
        if (t >= t1 && t <= t2)
        {
            double r1 = yield_curve[i].rate;
            double r2 = yield_curve[i + 1].rate;
            return r1 + (r2 - r1) * ((t - t1) / (t2 - t1));
        }
    }
    return yield_curve.back().rate;
}

double get_forward_rate(double t)
{
    double dt = 0.001;
    double r_a = get_interpolated_spot(t);
    double r_b = get_interpolated_spot(t + dt);
    return (r_b * (t + dt) - r_a * t) / dt;
}

vector<double> generate_random_sequence(int n)
{
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> dist(0.0, 1.0);
    vector<double> Z(n);
    for (int i = 0; i < n; ++i)
        Z[i] = dist(gen);
    return Z;
}

vector<double> short_rate(int n, double T, double volatility)
{
    vector<double> Z = generate_random_sequence(n);
    vector<double> r(n + 1);
    double dt = T / n;

    r[0] = get_forward_rate(0.0);

    for (int i = 0; i < n; ++i)
    {
        double t = i * dt;
        double fwd_current = get_forward_rate(t);
        double fwd_next = get_forward_rate(t + dt);
        r[i + 1] = r[i] + (fwd_next - fwd_current) + (volatility * volatility) * t * dt + volatility * sqrt(dt) * Z[i];
    }
    return r;
}