#include "../include/binomial_model.h"

int binomial_asset_pricing::binomial_tree(double S0, double k, double t, int n,
                                          double r, double u, char type) {

  double dt = t / n;
  double disc = type == 'E' ? exp(dt * r) : 1 + r;
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
    value[i] = price[i] - k > 0 ? price[i] - k : 0;
  }

  for (int i = n; i > 0; i--) {
    for (int j = 0; j < i; j++) {
      value[i] = (p * value[i] + q * value[i + 1]) / disc;
    }
  }

  return value[0];
}
