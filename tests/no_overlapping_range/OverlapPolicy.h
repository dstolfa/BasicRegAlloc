#ifndef _OVERLAP_POLICY_H_
#define _OVERLAP_POLICY_H_

#include "tests/BasicRegisterAllocTest.h"
#include <vector>

class OverlapPolicy {
protected:
  bool verify(BasicRegisterAllocTest<OverlapPolicy> *test,
              std::string &message) const;
  bool report(std::string_view message) const;
};

#endif // _OVERLAP_POLICY_H_
