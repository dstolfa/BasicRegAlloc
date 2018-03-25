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
