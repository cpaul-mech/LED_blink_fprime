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

void LedTester ::testBlinking() {
    // This test will make use of parameters. So need to load them.
    this->component.loadParameters();

    // Ensure LED stays off when blinking is disabled
    // The Led component defaults to blinking off
    this->invoke_to_run(0, 0);     // invoke the 'run' port to simulate running one cycle
    this->component.doDispatch();  // Trigger execution of async port

    ASSERT_EVENTS_LedState_SIZE(0);  // ensure no LedState change events we emitted

    ASSERT_from_gpioWrite_SIZE(0);  // ensure gpio LED wasn't set

    ASSERT_TLM_BlinkingTransitions_SIZE(0);  // ensure no LedTransitions were recorded

    // Send command to enable blinking
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();  // Trigger execution of async command
    ASSERT_CMD_RESPONSE_SIZE(1);   // ensure a command response was emitted
    ASSERT_CMD_RESPONSE(0, Led::OPCODE_BLINKING_ON_OFF, 0,
                        Fw::CmdResponse::OK);  // ensure the expected command response was emitted
    // Step through 3 run cycles to observe LED turning on and off 3 times
    // Cycle 1: LED initalization->On
    this->invoke_to_run(0, 0);
    this->component.doDispatch();  // Trigger execution of async port
    ASSERT_EVENTS_LedState_SIZE(1);
    ASSERT_EVENTS_LedState(0, Fw::On::ON);
    ASSERT_from_gpioWrite_SIZE(1);
    ASSERT_from_gpioWrite(0, Fw::Logic::HIGH);
    ASSERT_TLM_BlinkingTransitions_SIZE(1);
    ASSERT_TLM_BlinkingTransitions(0, 1);

    // Cycle 2: LED On->Off
    this->invoke_to_run(0, 0);
    this->component.doDispatch();  // Trigger execution of async port
    ASSERT_EVENTS_LedState_SIZE(2);
    ASSERT_EVENTS_LedState(1, Fw::On::OFF);
    ASSERT_from_gpioWrite_SIZE(2);
    ASSERT_from_gpioWrite(1, Fw::Logic::LOW);
    // TODO: Add assertions for LedTransitions telemetry
    ASSERT_TLM_BlinkingTransitions_SIZE(2);
    ASSERT_TLM_BlinkingTransitions(1, 2);

    // Cycle 3: LED Off->On
    this->invoke_to_run(0, 0);
    this->component.doDispatch();  // Trigger execution of async port
    //TODO: Write assertions for third cycle
    ASSERT_EVENTS_LedState_SIZE(3);
    ASSERT_EVENTS_LedState(2, Fw::On::ON);
    ASSERT_from_gpioWrite_SIZE(3);
    ASSERT_from_gpioWrite(2, Fw::Logic::HIGH);
    ASSERT_TLM_BlinkingTransitions_SIZE(3);
    ASSERT_TLM_BlinkingTransitions(2, 3);
    
}

void LedTester ::testBlinkInterval() {
    //Enable LED Blinking:
    this->sendCmd_BLINKING_ON_OFF(0, 0, Fw::On::ON);
    this->component.doDispatch();

    //Adjust blink interval to 4 cycles
    U32 blinkInterval = 4;
    this->paramSet_BlinkingInterval(blinkInterval, Fw::ParamValid::VALID);
    this->paramSend_BlinkingInterval(0,0);
    ASSERT_EVENTS_BlinkIntervalSet_SIZE(1);

    //Run for 12 cycles (3 transitions: at cycle 0, 4, and 8)
    for (U32 i = 0; i < 12; i++) {
        this->invoke_to_run(0, 0);
        this->component.doDispatch();
        
        // Check state transitions happen only at multiples of blinkInterval
        if (i % blinkInterval == 0) {
            U32 transitionNum = i / blinkInterval;
            
            // Verify we have the expected number of events so far
            ASSERT_EVENTS_LedState_SIZE(transitionNum + 1);
            
            // Verify the LED state alternates: ON, OFF, ON, OFF, ...
            // Even transitions (0, 2, 4...) should be ON
            // Odd transitions (1, 3, 5...) should be OFF
            if (transitionNum % 2 == 0) {
                ASSERT_EVENTS_LedState(transitionNum, Fw::On::ON);
            } else {
                ASSERT_EVENTS_LedState(transitionNum, Fw::On::OFF);
            }
        }
    }
    
    // Final check: After 12 cycles with interval of 4, we should have exactly 3 transitions
    ASSERT_EVENTS_LedState_SIZE(3);
}

}  // namespace Components
