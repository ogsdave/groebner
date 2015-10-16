#include <iostream>

#include "moGVW.h"

using namespace std;

int main(int argc, char* argv[]) {
  unordered_set<Polynomial<> > input;
  cout << "Input:" << endl;
  for (auto it = input.begin(); it != input.end(); ++it) {
    cout << *it << endl;
  }
  moGVWRunner<> runner;
  auto output = runner.moGVW(input);
  cout << "Output:" << endl;
  for (auto it = output.begin(); it != output.end(); ++it) {
    cout << *it << endl;
  }
  return 0;
}
