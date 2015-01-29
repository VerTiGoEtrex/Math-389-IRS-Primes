#include <iostream>
#include <iomanip>
#include <cassert>
#include <primesieve.hpp>

using namespace std;

void printStep(const char* op) {
  cout << "\033[0;94m" << op << "\033[0m" << endl;
}

double getWallTime()
{
  return static_cast<double>(std::clock()) / CLOCKS_PER_SEC;
}

void claimNumber(int x, vector<bool> &v, long long &playerScore, long long &computerScore) {
  v[x] = true;
  playerScore += x;
  for (int i = 1; i <= x/2; ++i) {
    if (x%i == 0 && !v[i]) {
      v[i] = true;
      computerScore += i;
    }
  }
}

bool hasDivisor(int x, vector<bool> &v) {
  for (int i = 1; i <= x/2; ++i)
    if (x%i == 0 && !v[i])
      return true;
  return false;
}

void simGame(int x) {
  // Game starts at index 1
  vector<int> primes;
  primesieve::generate_primes(20, &primes);
  vector<bool> taken(x+1);
  long long playerScore = 0, computerScore = 0;
  for (int j = primes.size()-1; j >= 0; --j) {
    for (size_t i = x/primes[j]; i < taken.size(); i += primes[j]) {
      if (i%primes[j] != 0)
        i += primes[j] - (i%primes[j]);
      assert(i % primes[j] == 0);
      bool newNum = false;
      for (int k = 0; k < j; ++k) {
        if (i % primes[k] == 0) {
          newNum = true;
          break;
        }
      }
      if (!newNum && hasDivisor(i, taken))
        claimNumber(i, taken, playerScore, computerScore);
    }
  }
  cout << "Sz: " << x << "\t Ratio: " << (float)playerScore/(x*(x+1)/2) << "\t PlayerScore: " << playerScore << "\t ComputerScore: " << computerScore << endl;
}

int main(int argc, char *argv[])
{
  double startTime = getWallTime();

  int min = 1;
  int max = 1;
  // Parse cmdline
  if (argc == 2) {
    max = atoi(argv[1]);
  } else if (argc == 3) {
    min = atoi(argv[1]);
    max = atoi(argv[2]);
  } else {
    return 1;
  }
  assert(min > 0);
  assert(max >= min);

  for (int i = min; i <= max; ++i) {
    simGame(i);
  }

  // Print the time elapsed
  double totalTime = getWallTime() - startTime;
  cout << "Total time elapsed" << " : " << totalTime << " sec" << endl;
  return 0;
}
