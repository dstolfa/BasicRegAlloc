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

#ifndef _BASIC_REG_ALLOC_H_
#define _BASIC_REG_ALLOC_H_

#include <vector>

/// A helper function that can be used to check whether or not a live range
/// causes a clash in the clash graph.
bool clashes(std::pair<int, int> &l1, std::pair<int, int> &l2);

/// This class contains all of the necessary data to form a clash graph that
/// will be used for register allocation.
///
/// The assumptions made by this class:
///   * The virtual register indices are monotonically increasing, starting from
///     virtual register one (1).
///   * The target architecture contains only orthogonal instructions. This
///     means that this register allocator does not support any instructions
///     which implicitly writes to a certain register.
class ClashGraph {
  std::vector<std::vector<int>> adjMatrix; // Graph adjacency matrix
  std::vector<std::size_t> deg;            // Degree of each node
  std::vector<int> weight;                 // Weight of each node

public:
  ClashGraph(std::size_t _matSize, std::vector<int> &w);
  void addEdge(int n1, int n2);
  void logGraph() const;
  std::vector<std::pair<int, int>> colourGraph(std::size_t k) const;
};

#endif
