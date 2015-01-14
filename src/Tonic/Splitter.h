//
//  Splitter.h
//  Tonic 
//
//  Created by Morgan Packard on 1/14/15.
//
// See LICENSE.txt for license and usage information.
//

/*
	Separate a stereo signal into two mono signals.
*/

#ifndef TONIC_SPLITTER_H
#define TONIC_SPLITTER_H

#include "Generator.h"

namespace Tonic {

	namespace Tonic_ {

		class Splitter_ : public Generator_{

		protected:
			Generator input_;
	
		public:
			Splitter_();
			~Splitter_();
			void setInput(Generator);
			void computeSynthesisBlock(const SynthesisContext_ &context);
			TonicFrames* getOutputFramesPointer();
		};
	}


	class Splitter : public TemplatedGenerator<Tonic_::Splitter_>{

	protected:

		Generator createSplitterChannel(int channel);

	public:
		TONIC_MAKE_GEN_SETTERS(Splitter, input, setInput)
		Generator left();
		Generator right();
	};
}

#endif


