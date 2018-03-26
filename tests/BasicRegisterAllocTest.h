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

#ifndef _BASIC_REGISTER_ALLOC_TEST_H
#define _BASIC_REGISTER_ALLOC_TEST_H

#include "lib/BasicRegAlloc.h"
#include "tests/BasicRegisterAllocTest.h"
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

template <typename TestingPolicy>
class BasicRegisterAllocTest : private TestingPolicy {
  using TestingPolicy::report;
  using TestingPolicy::verify;
  std::vector<int> indices;
  std::vector<std::pair<int, int>> liveIntervals;
  std::vector<int> weights;
  std::size_t numberOfVirtualRegisters;
  std::size_t numberOfPhysicalRegisters;
  std::string testCasePath;
  std::vector<std::pair<int, int>> result;
  std::string errorMessage;

  void allocateSpace();

public:
  BasicRegisterAllocTest() = default;
  ~BasicRegisterAllocTest() = default;
  bool loadFile(std::string_view _testCasePath);
  void setNumberOfPhysicalRegisters(std::size_t _numberOfPhysicalRegisters);

  const std::string_view getTestCasePath() const;
  const std::size_t getNumberOfPhysicalRegisters() const;
  const std::size_t getNumberOfVirtualRegisters() const;
  const std::vector<int> &getIndices() const;
  const std::vector<std::pair<int, int>> &getLiveIntervals() const;
  const std::vector<int> &getWeights() const;
  const std::vector<std::pair<int, int>> &getResult() const;

  bool run();
};

template <typename TestingPolicy>
void BasicRegisterAllocTest<TestingPolicy>::setNumberOfPhysicalRegisters(
    std::size_t _numberOfPhysicalRegisters) {
  numberOfPhysicalRegisters = _numberOfPhysicalRegisters;
}

template <typename TestingPolicy>
void BasicRegisterAllocTest<TestingPolicy>::allocateSpace() {
  indices.resize(numberOfVirtualRegisters);
  liveIntervals.resize(numberOfVirtualRegisters);
  weights.resize(numberOfVirtualRegisters);
}

template <typename TestingPolicy>
bool BasicRegisterAllocTest<TestingPolicy>::loadFile(
    std::string_view _testCasePath) {
  testCasePath = std::string(_testCasePath);
  std::ifstream testCaseStream(testCasePath);
  int currentMax = -1;

  if (!testCaseStream.is_open())
    return false;

  testCaseStream >> numberOfVirtualRegisters;
  allocateSpace();
  for (std::size_t i = 0; i < numberOfVirtualRegisters; i++) {
    testCaseStream >> indices[i];
    currentMax = std::max(currentMax, indices[i]);
    testCaseStream >> liveIntervals[i].first;
    testCaseStream >> liveIntervals[i].second;
    testCaseStream >> weights[i];
  }

  ClashGraph clashGraph(currentMax, weights);

  for (std::size_t i = 0; i < numberOfVirtualRegisters; i++) {
    for (std::size_t j = i + 1; j < numberOfVirtualRegisters; j++) {
      if (clashes(liveIntervals[i], liveIntervals[j]))
        clashGraph.addEdge(indices[i], indices[j]);
    }
  }

  result = clashGraph.colourGraph(numberOfPhysicalRegisters);
  return true;
}

template <typename TestingPolicy>
bool BasicRegisterAllocTest<TestingPolicy>::run() {
  if (!verify(this, errorMessage))
    return report(errorMessage);
  return true;
}

template <typename TestingPolicy>
const std::string_view
BasicRegisterAllocTest<TestingPolicy>::getTestCasePath() const {
  return testCasePath;
}

template <typename TestingPolicy>
const std::size_t
BasicRegisterAllocTest<TestingPolicy>::getNumberOfPhysicalRegisters() const {
  return numberOfPhysicalRegisters;
}

template <typename TestingPolicy>
const std::size_t
BasicRegisterAllocTest<TestingPolicy>::getNumberOfVirtualRegisters() const {
  return numberOfVirtualRegisters;
}

template <typename TestingPolicy>
const std::vector<int> &
BasicRegisterAllocTest<TestingPolicy>::getIndices() const {
  return indices;
}

template <typename TestingPolicy>
const std::vector<std::pair<int, int>> &
BasicRegisterAllocTest<TestingPolicy>::getLiveIntervals() const {
  return liveIntervals;
}

template <typename TestingPolicy>
const std::vector<int> &
BasicRegisterAllocTest<TestingPolicy>::getWeights() const {
  return weights;
}

template <typename TestingPolicy>
const std::vector<std::pair<int, int>> &
BasicRegisterAllocTest<TestingPolicy>::getResult() const {
  return result;
}

#endif // _BASIC_REGISTER_ALLOC_TEST_H
