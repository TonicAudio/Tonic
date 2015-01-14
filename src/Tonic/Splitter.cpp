//
//  Splitter.cpp
//  Tonic
//
//  Created by Morgan Packard on 10/26/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "Splitter.h"
#include "ControlTrigger.h"

namespace Tonic {
	namespace Tonic_{



		/////////////////////////////////  Splitter Channel ///////////////////////////////////////////////

		class SplitterChannel_ : public Generator_{

		protected:
			TonicFrames parentOutput_; // pointer to parent output
			Splitter parent_; // 
			int channelNumber_;

		public:
			SplitterChannel_(){
				setIsStereoOutput(false);
				parentOutput_.resize(kSynthesisBlockSize, 2);
			};
			~SplitterChannel_(){};
			void setParent(Splitter parent){ parent_ = parent; };
			void computeSynthesisBlock(const SynthesisContext_ &context){
				int parentChannels = parentOutput_.channels();

				parent_.tick(parentOutput_, context);
				TonicFloat* readHead = &parentOutput_[channelNumber_];
				TonicFloat* writeHead = &outputFrames_[0];
				for (int i = 0; i < kSynthesisBlockSize; i++)
				{
					*writeHead = *readHead;
					writeHead++;
					readHead += parentChannels;
				}
				
			};
			void setChannelNumber(int num){ channelNumber_ = num; };

		};

		class SplitterChannel : public TemplatedGenerator<Tonic_::SplitterChannel_>{

		public:
			void setParent(Splitter parent){ gen()->setParent(parent); };
			void setChannelNumber(int num){ gen()->setChannelNumber(num); };
		};    


		/////////////////////////////////////////////////////////////////////////////////////////////////



		Splitter_::Splitter_() {
			setIsStereoOutput(true);
		}

		Splitter_::~Splitter_(){

		}

		inline void Splitter_::computeSynthesisBlock(const SynthesisContext_ &context){
#ifdef TONIC_DEBUG
			if (!input_.isStereoOutput())
			{
				error(" Splitter_::computeSynthesisBlock. Input must be stereo", true);
			}
#endif // DEBUG
			input_.tick(outputFrames_, context);

		}

		void Splitter_::setInput(Generator in)
		{
			input_ = in;
		}

		TonicFrames* Splitter_::getOutputFramesPointer()
		{
			return &outputFrames_;
		}



	} // Namespace Tonic_


	Tonic::Generator Splitter::createSplitterChannel(int channel)
	{
		Tonic_::SplitterChannel splitterChannel;
		splitterChannel.setChannelNumber(channel);
		splitterChannel.setParent((Splitter)(*this));
		return splitterChannel;
	}



	Tonic::Generator Splitter::left()
	{
		return createSplitterChannel(0);
	}

	Tonic::Generator Splitter::right()
	{
		return createSplitterChannel(1);
	}



} // Namespace Tonic
