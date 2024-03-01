#include "include/binomial_model.h"
#include "include/simulations.h"
#include "include/stochastic.h"
#include "include/tools.h"

#include <fstream>
#include <iostream>
#include <sstream>

int main() {

  stochastic s = stochastic();
  tools visualizer = tools();
  simulation sim = simulation();
  // vector<vector<double>> rw = s.random_walk_symmetric(100, 100);
  vector<vector<double>> gbm = s.GBM(10, 100, 0.1, 0.3, 1000, 1);
  string file_name = "test.png";
  // visualizer.make_plot(gbm, file_name);
  //  s.plot_ma(gbm[0], 5, file_name);
  //  s.make_plot(gbm, file_name);

  double initial_price = 100;
  double strike = 100;
  double time_to_maturity = 1;
  double interest_rate = 0.06;
  int time_steps = 10;
  double up_factor = 1.1;
  char option_type = 'C';
  double barrier = 125;
  double vol = 0.3;

  binomial_asset_pricing pricing_model = binomial_asset_pricing(
      initial_price, strike, interest_rate, up_factor, option_type);

  vector<double> opt_price =
      pricing_model.european_option_binomial(time_to_maturity, time_steps);

  cout << "bin model " << opt_price[0] << endl;

  sim.gbm_vs_binomial(5, initial_price, 0.1, vol, time_to_maturity, 1000);

  /*


  vector<double> test;

  for (int i = 0; i < 1000; i++) {


vector<double> european_opt = pricing_model.european_option_binomial(
                initial_price, strike, time_to_maturity, i,
interest_rate, up_factor, option_type);

    vector<double> crr =
        pricing_model.CRR(initial_price, strike, interest_rate, vol,
                          time_to_maturity, i, option_type);
    cout << i << " " << crr[0] << endl;
    test.push_back(crr[0]);
  }
  string file = "european_test.png";

  double barrier_opt = pricing_model.barrier_option_binomial(
      initial_price, strike, barrier, time_to_maturity, time_steps,
      interest_rate, up_factor, option_type);
  s.make_plot(test, file);
  cout << barrier_opt << endl;

  */

  return 0;
}
