#include "../include/simulations.h"

void simulation::european_option_expected() {}

void simulation::gbm_vs_binomial(int num_simulations, double initial_price,
                                 double drift, double volatility, int time,
                                 int time_steps) {

  vector<vector<double>> gbm = m_stochastic_gen.GBM(
      num_simulations, initial_price, drift, volatility, time_steps, time);
  // up factor
  double dt = double(time) / time_steps;
  double u = exp(volatility * sqrt(dt));

  double r = 0; // interest rate??
  double disc = exp(dt * r);
  double d = 1 / u;
  cout << "up " << u << " down " << d << endl;

  // risk-neutral probabilities
  double p = (disc - d) / (u - d);
  double q = 1 - p;

  cout << "risk neutral probabilities: " << p << " " << q << endl;

  vector<vector<double>> bin_result(num_simulations,
                                    vector<double>(time_steps, initial_price));

  random_device rd{};
  mt19937 gen{rd()};
  bernoulli_distribution b_dist(p);

  for (int i = 0; i < num_simulations; i++) {
    for (int j = 1; j < time_steps; j++) {
      bin_result[i][j] = b_dist(gen) == 1 ? bin_result[i][j - 1] * u
                                          : bin_result[i][j - 1] * d;
    }
  }

  string file_name = "bin_result.png";
  m_visualizer.make_plot(bin_result, file_name);
}
