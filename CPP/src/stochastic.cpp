#include "../include/stochastic.h"
#include <random>

vector<vector<int>> stochastic::random_walk_symmetric(int num_simulations,
                                                      int time) {

  vector<vector<int>> res(num_simulations, vector<int>(time));

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

void stochastic::make_plot(vector<vector<double>> &results) {
  vector<double> x(100);
  for (int i = 0; i < 100; i++)
    x[i] = 0.01 * (i + 1);
  // plt::plot(x, results);

  for (auto rw : results)
    plt::plot(x, rw);

  plt::save("test_gbm.png");
}
