#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

#include "matplotlibcpp.h"
#include <bits/stdc++.h>

using namespace std;

namespace plt = matplotlibcpp;

class tools {
public:
  double mean(vector<double> &values);
  double variance(vector<double> &values);

  void make_plot(vector<vector<double>> &results, string file_name);
  void make_plot(vector<double> &x, vector<vector<double>> &results,
                 vector<string> &labels, string title, string file_name);
  void make_plot(vector<double> &results, string file_name);
  void plot_ma(vector<double> &values, int period, string file_name);
  void plot_mean_var(vector<double> &values);
  vector<double> expected_value(vector<vector<double>> &simulations);
};

#endif // TOOLS_HEADER_H
