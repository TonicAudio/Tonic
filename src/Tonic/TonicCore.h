//
//  TonicCore.h
//  Tonic
//
//  Created by Morgan Packard on 1/23/13.
//
//
// See LICENSE.txt for license and usage information.
//


#ifndef TONIC_TONICCORE_H
#define TONIC_TONICCORE_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <limits>
#include <cstring>
#include <cstdio>
#include <cmath>

extern "C" {
  #include <stdint.h>
}

// Uncomment or define in your build configuration to log debug messages and perform extra debug checks
// #define TONIC_DEBUG

// Determine if C++11 is available. If not, some synths cannot be used. (applies to oF demos, mostly)
#define TONIC_HAS_CPP_11 (__cplusplus > 199711L)

// Platform-specific macros and includes
#if defined (__APPLE__)

  #import <Accelerate/Accelerate.h>
  #define USE_APPLE_ACCELERATE

#endif

#if (defined (__APPLE__) || defined (__linux__))

  #include <pthread.h> 

  #define TONIC_MUTEX_T           pthread_mutex_t
  #define TONIC_MUTEX_INIT(x)     pthread_mutex_init(&x, NULL)
  #define TONIC_MUTEX_DESTROY(x)  pthread_mutex_destroy(&x)
  #define TONIC_MUTEX_LOCK(x)     pthread_mutex_lock(&x)
  #define TONIC_MUTEX_UNLOCK(x)   pthread_mutex_unlock(&x)

#elif (defined (_WIN32) || defined (__WIN32__))

  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  
  // Clear these macros to avoid interfering with ControlParameter::min and ::max
  #undef min
  #undef max

#if _MSC_VER < 1800
  // Windows' C90 <cmath> header does not define log2
  inline static float log2(float n) {
	return log(n) / log(2);
  }
#endif

  // Windows native mutexes
  #define TONIC_MUTEX_T CRITICAL_SECTION
  #define TONIC_MUTEX_INIT(x) InitializeCriticalSection(&x)
  #define TONIC_MUTEX_DESTROY(x) DeleteCriticalSection(&x)
  #define TONIC_MUTEX_LOCK(x) EnterCriticalSection(&x)
  #define TONIC_MUTEX_UNLOCK(x) LeaveCriticalSection(&x)

#endif

// --- Macro for enabling denormal rounding on audio thread ---

// TODO: Any other non-SSE platforms that allow denormals by default? ARM-based targets (iPhone, for example) do not.
#if (defined (__SSE__) || defined (_WIN32))
  #include <xmmintrin.h>
  #define  TONIC_ENABLE_DENORMAL_ROUNDING() _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON)
#else
  #define  TONIC_ENABLE_DENORMAL_ROUNDING()
#endif


using namespace std;

typedef uint32_t              TonicUInt32;
typedef int32_t               TonicInt32;
typedef float                 TonicFloat;

#ifndef PI
const TonicFloat PI           = 3.14159265358979f;
#endif

#ifndef TWO_PI
const TonicFloat TWO_PI       = 2.f * PI;
#endif

// channel indices
#define TONIC_LEFT            0
#define TONIC_RIGHT           1

// Causes 32nd bit in double to have fractional value 1 (decimal point on 32-bit word boundary)
// Allowing some efficient shortcuts for table lookup using power-of-two length tables
#define BIT32DECPT 1572864.0

//! Top-level namespace.
/*! Objects under the Tonic namespace are used to bulid synths and generator chains */
namespace Tonic {
  
  //! DSP-level namespace.
  /*! Objects under the Tonic_ namespace are internal DSP-level objects not intended for public usage */
  namespace Tonic_ {
    
    static TonicFloat sampleRate_ = 44100.f;
    
  }
  
  // -- Global Constants --
  
  //! Set the operational sample rate.
  /*! !!!: CHANGING WHILE RUNNING WILL RESULT IN UNDEFINED BEHAVIOR. MUST BE SET PRIOR TO OBJECT ALLOCATION. */
  static void setSampleRate(TonicFloat sampleRate){
    Tonic_::sampleRate_ = sampleRate;
  }
  
  //! Return sample rate
  static TonicFloat sampleRate(){
    return Tonic_::sampleRate_;
  };

  //! "Vector" size for audio processing. ControlGenerators update at this rate.
  /*! !!!: THIS VALUE SHOULD BE A POWER-OF-TWO WHICH IS LESS THAN THE HARDWARE BUFFER SIZE */
  static const unsigned int kSynthesisBlockSize = 64;
  
  // -- Global Types --
  
  //!For fast computation of int/fract using some bit-twiddlery
  /*! inspired by the pd implementation */
  union FastPhasor {
    double d;
    TonicInt32 i[2];
  };
  
  
  namespace Tonic_{
    
    //! Context which defines a particular synthesis graph
    
    /*! 
        Context passed down from root BufferFiller graph object to all sub-generators.
        synchronizes signal flow in cases when generator output is shared between multiple inputs
    */
    struct SynthesisContext_{
      
      //! Number of frames elapsed since context start
      // unsigned long will last 38+ days before overflow at 44.1 kHz
      unsigned long elapsedFrames;
      
      //! Elapsed time since context start
      double elapsedTime;
      
      //! If true, generators will be forced to compute fresh output
      // TODO: Not fully implmenented yet -- ND 2013/05/20
      bool forceNewOutput;
            
      SynthesisContext_() : elapsedFrames(0), elapsedTime(0), forceNewOutput(true){}
    
      void tick() {
        elapsedFrames += kSynthesisBlockSize;
        elapsedTime = (double)elapsedFrames/sampleRate();
        forceNewOutput = false;
      };
    
    };
    
  } // namespace Tonic_
  
  //! Dummy context for ticking things in-place.
  // Will always be at time 0, forceNewOutput == true
  static const Tonic_::SynthesisContext_ DummyContext;

#pragma mark - Utility Functions
  
  //-- Vector Math --
  inline static void vcopy( TonicFloat * dst, unsigned int dst_str, const TonicFloat * src, unsigned int src_str, unsigned int length ){
    for (unsigned int i=0; i<length; i++, dst += dst_str, src += src_str){
      *dst = *src;
    }
  }
  
  //-- Arithmetic --
  
  inline static TonicFloat max(TonicFloat a, TonicFloat b) {
    return (a > b ? a : b);
  }

  inline static TonicFloat min(TonicFloat a, TonicFloat b) {
    return (a < b ? a : b);
  }

  inline static TonicFloat clamp(TonicFloat val, TonicFloat min, TonicFloat max) {
    return (val < min ? min : (val > max ? max : val));
  }
  
  inline static TonicFloat map(TonicFloat val, TonicFloat inMin, TonicFloat inMax, TonicFloat outMin, TonicFloat outMax, bool clamp = true) {
    TonicFloat result = ((val - inMin)/(inMax - inMin)) * (outMax - outMin) + outMin;
    if (clamp){
      result = Tonic::clamp(result, min(outMin,outMax), max(outMin,outMax));
    }
    return result;
  }
  

  inline static TonicFloat lerp(TonicFloat x1, TonicFloat x2, TonicFloat f){
    return (x1 + f*(x2-x1));
  }
  
  inline TonicFloat sinc(float x)
  {
    TonicFloat pi_x;
    
    if(x == 0.0f)
      return 1.0f;
    else
    {
      pi_x = PI * x;
      return sinf(pi_x) / pi_x;
    }
  }

  
  inline bool isPowerOf2(unsigned int input, unsigned int * prevPo2){

    if (input == 0) return true;
    
    unsigned int po2 = 2;
    while (po2 < input){
      po2 *= 2;
    }
    
    if (prevPo2){
      
      unsigned int nextPo2 = po2 * 2;
      *prevPo2 = abs((int)input - (int)po2) < abs((int)input - (int)nextPo2) ? po2 : nextPo2;
    }
    
    return input == po2;
  }
  
  #define TONIC_LOG_MAP_BASEVAL -4
  
  //! Takes linear value 0-1, maps to logarithmic value (base logBase) scaled to min-max. Useful for making faders.
  inline static TonicFloat mapLinToLog(float linValue, float min, float max){
    float expValue = map(linValue, 0.f, 1.f, TONIC_LOG_MAP_BASEVAL, 0.f, true);
    return map(powf(10.f,expValue), 0.0001f, 1.0f, min, max, true);
  }
  
  //! Takes logarithmic value between min-max, maps to linear value 0-1. Useful for making faders.
  inline static TonicFloat mapLogToLin(float logValue, float min, float max){
    return map(log10f(map(logValue, min, max, 0.0001f, 1.f,true)), TONIC_LOG_MAP_BASEVAL, 0.f, 0.f, 1.f, true);
  }
  
  //-- Freq/MIDI --
  
  //! Midi note number to frequency in Hz
  inline static TonicFloat mtof(TonicFloat nn){
    return 440.0f * powf(2.0f, (nn-69.0f)/12.0f);
  }
  
  //! Frequency in Hz to midi note number
  inline static TonicFloat ftom(TonicFloat f){
    return 12.0f * (logf(f/440.0f)/logf(2.0f)) + 69.0f;
  }
  
  //-- Decibels --
  /*
    Using 0 dBFS as 1.0
  */
  inline static TonicFloat linTodB(TonicFloat lv){
    return 20.0f*log10f(max(0, lv));
  }
  
  inline static TonicFloat dBToLin(TonicFloat dBFS){
    return powf(10.f,(dBFS/20.0f));
  }
  
  // -- Misc --
  
  inline static TonicFloat randomSample(){
    return ((TonicFloat)rand()/RAND_MAX) * 2.0f - 1.0f;
  }
  
  static float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

  //! Tonic exception class
  // May want to implement custom exception behavior here, but for now, this is essentially a typedef
  class TonicException : public runtime_error
  {
    public:
    TonicException(string const& error) : runtime_error(error) {};

  };
  
  // -- Logging --
  
  static void error(string message, bool fatal = false){
    // maybe also log to console?
    printf("Tonic::error: %s\n", message.c_str() );
    if (fatal){
      throw TonicException(message);
    }
  }
  
  static void warning(string message){
    printf("Tonic::warning: %s\n", message.c_str());
  }
  
  static void debug(string message){
#ifdef TONIC_DEBUG
    printf("Tonic::debug: %s\n", message.c_str());
#endif
  }

  //! Dictionary helper class for registering objects by name. For correct usage, objects should be Smart Pointers.
  template<class T>
  class TonicDictionary {
    
    protected:
      
      typedef std::map<string, T> TonicDictionaryMap;
      TonicDictionaryMap dictionaryMap_;
      
    public:
      
      //! Add object to dictionary. Replaces old object if one exists.
      void insertObject(string name, T object){
        dictionaryMap_[name] = object;
      }
      
      bool containsObjectNamed(string name){
        typename TonicDictionaryMap::iterator it = dictionaryMap_.find(name);
        return it != dictionaryMap_.end();
      }
      
      //! Returns object with given name. Returns new object if no object has been set for name, does not insert it.
      T objectNamed(string name){
        T obj;
        typename TonicDictionaryMap::iterator it = dictionaryMap_.find(name);
        if (it != dictionaryMap_.end()){
          obj = it->second;
        }
        return obj;
      }
      
      //! Remove object for name
      void removeObjectNamed(string name){
        typename TonicDictionaryMap::iterator it = dictionaryMap_.find(name);
        if (it != dictionaryMap_.end()){
          dictionaryMap_.erase(it);
        }
      }
    
  };
  
  //! Reference counting smart pointer class template
  template<class T>
  class TonicSmartPointer {
    
    protected:
      
      T * obj;
      int * pcount;
      
    public:
      
      TonicSmartPointer() : obj(NULL), pcount(NULL) {}
      
      TonicSmartPointer(T * initObj) : obj(initObj) , pcount(initObj ? new int(1) : NULL) {}
      
      TonicSmartPointer(const TonicSmartPointer& r) : obj(r.obj), pcount(r.pcount){
        retain();
      }
      
      TonicSmartPointer& operator=(const TonicSmartPointer& r)
      {
        if(obj == r.obj) return *this;
        
        release();
        
        obj = r.obj;
        pcount = r.pcount;
        
        retain();
        
        return *this;
      }
      
      ~TonicSmartPointer(){
        release();
      }
      
      void retain(){
        if (pcount) (*pcount)++;
      }
      
      void release(){
        if(pcount && --(*pcount) == 0){
          delete obj;
          delete pcount;
          
          obj = NULL;
          pcount = NULL;
        }
      }
      
      bool operator==(const TonicSmartPointer& r){
        return obj == r.obj;
      }
    
  };

  
} // namespace Tonic


#endif
