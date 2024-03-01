#ifndef SIMULATIONS_HEADER_H
#define SIMULATIONS_HEADER_H

#include "binomial_model.h"
#include "stochastic.h"
#include "tools.h"
#include <bits/stdc++.h>

class simulation {
public:
  // simulation();
  void european_option_expected();

  void gbm_vs_binomial(int num_simulations, double initial_price, double drift,
                       double volatility, int time, int time_steps);

private:
  stochastic m_stochastic_gen;
  binomial_asset_pricing *m_bin_model;
  tools m_visualizer;
};

#endif // SIMULATIONS_HEADER_H
