#include "../include/tools.h"

void tools::make_plot(vector<vector<double>> &results, string &file_name) {
  /*
  vector<double> x(100);
  for (int i = 0; i < 100; i++)
    x[i] = 0.01 * (i + 1);
  */
  // plt::plot(x, results);

  for (auto rw : results)
    plt::plot(rw);

  double s = 120.0;
  char t = 'C';
  // double opt_expected = european_option_expected(results, s, t);
  // cout << "expected: " << opt_expected << endl;
  plt::save(file_name);
}

void tools::plot_ma(vector<double> &values, int period, string &file_name) {
  double total = 0;
  vector<double> ma;
  vector<double> x;

  for (int i = 0; i < values.size(); i++) {
    if (i < period) {
      total += values[i];
    } else {
      ma.push_back(total / period);
      x.push_back(i);
      total += values[i] - values[i - period];
    }
  }

  plt::plot(values);
  plt::plot(x, ma);
  plt::save(file_name);
}

void tools::make_plot(vector<double> &results, string &file_name) {

  plt::plot(results);

  plt::save(file_name);
}

void tools::plot_mean_var(vector<double> &values) {
  vector<double> mean;
  vector<double> var;

  double total = 0;
  for (int i = 0; i < values.size(); i++) {
    total += values[i];
    mean.push_back(total / (i + 1));
    // var.push_back(get_var(values, i, total / (i + 1)));
  }

  plt::plot(mean);
  plt::plot(values);
  plt::save("mean.png");

  plt::plot(var);
  plt::save("vol.png");

  cout << "mean: " << mean[mean.size() - 1]
       << " variance: " << var[var.size() - 1] << endl;
}
