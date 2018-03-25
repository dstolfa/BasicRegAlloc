#include "OverlapPolicy.h"
#include "lib/BasicRegAlloc.h"
#include "tests/BasicRegisterAllocTest.h"
#include <experimental/filesystem>
#include <iostream>
#include <vector>

namespace fs = std::experimental::filesystem;

/// This test is designed to check the case where we have more virtual registers
/// than physical registers and where all of the live intervals of the virtual
/// registers overlap. We expect the registers to be greedily allocated
/// depending on the weight, as well as a spill of one register (the one with
/// the lowest weight), however we do not test for that particularl property in
/// this test.
int main(void) {
  fs::path p = "../inputs/1";
  BasicRegisterAllocTest<OverlapPolicy> test;
  test.setNumberOfPhysicalRegisters(2);
  test.loadFile(fs::absolute(p).string());
  if (!test.run())
    return 1;
  return 0;
}
