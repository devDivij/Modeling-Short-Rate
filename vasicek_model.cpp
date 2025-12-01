#include <iostream>
#include <random>
#include <vector>
using namespace std;

vector<double> generate_random_sequence(int n)
{
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> dist(0.0, 1.0);

    vector<double> Z(n, 0.0);
    for (int i = 0; i < n; ++i)
    {
        double z = dist(gen);
        Z[i] = z;
    }
    return Z;
}

vector<double> short_rate(int n, double r0, double T, double volatility, double alpha, double b)
{
    vector<double> Z = generate_random_sequence(n);
    vector<double> r(n + 1);
    double dt = T / n;

    r[0] = r0;

    double memory_coeff = exp(-alpha * dt);
    double mean_reversion_coeff = b * (1 - memory_coeff);
    double stochastic_coeff = volatility * sqrt((1 - exp(-2 * alpha * dt)) / (2 * alpha));

    for (int i = 0; i < n; ++i)
    {
        r[i + 1] = r[i] * memory_coeff + mean_reversion_coeff + stochastic_coeff * Z[i];
    }
    return r;
}

int main()
{
    int n = 50;
    double T = 5.0;
    vector<double> r = short_rate(n, 4.0, T, 0.1, 1.0, 4.2);
    return 0;
}