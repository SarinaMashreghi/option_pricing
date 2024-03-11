#include "../include/tools.h"
#include <numeric>
#include <stdexcept>

// statistical analysis tools
double tools::mean(vector<double> &values) {
  return accumulate(values.begin(), values.end(), 0) / values.size();
}

double tools::variance(vector<double> &values) {
  double m = mean(values);
  double var = 0;

  for (auto val : values)
    var += pow(val - m, 2);
  return var / (size(values) - 1);
}

void tools::make_plot(vector<double> &x, vector<vector<double>> &results,
                      vector<string> &labels, string title, string file_name) {

  if (labels.size() != results.size())
    throw invalid_argument("Labels must match the size of results");

  for (int i = 0; i < results.size(); i++)
    plt::plot(x, results[i], {{"label", labels[i]}});

  plt::legend();
  plt::title(title);
  plt::save(file_name);
}

void tools::make_plot(vector<vector<double>> &results, string file_name) {

  for (auto rw : results)
    plt::plot(rw);

  plt::save(file_name);
}

void tools::plot_ma(vector<double> &values, int period, string file_name) {
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

void tools::make_plot(vector<double> &results, string file_name) {

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

vector<double> tools::expected_value(vector<vector<double>> &simulations) {
  int n_sims = simulations.size();
  int steps = simulations[0].size();

  vector<double> expected_vals(steps, 0);

  for (int i = 0; i < steps; i++) {
    for (int j = 0; j < n_sims; j++) {
      expected_vals[i] += simulations[j][i];
    }
    expected_vals[i] /= n_sims;
  }

  return expected_vals;
}
