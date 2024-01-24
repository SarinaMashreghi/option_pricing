#include "include/binomial_model.h"
#include "include/stochastic.h"
#include <fstream>
#include <iostream>
#include <sstream>

int main() {

  stochastic s = stochastic();
  // vector<vector<int>> rw = s.random_walk_symmetric(100, 100);
  vector<vector<double>> gbm = s.GBM(10, 100, 0.1, 0.3, 100, 1);
  s.make_plot(gbm);

  double initial_price = 100;
  double strike = 100;
  double time_to_maturity = 1;
  double interest_rate = 0.06;
  int time_steps = 3;
  double up_factor = 1.1;
  char option_type = 'C';
  double barrier = 125;

  binomial_asset_pricing pricing_model = binomial_asset_pricing();

  double european_opt = pricing_model.european_option_binomial(
      initial_price, strike, time_to_maturity, time_steps, interest_rate,
      up_factor, option_type);

  double barrier_opt = pricing_model.barrier_option_binomial(
      initial_price, strike, barrier, time_to_maturity, time_steps,
      interest_rate, up_factor, option_type);
  cout << barrier_opt << endl;

  return 0;
}
