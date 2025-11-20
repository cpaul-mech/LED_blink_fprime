# LED Component Integration Tests

These integration tests verify the LED component works correctly in the full system.

## Prerequisites

1. The deployment must be built first
2. The GDS must be running and connected to the deployment

## How to Run

### Step 1: Build the deployment

```bash
cd /path/to/led-blinker
fprime-util build
```

### Step 2: Start the GDS

```bash
fprime-gds
```

This will:
- Start the deployment application
- Start the web interface at http://localhost:5000
- Make the fprime_test_api fixture available to pytest

### Step 3: Run the tests (in a separate terminal)

```bash
cd /path/to/led-blinker
source fprime-venv/bin/activate
pytest LedBlinker/Components/Led/test/int/led_integration_tests.py --dictionary build-fprime-automatic-native/LedBlinker/LedBlinker/Top/LedBlinkerTopologyDictionary.json -v
```

Or run a specific test:

```bash
pytest LedBlinker/Components/Led/test/int/led_integration_tests.py::test_cmd_no_op --dictionary build-fprime-automatic-native/LedBlinker/LedBlinker/Top/LedBlinkerTopologyDictionary.json -v
```

**Example output when all tests pass:**
```
============================= test session starts ==============================
LedBlinker/Components/Led/test/int/led_integration_tests.py::test_cmd_no_op PASSED [ 33%]
LedBlinker/Components/Led/test/int/led_integration_tests.py::test_led_blink_on PASSED [ 66%]
LedBlinker/Components/Led/test/int/led_integration_tests.py::test_led_blink_off PASSED [100%]
============================== 3 passed in 0.41s =======================================
```

## Test Details

- `test_cmd_no_op`: Verifies basic commanding works
- `test_led_blink_on`: Tests turning LED blinking ON
- `test_led_blink_off`: Tests turning LED blinking OFF

## Troubleshooting

**Error: `fixture 'fprime_test_api' not found`**
- Make sure the GDS is running first before running pytest
- Verify fprime-gds is installed: `pip show fprime-gds`

**Error: Command or event not found**
- Check the command/event names match your component definitions
- Use the GDS web interface to verify command names

**Tests timeout**
- Increase the `timeout` parameter in test calls
- Verify the deployment is actually running
- Check that the GDS is connected to the deployment

