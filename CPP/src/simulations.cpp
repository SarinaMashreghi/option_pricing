#include "../include/simulations.h"
#include <stdexcept>

vector<vector<double>> simulation::binomial_model_sim(int num_simulations,
                                                      double initial_price,
                                                      double interest_rate,
                                                      double sigma, int time,
                                                      int time_steps) {

  vector<vector<double>> bin_model(num_simulations,
                                   vector<double>(time_steps, initial_price));
  // up factor
  double dt = double(time) / time_steps;
  double u = exp(sigma * dt);

  double disc = exp(dt * interest_rate);
  // double disc = pow((1 + interest_rate), 1.0 / time_steps);
  double d = 1 / u;
  // cout << "up " << u << " down " << d << endl;

  // risk-neutral probabilities
  double p = (disc - d) / (u - d);
  double q = 1 - p;

  // cout << "risk neutral probabilities: " << p << " " << q << endl;
  random_device rd{};
  mt19937 gen{rd()};
  bernoulli_distribution b_dist(p);

  for (int i = 0; i < num_simulations; i++) {
    for (int j = 1; j < time_steps; j++) {
      bin_model[i][j] =
          b_dist(gen) == 1 ? bin_model[i][j - 1] * u : bin_model[i][j - 1] * d;
      // bin_model[i][j] /= disc;
    }
  }

  return bin_model;
}

void simulation::gbm_vs_binomial_sim(int num_simulations, double initial_price,
                                     double drift, double volatility, int time,
                                     int time_steps) {

  vector<vector<double>> gbm = m_stochastic_gen.GBM(
      num_simulations, initial_price, drift, volatility, time_steps, time);

  double interest = 0.1; // necessary?
  vector<vector<double>> bin_result = binomial_model_sim(
      num_simulations, initial_price, interest, volatility, time, time_steps);

  vector<double> expected_bin = m_visualizer.expected_value(bin_result);
  vector<double> expected_gbm = m_visualizer.expected_value(gbm);

  cout << "Expected value binomial: " << expected_bin[time_steps - 1] << endl;
  cout << "Expected value gbm: " << expected_gbm[time_steps - 1] << endl;
  cout << "Theoretical gbm: " << initial_price * exp(drift * time) << endl;
  cout << "Theoretical gbm2: "
       << initial_price * exp(drift + volatility * volatility / 2) << endl;

  string file_name = "bin_result.png";
  // m_visualizer.make_plot(bin_result, file_name);
}

void simulation::option_expected_value(int num_simulations,
                                       double initial_price,
                                       double strike_price,
                                       double interest_rate, double volatility,
                                       char opt_type, int time,
                                       int time_steps) {

  vector<vector<double>> sim =
      binomial_model_sim(num_simulations, initial_price, interest_rate,
                         volatility, time, time_steps);

  double total = 0;
  double val;
  for (int i = 0; i < num_simulations; i++) {
    if (opt_type == 'C') // call option
      total += sim[i][time_steps - 1] > strike_price
                   ? sim[i][time_steps - 1] - strike_price
                   : 0;
    else if (opt_type == 'P') // put option
      total += sim[i][time_steps - 1] < strike_price
                   ? strike_price - sim[i][time_steps - 1]
                   : 0;
    else
      throw invalid_argument("Invalid option type");
    // total += val;
  }

  double disc = exp(double(time) / time_steps * interest_rate);
  double option_expected = total / double(num_simulations);
  option_expected /= pow(disc, time_steps);
  cout << "simulated value: " << option_expected << endl;
  vector<double> model_vals = m_bin_model->european_option_binomial(
      initial_price, strike_price, interest_rate, volatility, opt_type, "CRR",
      time, time_steps);
  cout << "formula: " << model_vals[0] << endl;
}
