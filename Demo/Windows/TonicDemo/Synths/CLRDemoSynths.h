// Synths.h

#pragma once

#include <Tonic.h>

using namespace System;
using namespace System::Collections::Generic;

namespace Tonic {
	
	public ref class CLRControlValue
	{
	public:
		static CLRControlValue^ wrap(ControlValue nativeValue)
		{
			return gcnew CLRControlValue();
		}
	};

	public ref class CLRSynthParameter
	{
	public:
		String^          name;
		CLRControlValue^ value;
		int				 type;
		float            min;
		float            max;

		static CLRSynthParameter^ wrap(Synth::SynthParameter nativeParam)
		{
			auto param = gcnew CLRSynthParameter();
			param->name = gcnew String(nativeParam.name.c_str());
			param->value = CLRControlValue::wrap(nativeParam.value);
			param->type = nativeParam.type;
			param->min = nativeParam.min;
			param->max = nativeParam.max;
			return param;
		}
	};

	public ref class CLRSynthWrapper abstract
	{
	public:
		List<CLRSynthParameter^>^ GetParameters() 
		{
			auto nativeParams = getNativeSynth()->getParameters();
			auto params = gcnew List<CLRSynthParameter^>();
			for (auto p : nativeParams)
			{
				params->Add(CLRSynthParameter::wrap(p));
			}
			return params;
		}

	protected:
		virtual Synth* getNativeSynth() { return m_pNativeSynth; }
		Synth* m_pNativeSynth;
	};
}

class CompressorDuckingTestSynth : public Tonic::Synth
{
public:
	CompressorDuckingTestSynth();
};

public ref class CLRCompressorDuckingTestSynth : public Tonic::CLRSynthWrapper
{
public:
	CLRCompressorDuckingTestSynth() 
	{
		m_pNativeSynth = new CompressorDuckingTestSynth();
	}
};