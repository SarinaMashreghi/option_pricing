#ifndef BINOMIAL_HEADER_H
#define BINOMIAL_HEADER_H

#include <bits/stdc++.h>

using namespace std;

class binomial_asset_pricing {

public:
  /*
  binomial_asset_pricing(double S0, double k, double r, double u, char opttype);

  binomial_asset_pricing(double S0, double k, double r, double mu, double sigma,
                         char opttype);
  */

  vector<double> european_option_binomial(double initial_price, double strike,
                                          double interest_rate,
                                          double volatility, char opt_type,
                                          string method, int time,
                                          int time_steps);
  double CRR(double sigma, double dt);

  // vector<double> european_option_binomial(double S0, double k, double t, int
  // n,
  //                                         double r, double u, char opttype);

  double barrier_option_binomial(double S0, double k, double H, double t, int n,
                                 double r, double u, char opttype);

  // vector<double> CRR(double initial_price, double strike, double interest,
  //                    double volatility, double time, int n_steps, char
  //                    opttype);

private:
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

  double m_initial_price;
  double m_strike_price;
  int m_time;
  int m_time_steps;
  double m_interest_rate;
  double m_up_factor;
  double m_down_factor;
  char m_opt_type;
  double m_discount;
  double m_dt;
  double m_p_risk_neutral;
  double m_q_risk_neutral;
};

#endif // BINOMIAL_HEADER_H
