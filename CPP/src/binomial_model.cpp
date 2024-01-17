#include "../include/binomial_model.h"

double binomial_asset_pricing::european_option_binomial(double S0, double k,
                                                        double t, int n,
                                                        double r, double u,
                                                        char opttype) {

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
      value[i] = (p * value[i] + q * value[i + 1]) / disc;
    }
  }

  return value[0];
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
      value[i] =
          (price[i] - k > 0 ? price[i] - k : 0) * (price[i] < H); // call option
    else
      value[i] =
          (k - price[i] > 0 ? k - price[i] : 0) * (price[i] < H); // put option
  }

  // backward recurssion
  for (int i = n; i > 0; i--) {
    for (int j = 0; j < i; j++) {
      if (price[i] < H)
        value[i] = (p * value[i] + q * value[i + 1]) / disc;
      else
        value[i] = 0;

      price[i] /= u;
    }
  }

  return value[0];
}
