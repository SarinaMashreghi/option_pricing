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

double stochastic::european_option_expected(vector<vector<double>> &gbm,
                                            double strike, char opt_type) {
  double total = 0;

  for (auto sim : gbm) {
    double final_price = sim[sim.size() - 1];
    if (opt_type == 'C')
      total += final_price > strike ? final_price - strike : 0;
    else if (opt_type == 'P')
      total += final_price < strike ? strike - final_price : 0;
  }

  return total / gbm.size();
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
