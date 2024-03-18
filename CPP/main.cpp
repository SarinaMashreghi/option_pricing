#include "include/binomial_model.h"
#include "include/simulations.h"
#include "include/stochastic.h"
#include "include/tools.h"

#include <fstream>
#include <iostream>
#include <sstream>

int main(int argc, char **argv) { // arguments: simulation name

  stochastic s = stochastic();
  tools visualizer = tools();
  simulation sim = simulation();
  binomial_asset_pricing pricing_model = binomial_asset_pricing();

  visualizer.video();

  double initial_price = 100;
  double strike = 110;
  double time_to_maturity = 3;
  double interest_rate = 0.06;
  int time_steps = 10000;
  double up_factor = 1.03045;
  char option_type = 'C';
  double barrier = 125;
  double vol = 0.3;

  if (strcmp(argv[1], "BIN") == 0) {

    vector<double> opt_price = pricing_model.european_option_binomial(
        initial_price, strike, interest_rate, vol, option_type, "CRR",
        time_to_maturity, time_steps);

    cout << "bin model " << opt_price[0] << endl;
  } else if (strcmp(argv[1], "GBM") == 0) {
    sim.gbm_vs_binomial_sim(10000, initial_price, 0, vol, time_to_maturity,
                            time_steps);
  } else if (strcmp(argv[1], "EXPECTED") == 0) {
    sim.option_expected_value(1000, initial_price, strike, interest_rate, vol,
                              option_type, time_to_maturity, time_steps, "GBM");
  } else if (strcmp(argv[1], "ANALYSIS") == 0) {
    sim.gbm_analysis(1000, 100, 0.4, 0.3, 1000, 1);
  } else if (strcmp(argv[1], "COMPARE") == 0) {
    sim.compare_methods(initial_price, strike, interest_rate, vol, 'C', 1,
                        10000);
  } else if (strcmp(argv[1], "MARTINGALE") == 0) {
    sim.martingale(1000, initial_price, interest_rate, vol, time_to_maturity,
                   time_steps);
  }

  return 0;
}
