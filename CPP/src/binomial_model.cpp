#include "../include/binomial_model.h"

binomial_asset_pricing::binomial_asset_pricing(double S0, double k, double r,
                                               double u, char opttype) {

  this->m_initial_price = S0;
  this->m_strike_price = k;
  this->m_interest_rate = r;

  this->m_up_factor = u;
  this->m_down_factor = 1 / this->m_up_factor;
  this->m_opt_type = opttype;
}

binomial_asset_pricing::binomial_asset_pricing(double S0, double k, double r,
                                               double mu, double sigme,
                                               char opttype) {

  this->m_initial_price = S0;
  this->m_strike_price = k;
  this->m_interest_rate = r;
}

double binomial_asset_pricing::CRR(double mu, double sigma) {}

vector<double>
binomial_asset_pricing::european_option_binomial(int time, int time_steps) {

  double dt = time / time_steps;
  double disc = exp(dt * m_interest_rate);
  double d = 1 / m_up_factor;

  // risk-neutral probabilities
  double p = (disc - d) / (m_up_factor - d);
  double q = 1 - p;

  vector<double> price(time_steps + 1,
                       m_initial_price * pow(m_up_factor, time_steps));

  for (int i = 1; i <= time_steps; i++) {
    price[i] = price[i - 1] * d / m_up_factor;
  }

  vector<double> value(time_steps + 1);

  for (int i = 0; i <= time_steps; i++) {
    if (m_opt_type == 'C')
      value[i] = price[i] - m_strike_price > 0 ? price[i] - m_strike_price
                                               : 0; // call option
    else
      value[i] = m_strike_price - price[i] > 0 ? m_strike_price - price[i]
                                               : 0; // put option
  }

  for (int i = time_steps; i > 0; i--) {
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
/*
vector<double> binomial_asset_pricing::CRR(double initial_price, double strike,
                                           double interest, double volatility,
                                           double time, int n_steps,
                                           char opttype) {
  double dt = time / n_steps;
  double u = exp(volatility * dt);

  return european_option_binomial(initial_price, strike, time, n_steps,
                                  interest, u, opttype);
}
*/
