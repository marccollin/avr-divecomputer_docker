//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#include "stm32f4xx.h"
#include "stm32f4discovery/Leds.h"

// ----------------------------------------------------------------------------

void
myConstLeds();

void
myAllocatedLeds();

void
myAllocatedVarLeds();

void
myVirtualLeds();

void
myVirtualVarLeds();

gpio::PortId ledsPortId;
gpio::PortBitId pinIds[4];

int
main(void)
{
  myConstLeds();

  ledsPortId = gpio::PortId::D;
  pinIds[0] = gpio::PortBitId::_12;
  pinIds[1] = gpio::PortBitId::_13;
  pinIds[2] = gpio::PortBitId::_14;
  pinIds[3] = gpio::PortBitId::_15;

  myAllocatedLeds();

  myAllocatedVarLeds();

  myVirtualLeds();

  myVirtualVarLeds();
}

void
myConstLeds()
{
  using namespace stm32f4discovery;

  led4Green.powerUp();
  led4Green.turnOn();
  led4Green.turnOff();
  led4Green.toggle();
  led4Green.isOn();
  led4Green.powerDown();

  led3Orange.powerUp();
  led3Orange.turnOn();
  led3Orange.turnOff();
  led3Orange.toggle();
  led3Orange.isOn();
  led3Orange.powerDown();

  led5Red.powerUp();
  led5Red.turnOn();
  led5Red.turnOff();
  led5Red.toggle();
  led5Red.isOn();
  led5Red.powerDown();

  led6Blue.powerUp();
  led6Blue.turnOn();
  led6Blue.turnOff();
  led6Blue.toggle();
  led6Blue.isOn();
  led6Blue.powerDown();
}

void
myAllocatedLeds()
{
  using namespace stm32f4;

  // using GpioPort = TAllocatedGpioPort<TConstantGpioPortNoPowerDownPolicy>;
  using GpioPort = TAllocatedGpioPort<TAllocatedGpioPortPowerDownPolicy>;

  GpioPort portD
    { gpio::PortId::D };

  // For brevity, the pin definition is aliased.
  using GpioPin = TAllocatedGpioPin<GpioPort>;

  // Instantiate the 4 pins used by the leds (allocated objects).
  GpioPin pinD12
    { portD, gpio::PortBitId::_12 };
  GpioPin pinD13
    { portD, gpio::PortBitId::_13 };
  GpioPin pinD14
    { portD, gpio::PortBitId::_14 };
  GpioPin pinD15
    { portD, gpio::PortBitId::_15 };

  // For brevity, the led definition using the OutputPin and
  // ActiveHigh policies is aliased to a shorter name.
  using Led = TAllocatedLed<GpioPin, TGpioOutputPinPolicy, TActiveHighPolicy>;

  // Instantiate the 4 leds, using the pins.
  Led led4GreenA
    { pinD12 };
  Led led3OrangeA
    { pinD13 };
  Led led5RedA
    { pinD14 };
  Led led6BlueA
    { pinD15 };

  led4GreenA.powerUp();
  led4GreenA.turnOn();
  led4GreenA.turnOff();
  led4GreenA.toggle();
  led4GreenA.isOn();
  led4GreenA.powerDown();

  led3OrangeA.powerUp();
  led3OrangeA.turnOn();
  led3OrangeA.turnOff();
  led3OrangeA.toggle();
  led3OrangeA.isOn();
  led3OrangeA.powerDown();

  led5RedA.powerUp();
  led5RedA.turnOn();
  led5RedA.turnOff();
  led5RedA.toggle();
  led5RedA.isOn();
  led5RedA.powerDown();

  led6BlueA.powerUp();
  led6BlueA.turnOn();
  led6BlueA.turnOff();
  led6BlueA.toggle();
  led6BlueA.isOn();
  led6BlueA.powerDown();
}

void
myAllocatedVarLeds()
{
  using namespace stm32f4;

  // using GpioPort = TAllocatedGpioPort<TConstantGpioPortNoPowerDownPolicy>;
  using GpioPort = TAllocatedGpioPort<TAllocatedGpioPortPowerDownPolicy>;

  GpioPort portD
    { ledsPortId };

  // For brevity, the pin definition is aliased.
  using GpioPin = TAllocatedGpioPin<GpioPort>;

  // Instantiate the 4 pins used by the leds (allocated objects).
  GpioPin pinD12
    { portD, pinIds[0] };
  GpioPin pinD13
    { portD, pinIds[1] };
  GpioPin pinD14
    { portD, pinIds[2] };
  GpioPin pinD15
    { portD, pinIds[3] };

  // For brevity, the led definition using the OutputPin and
  // ActiveHigh policies is aliased to a shorter name.
  using Led = TAllocatedLed<GpioPin, TGpioOutputPinPolicy, TActiveHighPolicy>;

  // Instantiate the 4 leds, using the pins.
  Led led4GreenA
    { pinD12 };
  Led led3OrangeA
    { pinD13 };
  Led led5RedA
    { pinD14 };
  Led led6BlueA
    { pinD15 };

  led4GreenA.powerUp();
  led4GreenA.turnOn();
  led4GreenA.turnOff();
  led4GreenA.toggle();
  led4GreenA.isOn();
  led4GreenA.powerDown();

  led3OrangeA.powerUp();
  led3OrangeA.turnOn();
  led3OrangeA.turnOff();
  led3OrangeA.toggle();
  led3OrangeA.isOn();
  led3OrangeA.powerDown();

  led5RedA.powerUp();
  led5RedA.turnOn();
  led5RedA.turnOff();
  led5RedA.toggle();
  led5RedA.isOn();
  led5RedA.powerDown();

  led6BlueA.powerUp();
  led6BlueA.turnOn();
  led6BlueA.turnOff();
  led6BlueA.toggle();
  led6BlueA.isOn();
  led6BlueA.powerDown();
}

void
myVirtualLeds()
{
  using namespace stm32f4;

  // using GpioPort = TAllocatedGpioPort<TConstantGpioPortNoPowerDownPolicy>;
  using GpioPort = TAllocatedGpioPort<TAllocatedGpioPortPowerDownPolicy>;

  GpioPort portD
    { gpio::PortId::D };

  // For brevity, the pin definition is aliased.
  using GpioPin = TAllocatedGpioPin<GpioPort>;

  // Instantiate the 4 pins used by the leds (allocated objects).
  GpioPin pinD12
    { portD, gpio::PortBitId::_12 };
  GpioPin pinD13
    { portD, gpio::PortBitId::_13 };
  GpioPin pinD14
    { portD, gpio::PortBitId::_14 };
  GpioPin pinD15
    { portD, gpio::PortBitId::_15 };

  // For brevity, the led definition using the OutputPin and
  // ActiveHigh policies is aliased to a shorter name.
  using Led = TVirtualLed<GpioPin, TGpioOutputPinPolicy, TActiveHighPolicy>;

  // Instantiate the 4 leds, using the pins.
  Led led4GreenA
    { pinD12 };
  Led led3OrangeA
    { pinD13 };
  Led led5RedA
    { pinD14 };
  Led led6BlueA
    { pinD15 };

  led4GreenA.powerUp();
  led4GreenA.turnOn();
  led4GreenA.turnOff();
  led4GreenA.toggle();
  led4GreenA.isOn();
  led4GreenA.powerDown();

  led3OrangeA.powerUp();
  led3OrangeA.turnOn();
  led3OrangeA.turnOff();
  led3OrangeA.toggle();
  led3OrangeA.isOn();
  led3OrangeA.powerDown();

  led5RedA.powerUp();
  led5RedA.turnOn();
  led5RedA.turnOff();
  led5RedA.toggle();
  led5RedA.isOn();
  led5RedA.powerDown();

  led6BlueA.powerUp();
  led6BlueA.turnOn();
  led6BlueA.turnOff();
  led6BlueA.toggle();
  led6BlueA.isOn();
  led6BlueA.powerDown();
}

void
myVirtualVarLeds()
{
  using namespace stm32f4;

  // using GpioPort = TAllocatedGpioPort<TConstantGpioPortNoPowerDownPolicy>;
  using GpioPort = TAllocatedGpioPort<TAllocatedGpioPortPowerDownPolicy>;

  GpioPort portD
    { ledsPortId };

  // For brevity, the pin definition is aliased.
  using GpioPin = TAllocatedGpioPin<GpioPort>;

  // Instantiate the 4 pins used by the leds (allocated objects).
  GpioPin pinD12
    { portD, pinIds[0] };
  GpioPin pinD13
    { portD, pinIds[1] };
  GpioPin pinD14
    { portD, pinIds[2] };
  GpioPin pinD15
    { portD, pinIds[3] };

  // For brevity, the led definition using the OutputPin and
  // ActiveHigh policies is aliased to a shorter name.
  using Led = TVirtualLed<GpioPin, TGpioOutputPinPolicy, TActiveHighPolicy>;

  // Instantiate the 4 leds, using the pins.
  Led led4GreenA
    { pinD12 };
  Led led3OrangeA
    { pinD13 };
  Led led5RedA
    { pinD14 };
  Led led6BlueA
    { pinD15 };

  led4GreenA.powerUp();
  led4GreenA.turnOn();
  led4GreenA.turnOff();
  led4GreenA.toggle();
  led4GreenA.isOn();
  led4GreenA.powerDown();

  led3OrangeA.powerUp();
  led3OrangeA.turnOn();
  led3OrangeA.turnOff();
  led3OrangeA.toggle();
  led3OrangeA.isOn();
  led3OrangeA.powerDown();

  led5RedA.powerUp();
  led5RedA.turnOn();
  led5RedA.turnOff();
  led5RedA.toggle();
  led5RedA.isOn();
  led5RedA.powerDown();

  led6BlueA.powerUp();
  led6BlueA.turnOn();
  led6BlueA.turnOff();
  led6BlueA.toggle();
  led6BlueA.isOn();
  led6BlueA.powerDown();
}

