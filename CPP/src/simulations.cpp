#include "../include/simulations.h"
#include <numeric>
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

  vector<double> ev = m_visualizer.expected_value(gbm);
  double sample_mean = ev[ev.size() - 1];
  double theory_mean = initial_val * exp(drift * time);
  double theory_var = initial_val * initial_val *
                      (exp(2 * drift * time + vol * vol * time)) *
                      (exp(vol * vol * time) - 1);
  vector<double> ev_val = m_visualizer.var_expected_val(gbm, ev);
  double estimate_var = ev_val[ev_val.size() - 1];

  double sigma = sqrt(log(estimate_var / (sample_mean * sample_mean) + 1));
  double estimated_vol =
      m_visualizer.estimated_volatility(gbm[0], double(time) / n_steps);

  cout << "actual vol: " << vol << endl
       << "Expected val: " << ev[ev.size() - 1] << endl
       << "simulated vol: " << sigma << endl
       << "Mean formula: " << theory_mean << endl
       << "Variance formula: " << theory_var << endl
       << "expected var: " << ev_val[ev.size() - 1] << endl
       << "Estimated vol: " << estimated_vol << endl;
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
                                       char opt_type, int time, int time_steps,
                                       string data_option) {

  vector<vector<double>> sim;
  if (data_option == "BIN") {
    sim = binomial_model_sim(num_simulations, initial_price, interest_rate,
                             volatility, time, time_steps);
  } else if (data_option == "GBM") {
    // 0 drift
    sim = m_stochastic_gen.GBM(num_simulations, initial_price, interest_rate,
                               volatility, time_steps, time);
  }

  double disc = exp(double(time) / time_steps * interest_rate);
  double total = 0;
  int interval_size = 5;
  int partition = num_simulations / interval_size;
  vector<double> option_expected(partition);
  vector<double> x(partition);
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

    if ((i + 1) % interval_size == 0) {
      int index = (i + 1) / interval_size - 1;
      x[index] = i + 1;
      option_expected[index] = total / i;
      option_expected[index] /= pow(disc, time_steps);
    }
  }

  unordered_map<string, vector<double>> final;

  cout << "simulated value: " << option_expected[partition - 1] << endl;
  vector<double> jr = m_bin_model.european_option_binomial(
      initial_price, strike_price, interest_rate, volatility, opt_type, "JR",
      time, time_steps);
  vector<double> crr_final = m_bin_model.european_option_binomial(
      initial_price, strike_price, interest_rate, volatility, opt_type, "CRR",
      time, time_steps);

  cout << "Jarrow and Rudd: " << jr[0] << endl;
  cout << "CRR " << crr_final[0] << endl;

  vector<double> crr(partition, crr_final[0]);

  double bsm = m_bin_model.black_scholes_merton(
      initial_price, strike_price, time, interest_rate, 0, volatility, 'C');

  cout << "black scholes " << bsm << endl;
  final["Expected Value"] = option_expected;
  final["CRR"] = crr;
  vector<string> l = {"Expected Value", "CRR"};
  m_visualizer.make_plot(x, final, "Expected Value vs CRR", "expected_val.png");
}

void simulation::compare_methods(double initial_price, double strike,
                                 double interest, double vol, double opt_type,
                                 int time, int max_steps) {

  int n = max_steps / 100;
  vector<double> x(n);

  for (int i = 0; i < n; i++)
    x[i] = (i + 1) * 100;

  unordered_map<string, vector<double>> total;
  vector<double> jr(n);
  vector<double> crr(n);
  vector<double> bsm(n, m_bin_model.black_scholes_merton(initial_price, strike,
                                                         time, interest, 0, vol,
                                                         opt_type));

  for (int i = 0; i < n; i++) {
    vector<double> jr_tmp = m_bin_model.european_option_binomial(
        initial_price, strike, interest, vol, opt_type, "JR", time, x[i]);
    vector<double> crr_tmp = m_bin_model.european_option_binomial(
        initial_price, strike, interest, vol, opt_type, "CRR", time, x[i]);
    jr[i] = jr_tmp[0];
    crr[i] = crr_tmp[0];
  }

  total["JR"] = jr;
  total["CRR"] = crr;
  total["BSM"] = bsm;

  m_visualizer.make_plot(x, total, "Methods Comparison", "compare_methods.png");
}

void discount(vector<vector<double>> &data, double dt, double interest) {
  double disc = exp(dt * interest);
  for (int i = 0; i < data[0].size(); i++) {
    for (int j = 0; j < data.size(); j++) {
      data[j][i] /= pow(disc, i + 1);
    }
  }
}

void simulation::martingale(int num_simulations, double initial_price,
                            double interest_rate, double sigma, int time,
                            int time_steps) {

  vector<vector<double>> bin_sim =
      binomial_model_sim(num_simulations, initial_price, interest_rate, sigma,
                         time, time_steps); // not discounted
  discount(bin_sim, double(time) / time_steps, interest_rate);

  vector<double> expected = m_visualizer.expected_value(bin_sim);
  m_visualizer.make_plot(expected, "martingale.png");
}

void simulation::random_walk_sim(int n_simulations) {
  vector<vector<double>> rw =
      m_stochastic_gen.random_walk_symmetric(n_simulations, 100);
  m_visualizer.make_plot_steps(rw);
}
