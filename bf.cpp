#include <iostream>
#include <iomanip>
#include <cassert>
#include <primesieve.hpp>
#include <algorithm>

using namespace std;

void printStep(const char* op) {
  cout << "\033[0;94m" << op << "\033[0m" << endl;
}

double getWallTime()
{
  return static_cast<double>(std::clock()) / CLOCKS_PER_SEC;
}

void claimNumber(int x, vector<bool> &v, long long &playerScore) {
  assert((unsigned int)x < v.size());
  v[x] = true;
  playerScore += x;
  for (int i = 1; i <= x/2; ++i) {
    if (x%i == 0 && !v[i]) {
      v[i] = true;
    }
  }
}

bool hasDivisor(int x, vector<bool> &v, int lb) {
  for (int i = max(lb, 1); i <= x/2; ++i)
    if (x%i == 0 && !v[i])
      return true;
  return false;
}

void simGame(int x, vector<int> &moves) {
  // Game starts at index 1
  vector<int> primes;
  primesieve::generate_primes(7, &primes);
  vector<bool> taken(x+1);
  long long playerScore = 0, computerScore = 0;
  for (int j = primes.size()-1; j >= 0; --j) {
    size_t i = x/2 + 1;
    if (i%primes[j] != 0){
      i += primes[j] - (i%primes[j]);
      if (i >= taken.size())
        continue;
    }
    for (; i < taken.size(); i += primes[j]) {
      assert(i % primes[j] == 0);
      bool newNum = false;
      for (int k = 0; k < j; ++k) {
        if (i % primes[k] == 0) {
          newNum = true;
          break;
        }
      }
      if (!newNum && hasDivisor(i, taken, taken.size()/(2*primes[j]))) {
        claimNumber(i, taken, playerScore);
        moves.push_back(i);
      }
    }
//    int numThings = 0;
//    auto begin = taken.begin();
//    begin += taken.size();
//    auto end = taken.begin();
//    end += taken.size() / 3;
//    for_each(begin, end, [&](bool lel) { if (lel) ++numThings;});
//    cout << numThings << endl;
  }
  auto totScore = (x*(x+1))/2;
  computerScore = totScore - playerScore;
  cout << "Sz: " << x << "\t Ratio: " << (float)playerScore/totScore << "\t PlayerScore: " << playerScore << "\t ComputerScore: " << computerScore << endl;
//  cout << x << ", " << playerScore << ", " << computerScore << ", " << totScore << endl;
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
    vector<int> moves;
    simGame(i, moves);
//    cout << "MOVES:\n";
//    for (auto &m : moves)
//      cout << m << " ";
//    cout << endl;
  }

  // Print the time elapsed
  double totalTime = getWallTime() - startTime;
  cout << "Total time elapsed" << " : " << totalTime << " sec" << endl;
  return 0;
}
