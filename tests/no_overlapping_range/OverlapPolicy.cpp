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

#include "OverlapPolicy.h"
#include "lib/BasicRegAlloc.h"
#include <iostream>

bool OverlapPolicy::verify(BasicRegisterAllocTest<OverlapPolicy> *test,
                           std::string &message) const {
  auto liveIntervals = test->getLiveIntervals();
  auto res = test->getResult();

  for (std::size_t i = 1; i < res.size(); i++)
    for (std::size_t j = 0; j < i; j++)
      // Check if we have a clash *and* the same register allocated. If that is
      // the case, we have violated the property we are testing for.
      if (clashes(liveIntervals[res[i].first], liveIntervals[res[j].first]) &&
          res[i].second == res[j].second) {
        message += res[i].first;
        message += ", ";
        message += res[j].first;
        message += " with overlapping live ranges <(";
        message += liveIntervals[res[i].first].first;
        message += ", ";
        message += liveIntervals[res[i].first].second;
        message += "), (";
        message += liveIntervals[res[j].first].first;
        message += ", ";
        message += liveIntervals[res[j].first].second;
        message += ")> have been allocated to <";
        message += res[i].second;
        message += ">";
        return false;
      }
  return true;
}

bool OverlapPolicy::report(std::string_view message) const {
  std::cerr << message << std::endl;
  return false;
}
