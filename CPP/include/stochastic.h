#ifndef STOCHASTIC_HEADER_H
#define STOCHASTIC_HEADER_H

#include "matplotlibcpp.h"
#include <bits/stdc++.h>
#include <cmath>
#include <random>

using namespace std;
namespace plt = matplotlibcpp;

class stochastic {
public:
  vector<vector<double>> random_walk_symmetric(int num_simulations, int time);
  vector<vector<double>> GBM(int num_simulations, double initial_val,
                             double drift, double var, int n_steps, int time);
  void make_plot(vector<vector<double>> &results, string &file_name);
  void make_plot(vector<double> &results, string &file_name);
  void plot_ma(vector<double> &values, int period, string &file_name);
  void plot_mean_var(vector<double> &values);

  double get_var(vector<double> &values, int idx, double mean);
  double get_vol(vector<double> &values, int idx, double mean);
};

#endif // STOCHASTIC_HEADER_H
