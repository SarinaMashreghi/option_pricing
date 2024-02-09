#include "../include/binomial_model.h"

vector<double> binomial_asset_pricing::european_option_binomial(
    double S0, double k, double t, int n, double r, double u, char opttype) {

  double dt = t / n;
  double disc = exp(dt * r);
  double d = 1 / u;

  // risk-neutral probabilities
  double p = (disc - d) / (u - d);
  double q = 1 - p;

  vector<double> price(n + 1, S0 * pow(u, n));

  for (int i = 1; i <= n; i++) {
    price[i] = price[i - 1] * d / u;
  }

  vector<double> value(n + 1);

  for (int i = 0; i <= n; i++) {
    if (opttype == 'C')
      value[i] = price[i] - k > 0 ? price[i] - k : 0; // call option
    else
      value[i] = k - price[i] > 0 ? k - price[i] : 0; // put option
  }

  for (int i = n; i > 0; i--) {
    for (int j = 0; j < i; j++) {
      value[j] = (p * value[j] + q * value[j + 1]) / disc;
    }
  }

  return value;
}

double binomial_asset_pricing::barrier_option_binomial(double S0, double k,
                                                       double H, double t,
                                                       int n, double r,
                                                       double u, char opttype) {
  double dt = t / n;
  double disc = exp(dt * r);
  double d = 1 / u;

  // risk-neutral probabilities
  double p = (disc - d) / (u - d);
  double q = 1 - p;

  vector<double> price(n + 1, S0 * pow(u, n));

  for (int i = 1; i <= n; i++) {
    price[i] = price[i - 1] * d / u;
  }

  vector<double> value(n + 1);

  for (int i = 0; i <= n; i++) {
    if (opttype == 'C')
      value[i] = (price[i] - k > 0 ? price[i] - k : 0) *
                 (price[i] <= H); // call option
    else
      value[i] =
          (k - price[i] > 0 ? k - price[i] : 0) * (price[i] >= H); // put option
  }

  // backward recurssion
  for (int i = n; i > 0; i--) {
    for (int j = 0; j < i; j++) {
      if (price[i] < H)
        value[j] = (p * value[j] + q * value[j + 1]) / disc;
      else
        value[j] = 0;

      price[j] /= u;
    }
  }

  return value[0];
}

// setting parameters
vector<double> binomial_asset_pricing::CRR(double initial_price, double strike,
                                           double interest, double volatility,
                                           double time, int n_steps,
                                           char opttype) {
  double dt = time / n_steps;
  double u = exp(volatility * dt);

  return european_option_binomial(initial_price, strike, time, n_steps,
                                  interest, u, opttype);
}
