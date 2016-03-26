#ifndef __Tonic__ControlSineOsc__
#define __Tonic__ControlSineOsc__

#include "ControlGenerator.h"

namespace Tonic{

  namespace Tonic_{

    class ControlSineOsc_ : public ControlGenerator_{

    ControlGenerator freq_;
    float phase_;

    public:

      ControlSineOsc_();

      void computeOutput(const SynthesisContext_ & context);
      void setFreq(ControlGenerator gen){freq_ = gen;}
    };

  }

  class ControlSineOsc : public TemplatedControlGenerator<Tonic_::ControlSineOsc_>
  {
  public:

    TONIC_MAKE_CTRL_GEN_SETTERS(ControlSineOsc, freq, setFreq)

  };

}


#endif
