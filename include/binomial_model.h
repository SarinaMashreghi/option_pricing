#ifndef BINOMIAL_HEADER_H
#define BINOMIAL_HEADER_H

#include <bits/stdc++.h>

using namespace std;

class binomial_asset_pricing {

  /*
   * parameters:
   *  S0 = initial stock price
   *  k = strike price
   *  t = time to maturity
   *  n = number of time steps
   *  r = interest rate
   *  u = up factor
   *  d = 1/u
   *  type = type of interest
   *
   *  to avoid arbitrage: 0 < d < 1+r < u
   *
   *  Delta hedging formula: D0 = (V1(U)-V1(D)) / (S1(U), S1(D))
   * */

public:
  double european_option_binomial(double S0, double k, double t, int n,
                                  double r, double u, char opttype);
  double barrier_option_binomial(double S0, double k, double H, double t, int n,
                                 double r, double u, char opttype);
};

#endif // BINOMIAL_HEADER_H
