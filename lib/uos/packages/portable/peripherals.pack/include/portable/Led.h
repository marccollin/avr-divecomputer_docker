//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

#ifndef PORTABLE_LED_H_
#define PORTABLE_LED_H_

// ----------------------------------------------------------------------------

// Static implementation, to be used as base for actual classes.

template< //
    typename GpioPin_T, //
    template<typename > class PowerPolicy_T, //
    template<typename > class ActivePolicy_T //
>
  class TLedImplementation : public PowerPolicy_T<GpioPin_T>
  {
    using GpioPin = GpioPin_T;
    using PowerPolicy = PowerPolicy_T<GpioPin_T>;
    using ActivePolicy = ActivePolicy_T<GpioPin_T>;

  protected:

    TLedImplementation() = default;

    inline static void
    __attribute__((always_inline))
    powerUp(GpioPin* gpioPin)
    {
      PowerPolicy::powerUp(*gpioPin);
      ActivePolicy::makeInactive(*gpioPin);
    }

    inline static void
    __attribute__((always_inline))
    powerDown(GpioPin* gpioPin)
    {
      ActivePolicy::makeInactive(*gpioPin);
      PowerPolicy::powerDown(*gpioPin);
    }

    inline static void
    __attribute__((always_inline))
    turnOn(GpioPin* gpioPin)
    {
      ActivePolicy::makeActive(*gpioPin);
    }

    inline static void
    __attribute__((always_inline))
    turnOff(GpioPin* gpioPin)
    {
      ActivePolicy::makeInactive(*gpioPin);
    }

    inline static void
    __attribute__((always_inline))
    toggle(GpioPin* gpioPin)
    {
      gpioPin->toggle();
    }

    inline static bool
    __attribute__((always_inline))
    isOn(GpioPin* gpioPin)
    {
      return ActivePolicy::isActive(*gpioPin);
    }
  };

// ----------------------------------------------------------------------------

// Interface for virtual classes

template<typename GpioPin_T>
  class TAbstractLed
  {
    using GpioPin = GpioPin_T;

  public:

    TAbstractLed() = default;

    virtual
    ~TAbstractLed()
    {
    }

    virtual void
    powerUp(void) const = 0;

    virtual void
    powerDown(void) const = 0;

    virtual void
    turnOn(void) const = 0;

    virtual void
    turnOff(void) const = 0;

    virtual void
    toggle(void) const = 0;

    virtual bool
    isOn(void) const = 0;

  };

// ----------------------------------------------------------------------------

template< //
    typename GpioPin_T, //
    template<typename > class PowerPolicy_T, //
    template<typename > class ActivePolicy_T //
>
  class TVirtualLed : public TAbstractLed<GpioPin_T>, //
      private TLedImplementation<GpioPin_T, PowerPolicy_T, ActivePolicy_T>
  {
    using GpioPin = GpioPin_T;
    using Implementation = TLedImplementation<GpioPin_T, PowerPolicy_T, ActivePolicy_T>;

  public:

    TVirtualLed(GpioPin& gpioPin_) :
        gpioPin(gpioPin_)
    {
    }

    virtual
    ~TVirtualLed()
    {
    }

    virtual void
    powerUp(void) const
    {
      Implementation::powerUp(&this->gpioPin);
    }

    virtual void
    powerDown(void) const
    {
      Implementation::powerDown(&this->gpioPin);
    }

    virtual void
    turnOn(void) const
    {
      Implementation::turnOn(&this->gpioPin);
    }

    virtual void
    turnOff(void) const
    {
      Implementation::turnOff(&this->gpioPin);
    }

    virtual void
    toggle(void) const
    {
      Implementation::toggle(&this->gpioPin);
    }

    virtual bool
    isOn(void) const
    {
      return Implementation::isOn(&this->gpioPin);
    }

  protected:

    GpioPin& gpioPin;

  };

// ----------------------------------------------------------------------------

template< //
    typename GpioPin_T, //
    template<typename > class PowerPolicy_T, //
    template<typename > class ActivePolicy_T //
>
  class TAllocatedLed : private TLedImplementation<GpioPin_T, PowerPolicy_T,
      ActivePolicy_T>
  {
    using GpioPin = GpioPin_T;
    using Implementation = TLedImplementation<GpioPin_T, PowerPolicy_T, ActivePolicy_T>;

  public:

    TAllocatedLed(GpioPin& gpioPin_) :
        gpioPin(gpioPin_)
    {
    }

    void
    powerUp(void) const
    {
      Implementation::powerUp(&this->gpioPin);
    }

    void
    powerDown(void) const
    {
      Implementation::powerDown(&this->gpioPin);
    }

    inline void
    turnOn(void) const
    {
      Implementation::turnOn(&this->gpioPin);
    }

    inline void
    turnOff(void) const
    {
      Implementation::turnOff(&this->gpioPin);
    }

    inline void
    toggle(void) const
    {
      Implementation::toggle(&this->gpioPin);
    }

    inline bool
    isOn(void) const
    {
      return Implementation::isOn(&this->gpioPin);
    }

  protected:

    GpioPin& gpioPin;

  };

// ----------------------------------------------------------------------------

template< //
    typename GpioPin_T, //
    template<typename > class PowerPolicy_T, //
    template<typename > class ActivePolicy_T /* = TActiveLowPolicy */ //
>
  class TConstantLed : private TLedImplementation<GpioPin_T, PowerPolicy_T,
      ActivePolicy_T>
  {
    using GpioPin = GpioPin_T;
    using Implementation = TLedImplementation<GpioPin_T, PowerPolicy_T, ActivePolicy_T>;

  public:

    TConstantLed() = default;

    TConstantLed(GpioPin& gpioPin __attribute__((unused)))
    {
    }

    inline static void
    __attribute__((always_inline))
    powerUp(void)
    {
      Implementation::powerUp(nullptr);
    }

    inline static void
    __attribute__((always_inline))
    powerDown(void)
    {
      Implementation::powerDown(nullptr);
    }

    inline static void
    __attribute__((always_inline))
    turnOn(void)
    {
      Implementation::turnOn(nullptr);
    }

    inline static void
    __attribute__((always_inline))
    turnOff(void)
    {
      Implementation::turnOff(nullptr);
    }

    inline static void
    __attribute__((always_inline))
    toggle(void)
    {
      Implementation::toggle(nullptr);
    }

    inline static bool
    __attribute__((always_inline))
    isOn(void)
    {
      return Implementation::isOn(nullptr);
    }

  };

// ----------------------------------------------------------------------------

#endif // PORTABLE_LED_H_
