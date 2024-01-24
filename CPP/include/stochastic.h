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
  vector<vector<int>> random_walk_symmetric(int num_simulations, int time);
  vector<vector<double>> GBM(int num_simulations, double initial_val,
                             double drift, double var, int n_steps, int time);
  void make_plot(vector<vector<double>> &results);
};

#endif // STOCHASTIC_HEADER_H
