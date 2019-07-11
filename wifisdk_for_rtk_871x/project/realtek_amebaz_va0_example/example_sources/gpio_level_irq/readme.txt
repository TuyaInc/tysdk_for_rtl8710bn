Example Description

This example describes how to implement high/low level trigger on 1 gpio pin.

Pin name PA_12 and PA_5 map to GPIOA_12 and GPIOA_5:
Connect PA_12 and PA_5
 - PA_12 as gpio input high/low level trigger.
 - PA_5 as gpio output

In this example, PA_5 is signal source that change level to high and low periodically.

PA_12 setup to listen low level events in initial.
When PA_12 catch low level events, it disable the irq to avoid receiving duplicate events.
(NOTE: the level events will keep invoked if level keeps in same level)

Then PA_12 is configured to listen high level events and enable irq.
As PA_12 catches high level events, it changes back to listen low level events.

Thus PA_12 can handle both high/low level events.

In this example, you will see log that prints high/low level event periodically.
