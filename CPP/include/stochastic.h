#ifndef STOCHASTIC_HEADER_H
#define STOCHASTIC_HEADER_H

#include "matplotlibcpp.h"
#include <bits/stdc++.h>

using namespace std;
namespace plt = matplotlibcpp;

class stochastic {
public:
  vector<vector<int>> random_walk_symmetric(int num_simulations, int time);
  void make_plot(vector<vector<int>> &results);
};

#endif // STOCHASTIC_HEADER_H
