#ifndef SIMULATIONS_HEADER_H
#define SIMULATIONS_HEADER_H

#include "binomial_model.h"
#include "stochastic.h"
#include "tools.h"
#include <bits/stdc++.h>

class simulation {
public:
  simulation();
  void gbm_analysis(int num_simulations, int initial_val, double drift,
                    double vol, int n_steps, int time);

  vector<vector<double>> binomial_model_sim(int num_simulations,
                                            double initial_price,
                                            double interest_rate, double sigma,
                                            int time, int time_steps);

  void gbm_vs_binomial_sim(int num_simulations, double initial_price,
                           double drift, double volatility, int time,
                           int time_steps);

  void option_expected_value(int num_simulation, double initial_price,
                             double strike_price, double interest_rate,
                             double volatility, char opt_type, int time,
                             int time_steps, string data_method);

  void compare_methods(double initial_price, double strike, double interest,
                       double vol, double opt_type, int time, int max_steps);

  void martingale(int num_simulations, double initial_price,
                  double interest_rate, double sigma, int time, int time_steps);

private:
  stochastic m_stochastic_gen;
  binomial_asset_pricing m_bin_model;
  tools m_visualizer;
};

#endif // SIMULATIONS_HEADER_H
