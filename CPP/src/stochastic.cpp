#include "../include/stochastic.h"
#include <random>

vector<vector<double>> stochastic::random_walk_symmetric(int num_simulations,
                                                         int time) {

  vector<vector<double>> res(num_simulations, vector<double>(time));

  for (int i = 0; i < num_simulations; i++) {
    res[i][0] = 0;
    for (int j = 1; j < time; j++) {
      int r = rand();
      if (r % 2 == 0)
        res[i][j] = res[i][j - 1] + 1;
      else
        res[i][j] = res[i][j - 1] - 1;
    }
  }

  return res;
}

vector<vector<double>> stochastic::GBM(int num_simulations, double initial_val,
                                       double drift, double var, int n_steps,
                                       int time) {
  double dt = double(time) / n_steps;

  random_device rd{};
  mt19937 gen{rd()};

  normal_distribution<double> d(0, sqrt(dt));
  auto random_val = [&d, &gen] { return d(gen); };

  vector<vector<double>> res(num_simulations,
                             vector<double>(n_steps, initial_val));

  for (int i = 0; i < num_simulations; i++) {
    for (int j = 1; j < n_steps; j++) {
      res[i][j] = res[i][j - 1] *
                  exp((drift - pow(var, 2) / 2) * dt + var * random_val());
    }
  }

  return res;
}

void stochastic::make_plot(vector<vector<double>> &results, string &file_name) {
  /*
  vector<double> x(100);
  for (int i = 0; i < 100; i++)
    x[i] = 0.01 * (i + 1);
  */
  // plt::plot(x, results);

  for (auto rw : results)
    plt::plot(rw);

  plt::save(file_name);
}

void stochastic::make_plot(vector<double> &results, string &file_name) {

  plt::plot(results);

  plt::save(file_name);
}

void stochastic::plot_ma(vector<double> &values, int period,
                         string &file_name) {
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

double stochastic::get_var(vector<double> &values, int idx, double mean) {
  double total = 0;
  for (int i = 0; i <= idx; i++)
    total += pow(values[i] - mean, 2);
  return total / (idx + 1);
}

double stochastic::get_vol(vector<double> &values, int idx, double mean) {
  double var = get_var(values, idx, mean);
  double std = sqrt(var);
  return std;
}

void stochastic::plot_mean_var(vector<double> &values) {
  vector<double> mean;
  vector<double> var;

  double total = 0;
  for (int i = 0; i < values.size(); i++) {
    total += values[i];
    mean.push_back(total / (i + 1));
    var.push_back(get_var(values, i, total / (i + 1)));
  }

  plt::plot(mean);
  plt::plot(values);
  plt::save("mean.png");

  plt::plot(var);
  plt::save("vol.png");

  cout << "mean: " << mean[mean.size() - 1]
       << " variance: " << var[var.size() - 1] << endl;
}
