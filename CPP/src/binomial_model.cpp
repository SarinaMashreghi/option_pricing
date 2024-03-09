#include "../include/binomial_model.h"
#include <stdexcept>

/*
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
*/

// check N and Booles rule
// (https://sites.google.com/view/vinegarhill-financelabs/black-scholes-merton)

// N(0,1) density

double f(double x) {

  double pi = 4.0 * atan(1.0);

  return exp(-x * x * 0.5) / sqrt(2 * pi);
}

// Boole's Rule

double Boole(double StartPoint, double EndPoint, int n) {

  vector<double> X(n + 1, 0.0);

  vector<double> Y(n + 1, 0.0);

  double delta_x = (EndPoint - StartPoint) / double(n);

  for (int i = 0; i <= n; i++)

  {

    X[i] = StartPoint + i * delta_x;

    Y[i] = f(X[i]);
  }

  double sum = 0;

  for (int t = 0; t <= (n - 1) / 4; t++)

  {

    int ind = 4 * t;

    sum +=

        (1 / 45.0) *
        (14 * Y[ind] + 64 * Y[ind + 1] + 24 * Y[ind + 2] +

         64 * Y[ind + 3] + 14 * Y[ind + 4]) *
        delta_x;
  }

  return sum;
}

// N(0,1) cdf by Boole's Rule

double N(double x) { return Boole(-10.0, x, 240); }

double binomial_asset_pricing::black_scholes_merton(double S, double K,
                                                    double T, double r,
                                                    double q, double v,
                                                    char opt_type) {

  double d = (log(S / K) + T * (r - q + 0.5 * v * v)) / (v * sqrt(T));
  double call = S * exp(-q * T) * N(d) - exp(-r * T) * K * N(d - v * sqrt(T));

  if (opt_type == 'C')
    return call;

  return call - S * exp(-q * T) + K * exp(-r * T);
}

void binomial_asset_pricing::CRR(double sigma, double dt, double disc) {
  // Cox, Ross and Rubinstein method
  m_up_factor = exp(sigma * sqrt(dt));
  m_down_factor = 1 / m_up_factor;
  m_p_risk_neutral = (disc - m_down_factor) / (m_up_factor - m_down_factor);
  m_q_risk_neutral = 1 - m_p_risk_neutral;
}

void binomial_asset_pricing::JR(double sigma, double dt, double interest) {
  // Jarrow and Rudd method
  m_q_risk_neutral = 0.5;
  m_p_risk_neutral = 0.5;

  double nu = interest - sigma * sigma / 2;
  m_up_factor = exp(nu * dt + sigma * sqrt(dt));
  m_down_factor = exp(nu * dt - sigma * sqrt(dt));
}

vector<double> binomial_asset_pricing::european_option_binomial(
    double initial_price, double strike, double interest_rate,
    double volatility, char opt_type, string method, int time, int time_steps) {

  double dt = double(time) / time_steps;
  double disc = exp(dt * interest_rate);
  // double disc = pow((1 + interest_rate), 1.0 / time_steps);

  // double u; // up factor
  // double d; // down factor
  if (method == "CRR") {
    CRR(volatility, dt, disc);
  } else if (method == "JR") {
    JR(volatility, dt, interest_rate);
  } else {
    throw invalid_argument("Invalid method");
  }

  // cout << "up factor " << m_up_factor << " d " << m_down_factor << endl;
  vector<double> price(time_steps + 1,
                       initial_price * pow(m_up_factor, time_steps));

  for (int i = 1; i <= time_steps; i++) {
    price[i] = price[i - 1] * m_down_factor / m_up_factor;
  }

  vector<double> value(time_steps + 1);

  for (int i = 0; i <= time_steps; i++) {
    if (opt_type == 'C')
      value[i] = price[i] - strike > 0 ? price[i] - strike : 0; // call option
    else if (opt_type == 'P')
      value[i] = strike - price[i] > 0 ? strike - price[i] : 0; // put option
    else
      throw invalid_argument("Invalid option type");
  }

  for (int i = time_steps; i > 0; i--) {
    for (int j = 0; j < i; j++) {
      value[j] =
          (m_p_risk_neutral * value[j] + m_q_risk_neutral * value[j + 1]) /
          disc;
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
