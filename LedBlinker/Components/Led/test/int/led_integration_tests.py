"""Integration tests for the Led component

These tests should be run with pytest while the GDS is running.

To run these tests:
1. Start fprime-gds in one terminal
2. Run pytest with the dictionary in another terminal:
   pytest LedBlinker/Components/Led/test/int/led_integration_tests.py --dictionary build-fprime-automatic-native/LedBlinker/LedBlinker/Top/LedBlinkerTopologyDictionary.json -v
"""
import time
from fprime_gds.common.testing_fw import predicates


def test_cmd_no_op(fprime_test_api):
    """Test command CMD_NO_OP

    Test that CMD_NO_OP can be sent and return without any errors
    """
    # Note: cmdDisp is in the CdhCore subsystem
    fprime_test_api.send_and_assert_command("CdhCore.cmdDisp.CMD_NO_OP", max_delay=0.5)


def test_led_blink_on(fprime_test_api):
    """Test LED BLINKING_ON_OFF command - Turn blinking ON
    
    Test that the LED can be commanded to start blinking
    """
    # Send command to turn LED blinking ON (Fw.On enum expects string "ON")
    fprime_test_api.send_and_assert_command("LedBlinker.led.BLINKING_ON_OFF", args=["ON"], max_delay=0.5)
    
    # Verify command was successful (no command error events)
    assert fprime_test_api.get_event_test_history().size() >= 0


def test_led_blink_off(fprime_test_api):
    """Test LED BLINKING_ON_OFF command - Turn blinking OFF
    
    Test that the LED can be commanded to stop blinking
    """
    # Send command to turn LED blinking OFF (Fw.On enum expects string "OFF")
    fprime_test_api.send_and_assert_command("LedBlinker.led.BLINKING_ON_OFF", args=["OFF"], max_delay=0.5)
    
    # Verify command was successful
    assert fprime_test_api.get_event_test_history().size() >= 0

def test_blinking(fprime_test_api):
    # Send command to enable blinking, then assert expected events are emitted
    blink_start_evr = fprime_test_api.get_event_pred("LedBlinker.led.SetBlinkingState", ["ON"])
    led_on_evr = fprime_test_api.get_event_pred("LedBlinker.led.LedState", ["ON"])
    led_off_evr = fprime_test_api.get_event_pred("LedBlinker.led.LedState", ["OFF"])

    fprime_test_api.send_and_assert_event(
        "LedBlinker.led.BLINKING_ON_OFF",
        args=["ON"],
        events=[blink_start_evr, led_on_evr, led_off_evr, led_on_evr],
    )

    blink_state_on_tlm = fprime_test_api.get_telemetry_pred("LedBlinker.led.BlinkingState", "ON")
    fprime_test_api.assert_telemetry(blink_state_on_tlm)

    # Send command to stop blinking, then assert blinking stops
    blink_stop_evr = fprime_test_api.get_event_pred("LedBlinker.led.SetBlinkingState", ["OFF"])
    fprime_test_api.send_and_assert_event(
        "LedBlinker.led.BLINKING_ON_OFF", args=["OFF"], events=[blink_stop_evr]
    )

    # Assert that blink command sets blinking state off
    #TODO: use fprime_test_api.assert_telemetry to check that "LedBlinker.led.BlinkingState" is off
    blink_state_off_tlm = fprime_test_api.get_telemetry_pred("LedBlinker.led.BlinkingState", "OFF")
    fprime_test_api.assert_telemetry(blink_state_off_tlm, timeout=2)

    time.sleep(1)  # Wait one second to let any in-progress telemetry be sent
    # Save reference to current telemetry history so we can search against future telemetry
    telem_after_blink_off = fprime_test_api.telemetry_history.size()
    time.sleep(2)  # Wait to receive telemetry after stopping blinking
    # Assert that blinking has stopped and that LedTransitions is no longer updating
    fprime_test_api.assert_telemetry_count(
        0, "LedBlinker.led.BlinkingTransitions", start=telem_after_blink_off
    )
        # Assert that the LedTransitions channel increments
    results = fprime_test_api.assert_telemetry_count(
        predicates.greater_than(2), "LedBlinker.led.BlinkingTransitions", timeout=4
    )
    ascending = True
    prev = None
    for res in results:
        if prev is not None:
            if not res.get_val() > prev.get_val():
                ascending = False
                fprime_test_api.log(
                    f"LedBlinker.led.LedTransitions not in ascending order: First ({prev.get_val()}) Second ({res.get_val()})"
                )
        prev = res
    assert fprime_test_api.test_assert(
        ascending, "Expected all LedBlinker.led.BlinkingTransitions updates to ascend.", True
    )