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

#include "lib/BasicRegAlloc.h"

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

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
      if (clashes(in.liveIntervals[i], in.liveIntervals[j]))
        g.addEdge(in.indices[i], in.indices[j]);
    }
  }

  // Log the clash graph
  g.logGraph();
  auto res = g.colourGraph(in.numberOfPlaces);

  for (std::size_t i = 0; i < res.size(); i++) {
    std::cout << "(" << res[i].first + 1 << ", " << res[i].second << ")"
              << std::endl;
  }

  return 0;
}
