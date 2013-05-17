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

	public ref class CLRControlParameter
	{
	public:
		String^          displayName;
		CLRControlValue^ value;
		float            min;
		float            max;

		static CLRControlParameter^ wrap(ControlParameter nativeParam)
		{
			auto param = gcnew CLRControlParameter();
			param->displayName = gcnew String(nativeParam.getDisplayName().c_str());
			param->value = CLRControlValue::wrap(nativeParam.getValue());
			param->min = nativeParam.getMin();
			param->max = nativeParam.getMax();
			return param;
		}
	};

	public ref class CLRSynthWrapper abstract
	{
	public:
		List<CLRControlParameter^>^ GetParameters() 
		{
			auto nativeParams = getNativeSynth()->getParameters();
			auto params = gcnew List<CLRControlParameter^>();
			for (auto p : nativeParams)
			{
				params->Add(CLRControlParameter::wrap(p));
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