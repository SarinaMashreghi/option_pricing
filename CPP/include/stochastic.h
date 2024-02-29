#ifndef STOCHASTIC_HEADER_H
#define STOCHASTIC_HEADER_H

#include <bits/stdc++.h>
#include <cmath>
#include <random>

using namespace std;

class stochastic {
public:
  vector<vector<double>> random_walk_symmetric(int num_simulations, int time);
  vector<vector<double>> GBM(int num_simulations, double initial_val,
                             double drift, double var, int n_steps, int time);

  double get_var(vector<double> &values, int idx, double mean);
  double get_vol(vector<double> &values, int idx, double mean);

  double european_option_expected(vector<vector<double>> &gbm, double strike,
                                  char opt_type);
};

#endif // STOCHASTIC_HEADER_H
