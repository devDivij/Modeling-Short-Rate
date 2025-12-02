#include <iostream>
#include <random>
#include <vector>
using namespace std;

double normal_random_variable(int n, mt19937 &gen)
{
    normal_distribution<double> dist(0.0, 1.0);
    double z = dist(gen);
    return z;
}
double chi_square_random_variable(double d, mt19937 &gen)
{
    chi_squared_distribution<double> dist(d);
    double x = dist(gen);
    return x;
}
double poisson_random_variable(double lambda, mt19937 &gen)
{
    poisson_distribution<int> dist(lambda);
    double N = dist(gen);
    return N;
}
vector<double> short_rate(int n, double r0, double T, double volatility, double alpha, double b)
{
    double d = (4 * b * alpha) / pow(volatility, 2);
    vector<double> r(n + 1);
    double dt = T / n;

    r[0] = r0;

    double c = pow(volatility, 2) * (1 - exp(-alpha * dt)) / (4 * alpha);

    random_device rd;
    mt19937 gen(rd());
    if (d > 1)
    {
        for (int i = 0; i < n; ++i)
        {
            double Z = normal_random_variable(n, gen);
            double X = chi_square_random_variable(d - 1.0, gen);
            double lambda = r[i] * exp(-alpha * dt) / c;
            r[i + 1] = c * (pow((Z + sqrt(lambda)), 2) + X);
        }
    }
    else if (d <= 1)
    {

        for (int i = 0; i < n; ++i)
        {
            double lambda = r[i] * exp(-alpha * dt) / c;
            double N = poisson_random_variable(lambda / 2, gen);
            double X = chi_square_random_variable(d + 2 * N, gen);
            r[i + 1] = c * X;
        }
    }

    return r;
}

int main()
{
    int n = 50;
    double T = 5.0;
    vector<double> r = short_rate(n, 5.0, T, 1.0, 1.0, 4.0);

    return 0;
}