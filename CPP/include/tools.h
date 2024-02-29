#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

#include "matplotlibcpp.h"
#include <bits/stdc++.h>

using namespace std;

namespace plt = matplotlibcpp;

class tools {
public:
  void make_plot(vector<vector<double>> &results, string &file_name);
  void make_plot(vector<double> &results, string &file_name);
  void plot_ma(vector<double> &values, int period, string &file_name);
  void plot_mean_var(vector<double> &values);
};

#endif // TOOLS_HEADER_H
