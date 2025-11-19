// ======================================================================
// \title  LedTester.cpp
// \author cpaulmech
// \brief  cpp file for Led component test harness implementation class
// ======================================================================

#include "LedTester.hpp"

namespace Components {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

LedTester ::LedTester() : LedGTestBase("LedTester", LedTester::MAX_HISTORY_SIZE), component("Led") {
    this->initComponents();
    this->connectPorts();
}

LedTester ::~LedTester() {}

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void LedTester ::toDo() {
    // TODO
}

}  // namespace Components
