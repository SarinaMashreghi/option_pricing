#include "../include/tools.h"
#include <fstream>
#include <numeric>
#include <stdexcept>

// statistical analysis tools
double tools::mean(vector<double> &values) {
  return double(accumulate(values.begin(), values.end(), 0.0)) /
         (values.size() - 1);
}

double tools::variance(vector<double> &values) {
  double m = mean(values);
  double var = 0;

  for (auto val : values)
    var += pow(val - m, 2);
  return var / (values.size() - 1);
}

double tools::estimated_volatility(vector<double> &values, double dt) {
  vector<double> log_return(values.size() - 1);
  for (int i = 0; i < log_return.size(); i++)
    log_return[i] = log(values[i + 1] / values[i]);
  double mean_return = mean(log_return);
  double sample_var = variance(log_return);
  double estimated_vol = sqrt(sample_var / dt);
  return estimated_vol;
}

void tools::make_plot(vector<double> &x,
                      unordered_map<string, vector<double>> &results,
                      string title, string file_name) {

  plt::xlim(0, 100);
  for (auto item : results)
    plt::plot(x, item.second, {{"label", item.first}});

  plt::legend();
  plt::title(title);
  // plt::save(file_name);
}

void tools::make_plot(vector<vector<double>> &results, string file_name) {

  plt::xlim(0, 100);
  plt::ylim(-35, 35);
  for (int i = 0; i < results.size(); i++) {
    plt::plot(results[i]);
    plt::save("test/" + to_string(i) + ".png");
    cout << i << endl;
  }
  cout << "here" << endl;

  // plt::save(file_name);
}

string random_hex_color() {
  std::string color = "#";
  char hex_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                      '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

  for (int i = 0; i < 6; ++i) {
    color += hex_chars[rand() % 16];
  }

  return color;
}

void tools::make_plot_steps(vector<vector<double>> &results) {
  plt::xlim(0, 100);
  plt::ylim(-35, 35);
  int counter = 0;
  for (int i = 0; i < results.size(); i++) {
    string color = random_hex_color();
    for (int j = 1; j < results[i].size(); j++) {
      plt::plot(vector<double>(results[i].begin(), results[i].begin() + j),
                {{"color", color}});
      plt::save("test/" + to_string(counter) + ".png");
      counter++;
    }
  }
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
  /* expected value at each step of the simulation */
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

vector<double> expected_value_2(vector<vector<double>> &simulations) {
  /* E[X^2] */
  int n_sims = simulations.size();
  int steps = simulations[0].size();

  vector<double> expected_vals(steps, 0);

  for (int i = 0; i < steps; i++) {
    for (int j = 0; j < n_sims; j++) {
      expected_vals[i] += simulations[j][i] * simulations[j][i];
    }
    expected_vals[i] /= n_sims - 1;
  }

  return expected_vals;
}

vector<double> tools::var_expected_val(vector<vector<double>> &sim,
                                       vector<double> &expected_val) {
  vector<double> ex2 = expected_value_2(sim);
  vector<double> var(expected_val.size());
  for (int i = 0; i < expected_val.size(); i++) {
    var[i] = ex2[i] - expected_val[i] * expected_val[i];
  }
  /*
  vector<double> var(expected_val.size(), 0);

  for (int i = 0; i < expected_val.size(); i++) {
    for (int j = 0; j < sim.size(); j++) {
      var[i] += pow(sim[j][i] - expected_val[i], 2);
    }
    var[i] /= sim.size() - 1;
  }
  */
  return var;
}
/*
void tools::export_csv(unordered_map<string, vector<double>> &values,
                       string file_name) {
  ofstream file;
  file.open(file_name);
}
*/
