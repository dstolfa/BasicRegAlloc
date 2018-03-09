/*-
 * Copyright (c) 2018 Domagoj Stolfa
 * All rights reserved.
 *
 * This software was developed by BAE Systems, the University of Cambridge
 * Computer Laboratory, and Memorial University under DARPA/AFRL contract
 * FA8650-15-C-7558 ("CADETS"), as part of the DARPA Transparent Computing
 * (TC) research program.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

static bool debug = false;

#define CLOG(x)                                                                \
  if (debug)                                                                   \
    std::clog << x;

/// The Input struct holds an abstraction for the inputs of the allocator
struct Input {
  std::vector<int> indices;
  std::vector<std::pair<int, int>> liveIntervals;
  std::vector<int> weight;
  std::size_t numberOfBuckets;
  std::size_t numberOfPlaces;

  void allocateSpace() {
    indices.resize(numberOfBuckets);
    liveIntervals.resize(numberOfBuckets);
    weight.resize(numberOfBuckets);
  }
};

/// The ClashGraph structure holds the information necessary to do register
/// allocation. Subject to change.
struct ClashGraph {
private:
  std::vector<std::vector<int>> adjMatrix;
  std::vector<std::size_t> deg;
  std::vector<int> weight;

public:
  ClashGraph(std::size_t _matSize, std::vector<int> &w)
      : adjMatrix(_matSize, std::vector<int>(_matSize)), deg(_matSize),
        weight(w) {}

  /// Adds an edge to the clash graph and maintains degree
  void addEdge(int n1, int n2) {
    adjMatrix[n1 - 1][n2 - 1] = 1;
    adjMatrix[n2 - 1][n1 - 1] = 1;
    deg[n1 - 1]++;
    deg[n2 - 1]++;
  }

  /// Used to dump the adjacency matrix
  void logGraph() const {
    for (std::size_t i = 0; i < adjMatrix.size(); i++) {
      CLOG("Element " << i + 1 << ": ");
      for (std::size_t j = 0; j < adjMatrix[i].size(); j++) {
        if (adjMatrix[i][j] == 1)
          CLOG(j + 1 << " | ");
      }
      CLOG(std::endl);
    }
  }

  /// Returns the result in the format of { (id, register), ... }
  std::vector<std::pair<int, int>> colourGraph(std::size_t k) const {
    std::stack<std::pair<int, bool>> s; // The stack used for the heuristic
    std::vector<int> allColours(k);     // All of the possible colours
    std::vector<std::pair<int, int>> res(adjMatrix.size()); // Result
    std::vector<std::size_t> tempDeg(deg);    // A copy of degrees, will change
    std::vector<int> tempWeight(weight);      // A copy of weight, will change
    std::size_t nodesLeft = adjMatrix.size(); // Num of nodes to process

    std::generate(allColours.begin(), allColours.end(),
                  [n = 0]() mutable { return n++; });

    do {
      // Get the minimal degree
      auto ind = std::distance(
          tempDeg.begin(), std::min_element(tempDeg.begin(), tempDeg.end()));
      // If the minimal degree is greater or equal to the amount of colours
      // we're trying to colour the graph with, we need to spill. Our spilling
      // heuristic is the one with the minimum weight. Otherwise, we simply
      // place the node with the minimal degree on the stack and leave it to be
      // dealt with later on.
      if (tempDeg[ind] >= k) {
        auto minused = std::distance(
            tempWeight.begin(),
            std::min_element(tempWeight.begin(), tempWeight.end()));
        tempWeight[minused] = std::numeric_limits<int>::max();
        s.push(std::make_pair(minused, true)); // true == spilled
        CLOG("Pushing: " << minused + 1 << " true" << std::endl);
        ind = minused;
      } else {
        s.push(std::make_pair(ind, false)); // false == not spilled
        CLOG("Pushing: " << ind + 1 << " false" << std::endl);
      }

      // Update the degree of each node in the graph after we've "removed" the
      // current node.
      for (std::size_t i = 0; i < adjMatrix.size(); i++) {
        if (adjMatrix[i][ind] == 1) {
          tempDeg[i]--;
        }
      }
      // Set the degree to the maximal value as a workaround. Needs to be done
      // in a better way, but this will do initially.
      tempDeg[ind] = std::numeric_limits<std::size_t>::max();
    } while (--nodesLeft > 0);

    nodesLeft = s.size();
    std::vector<bool> poppedNodes(adjMatrix.size());
    do {
      auto e = s.top();
      CLOG("Popping: " << e.first + 1 << std::endl);
      s.pop();

      poppedNodes[e.first] = true;
      // Check if this value needs to be spilled. If so, we use -1 to denote a
      // spill. Otherwise, we look at its neighbours and decide to colour it
      // with the first colour available in the set that contains the set
      // difference of neighbouring colours and all colours, as such:
      // allColours \ cSet = colourDiff.
      if (e.second == true) {
        res[e.first] = std::make_pair(e.first, -1);
      } else {
        std::vector<int> cSet;
        for (int i = 0; i < poppedNodes.size(); i++) {
          if (e.first != i && poppedNodes[i] && adjMatrix[e.first][i] == 1) {
            cSet.push_back(res[i].second);
          }
        }
        std::vector<int> colourDiff(allColours.size());
        std::sort(cSet.begin(), cSet.end());
        std::set_difference(allColours.begin(), allColours.end(), cSet.begin(),
                            cSet.end(),
                            std::inserter(colourDiff, colourDiff.begin()));
        // We have our colour now, update our resulting vector.
        res[e.first] = std::make_pair(e.first, colourDiff[0]);
      }
    } while (--nodesLeft > 0);

    return res;
  }
};

bool overlaps(std::pair<int, int> &l1, std::pair<int, int> &l2) {
  return (l1.first < l2.second) && (l2.first < l1.second);
}

int main(void) {
  Input in;
  int currentMax;

  std::cin >> in.numberOfPlaces;
  std::cin >> in.numberOfBuckets;
  in.allocateSpace();
  currentMax = -1;
  for (std::size_t i = 0; i < in.numberOfBuckets; i++) {
    std::cin >> in.indices[i];
    currentMax = std::max(currentMax, in.indices[i]);
    std::cin >> in.liveIntervals[i].first;
    std::cin >> in.liveIntervals[i].second;
    std::cin >> in.weight[i];
  }

  ClashGraph g(currentMax, in.weight);

  for (std::size_t i = 0; i < in.numberOfBuckets; i++) {
    for (std::size_t j = i + 1; j < in.numberOfBuckets; j++) {
      if (overlaps(in.liveIntervals[i], in.liveIntervals[j]))
        g.addEdge(in.indices[i], in.indices[j]);
    }
  }

  g.logGraph();
  auto res = g.colourGraph(in.numberOfPlaces);

  for (std::size_t i = 0; i < res.size(); i++) {
    std::cout << "(" << res[i].first + 1 << ", " << res[i].second << ")"
              << std::endl;
  }

  return 0;
}
