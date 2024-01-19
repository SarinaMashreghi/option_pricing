#include "../include/stochastic.h"

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

void stochastic::make_plot(vector<vector<int>> &results) {

  for (auto rw : results)
    plt::plot(rw);
  plt::save("test_rand_walk.png");
}
