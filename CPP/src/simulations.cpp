#include "../include/simulations.h"
#include <stdexcept>

simulation::simulation() { m_bin_model = binomial_asset_pricing(); }

vector<vector<double>> simulation::binomial_model_sim(int num_simulations,
                                                      double initial_price,
                                                      double interest_rate,
                                                      double sigma, int time,
                                                      int time_steps) {

  /* returns non discounted price simulated by the binomial model */
  double dt = double(time) / time_steps;
  double u = exp(sigma * sqrt(dt));

  double disc = exp(dt * interest_rate);
  double d = 1 / u;
  // cout << "up " << u << " down " << d << endl;

  // risk-neutral probabilities
  double p = (disc - d) / (u - d);
  double q = 1 - p;

  vector<vector<double>> bin_model(
      num_simulations, vector<double>(time_steps, initial_price / disc));

  // cout << "risk neutral probabilities: " << p << " " << q << endl;
  random_device rd{};
  mt19937 gen{rd()};
  bernoulli_distribution b_dist(p);

  for (int i = 0; i < num_simulations; i++) {
    for (int j = 1; j < time_steps; j++) {
      bin_model[i][j] =
          b_dist(gen) == 1 ? bin_model[i][j - 1] * u : bin_model[i][j - 1] * d;
    }
  }

  return bin_model;
}

void simulation::gbm_analysis(int num_simulations, int initial_val,
                              double drift, double vol, int n_steps, int time) {

  vector<vector<double>> gbm = m_stochastic_gen.GBM(
      num_simulations, initial_val, drift, vol, n_steps, time);

  vector<double> log_diff(n_steps - 1);
  for (int i = 0; i < n_steps - 1; i++) {
    log_diff[i] = log(gbm[0][i + 1] / gbm[0][i]);
    // cout << log_diff[i] << endl;
  }
  cout << "log diff " << sqrt(m_visualizer.variance(log_diff)) << endl;

  double M = m_visualizer.mean(gbm[0]);
  double V = m_visualizer.variance(gbm[0]);
  double theory_mean =
      initial_val * exp(drift * time); // multiplication by initial value???
  double theory_var =
      initial_val * initial_val * (exp(2 * drift * time + vol * vol * time));

  double sigma = sqrt(log(theory_var / (theory_mean * theory_mean)) + 1);

  cout << "actual vol: " << vol << endl
       << "simulated vol: " << sigma << endl
       << "Mean formula: " << theory_mean << endl
       << "simulated mean: " << M << endl
       << "Variance formula: " << theory_var << endl
       << "simulated var: " << V << endl;
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

  double disc = exp(double(time) / time_steps * interest_rate);
  /* add this to another function
vector<double> expected_price = m_visualizer.expected_value(sim);
cout << "expected price: " << expected_price[time_steps - 1] << endl;
cout << "expected price discounted: "
     << expected_price[time_steps - 1] / pow(disc, time_steps) << endl;
  */
  double total = 0;
  vector<double> option_expected(num_simulations);
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

    option_expected[i] = total / (i + 1);
    option_expected[i] /= pow(disc, time_steps);
  }

  // double option_expected = total / double(num_simulations);
  // option_expected /= pow(disc, time_steps);

  cout << "simulated value: " << option_expected[num_simulations - 1] << endl;
  vector<double> jr = m_bin_model.european_option_binomial(
      initial_price, strike_price, interest_rate, volatility, opt_type, "JR",
      time, time_steps);
  vector<double> crr = m_bin_model.european_option_binomial(
      initial_price, strike_price, interest_rate, volatility, opt_type, "CRR",
      time, time_steps);

  cout << "Jarrow and Rudd: " << jr[0] << endl;
  cout << "CRR " << crr[0] << endl;

  double bsm = m_bin_model.black_scholes_merton(
      initial_price, strike_price, time, interest_rate, 0, volatility, 'C');

  cout << "black scholes " << bsm << endl;
  m_visualizer.make_plot(option_expected, "expected_val.png");
}
