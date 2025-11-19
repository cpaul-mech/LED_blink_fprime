module Components {
    @ Component to blink an LED driven by a rate group
    active component Led {

        # One async command/port is required for active components
        # This should be overridden by the developers with a useful command/port
        @ TODO
        async command BLINKING_ON_OFF(
            onOff: Fw.On @< Indicates whether the LED should be on or off
        )

        @ reports the state we set to blinking
        event SetBlinkingState($state: Fw.On) \
            severity activity high \
            id 0 \
            format "Set blinking state to {}"


        @ Sets the blinking interval in milliseconds 
        event BlinkIntervalSet(interval: U32) \
            severity activity high \
            id 1 \
            format "Set blinking interval to {} milliseconds"

        @ Sets the LED State On or off
        event LedState($state: Fw.On) \
            severity activity low \
            id 2 \
            format "Set LED state to {}"
        
        @ Telemetry channel for the blinking state
        telemetry BlinkingState: Fw.On 

        @Telemetry channel for the Number of blinking transitions
        telemetry BlinkingTransitions: U64

        @ Blinking Interval in rate Group Ticks
        param BlinkingInterval: U32 default 1

        @ Port receiving calls from the rate group
        async input port run: Svc.Sched

        @ Port for sending calls to the GPIO driver
        output port gpioSet: Drv.GpioWrite

        

        ##############################################################################
        #### Uncomment the following examples to start customizing your component ####
        ##############################################################################

        # @ Example async command
        # async command COMMAND_NAME(param_name: U32)

        # @ Example telemetry counter
        # telemetry ExampleCounter: U64

        # @ Example event
        # event ExampleStateEvent(example_state: Fw.On) severity activity high id 0 format "State set to {}"

        # @ Example port: receiving calls from the rate group
        # sync input port run: Svc.Sched

        # @ Example parameter
        # param PARAMETER_NAME: U32

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut

        @ Port to return the value of a parameter
        param get port prmGetOut

        @Port to set the value of a parameter
        param set port prmSetOut

    }
}