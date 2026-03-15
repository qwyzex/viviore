#include <chrono>
#include <iostream>
#include <random>

using namespace std;

#ifndef ENTROPY_H
#define ENTROPY_H

double entropy() {
  static mt19937_64 rng(
      chrono::high_resolution_clock::now().time_since_epoch().count());
  static uniform_real_distribution<double> unif(0.0, 1.0);

  return unif(rng);
}

#endif
