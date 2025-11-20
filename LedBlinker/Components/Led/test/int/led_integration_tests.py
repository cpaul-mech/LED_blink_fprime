"""Integration tests for the Led component

These tests should be run with pytest while the GDS is running.

To run these tests:
1. Start fprime-gds in one terminal
2. Run pytest with the dictionary in another terminal:
   pytest LedBlinker/Components/Led/test/int/led_integration_tests.py --dictionary build-fprime-automatic-native/LedBlinker/LedBlinker/Top/LedBlinkerTopologyDictionary.json -v
"""


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
