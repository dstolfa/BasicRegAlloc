#include "lib/BasicRegAlloc.h"
#include <iostream>
#include <vector>

static void logOverlap(int v1, int v2, std::pair<int, int> lv1,
                       std::pair<int, int> lv2, int physreg) {
  std::cerr << "Virtual registers <" << v1 << ", " << v2
            << "> with live ranges <(" << lv1.first << ", " << lv1.second
            << "), (" << lv2.first << ", " << lv2.second
            << ")> are allocated to the same physical register: <" << physreg
            << ">" << std::endl;
}

/// This test is designed to check the case where we have more virtual registers
/// than physical registers and where all of the live intervals of the virtual
/// registers overlap. We expect the registers to be greedily allocated
/// depending on the weight, as well as a spill of one register (the one with
/// the lowest weight), however we do not test for that particularl property in
/// this test.
int main(void) {
  // All virtual registers (1, 2, 3) have live ranges (2, 5). The weights
  // of registers 1, 2 and 3 are 3, 4 and 6 respectively.
  std::vector<int> indices = {{1, 2, 3}};
  std::vector<std::pair<int, int>> liveIntervals = {
      {std::make_pair(2, 5), std::make_pair(2, 5), std::make_pair(2, 5)}};
  std::vector<int> weight = {{3, 4, 6}};
  const std::size_t numberOfRegisters = 2;
  const std::size_t numberOfBuckets = 3;

  // Construct the clash graph.
  ClashGraph g(3, weight);
  for (std::size_t i = 0; i < numberOfBuckets; i++) {
    for (std::size_t j = i + 1; j < numberOfBuckets; j++) {
      if (clashes(liveIntervals[i], liveIntervals[j]))
        g.addEdge(indices[i], indices[j]);
    }
  }

  auto res = g.colourGraph(numberOfRegisters);
  for (std::size_t i = 1; i < res.size(); i++)
    for (std::size_t j = 0; j < i; j++)
      // Check if we have a clash *and* the same register allocated. If that is
      // the case, we have violated the property we are testing for.
      if (clashes(liveIntervals[res[i].first], liveIntervals[res[j].first]) &&
          res[i].second == res[j].second) {
        logOverlap(res[i].first, res[j].first, liveIntervals[res[i].first],
                   liveIntervals[res[j].first], res[i].second);
        return 1;
      }
  return 0;
}
