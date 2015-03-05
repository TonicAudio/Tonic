//
//  AudioFileUtils.h
//  TonicLib
//
//  Created by Morgan Packard on 10/26/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#ifndef __TonicLib__AudioFileUtils__
#define __TonicLib__AudioFileUtils__

#define _CRT_SECURE_NO_WARNINGS

#include "SampleTable.h"

namespace Tonic {

  SampleTable loadAudioFile(string path, int numChannels = 2);

  namespace Tonic_ {


    /***************************************************/

    // WHOLESALE COPY/PORT OF STK'S FILEREAD CLASS
    // https://ccrma.stanford.edu/software/stk/

    /*! \class FileRead
    \brief STK audio file input class.

    This class provides input support for various
    audio file formats.  Multi-channel (>2)
    soundfiles are supported.  The file data is
    returned via an external StkFrames object
    passed to the read() function.  This class
    does not store its own copy of the file data,
    rather the data is read directly from disk.

    FileRead currently supports uncompressed WAV,
    AIFF/AIFC, SND (AU), MAT-file (Matlab), and
    STK RAW file formats.  Signed integer (8-,
    16-, 24-, and 32-bit) and floating-point (32- and
    64-bit) data types are supported.  Compressed
    data types are not supported.

    STK RAW files have no header and are assumed to
    contain a monophonic stream of 16-bit signed
    integers in big-endian byte order at a sample
    rate of 22050 Hz.  MAT-file data should be saved
    in an array with each data channel filling a
    matrix row.  The sample rate for MAT-files should
    be specified in a variable named "fs".  If no
    such variable is found, the sample rate is
    assumed to be 44100 Hz.

    by Perry R. Cook and Gary P. Scavone, 1995--2014.
    */
    /***************************************************/


    class TonicFrames;


    class FileRead
    {

    public:

      /* typedefs and declarations specific to stk.h taht we need in order to compile */

      typedef unsigned long FileReadFormat;
      static const FileReadFormat FILEREAD_SINT8;   /*!< -128 to +127 */
      static const FileReadFormat FILEREAD_SINT16;  /*!< -32768 to +32767 */
      static const FileReadFormat FILEREAD_SINT24;  /*!< Lower 3 bytes of 32-bit signed integer. */
      static const FileReadFormat FILEREAD_SINT32;  /*!< -2147483648 to +2147483647. */
      static const FileReadFormat FILEREAD_FLOAT32; /*!< Normalized between plus/minus 1.0. */
      static const FileReadFormat FILEREAD_FLOAT64; /*!< Normalized between plus/minus 1.0. */

      typedef unsigned short UINT16;
      typedef unsigned int UINT32;
      typedef signed short SINT16;
      typedef signed int SINT32;
      typedef float FLOAT32;
      typedef double FLOAT64;

      /* end typedefs and declarations */

      //! Default constructor.
      FileRead(void);

      //! Overloaded constructor that opens a file during instantiation.
      /*!
      An StkError will be thrown if the file is not found or its
      format is unknown or unsupported.  The optional arguments allow a
      headerless file type to be supported.  If \c typeRaw is false (the
      default), the subsequent parameters are ignored.
      */
      FileRead(std::string fileName, bool typeRaw = false, unsigned int nChannels = 1,
        FileReadFormat format = FILEREAD_SINT16, TonicFloat rate = 22050.0);

      //! Class destructor.
      ~FileRead(void);

      //! Open the specified file and determine its formatting.
      /*!
      An StkError will be thrown if the file is not found or its
      format is unknown or unsupported.  The optional arguments allow a
      headerless file type to be supported.  If \c typeRaw is false (the
      default), the subsequent parameters are ignored.
      */
      void open(std::string fileName, bool typeRaw = false, unsigned int nChannels = 1,
        FileReadFormat format = FILEREAD_SINT16, TonicFloat rate = 22050.0);

      //! If a file is open, close it.
      void close(void);

      //! Returns \e true if a file is currently open.
      bool isOpen(void);

      //! Return the file size in sample frames.
      unsigned long fileSize(void) const { return fileSize_; };

      //! Return the number of audio channels in the file.
      unsigned int channels(void) const { return channels_; };

      //! Return the data format of the file.
      FileReadFormat format(void) const { return dataType_; };

      //! Return the file sample rate in Hz.
      /*!
      WAV, SND, and AIF formatted files specify a sample rate in
      their headers.  By definition, STK RAW files have a sample rate of
      22050 Hz.  MAT-files are assumed to have a rate of 44100 Hz.
      */
      TonicFloat fileRate(void) const { return fileRate_; };

      //! Read sample frames from the file into an StkFrames object.
      /*!
      The number of sample frames to read will be determined from the
      number of frames of the StkFrames argument.  If this size is
      larger than the available data in the file (given the file size
      and starting frame index), the extra frames will be unaffected
      (the StkFrames object will not be resized).  Optional parameters
      are provided to specify the starting sample frame within the file
      (default = 0) and whether to normalize the data with respect to
      fixed-point limits (default = true).  An StkError will be thrown
      if a file error occurs or if the number of channels in the
      StkFrames argument is not equal to that in the file.
      */
      void read(Tonic::TonicFrames& buffer, unsigned long startFrame = 0, bool doNormalize = true);

    protected:

      //! Static method that byte-swaps a 16-bit data type.
      static void swap16(unsigned char *ptr);

      //! Static method that byte-swaps a 32-bit data type.
      static void swap32(unsigned char *ptr);

      //! Static method that byte-swaps a 64-bit data type.
      static void swap64(unsigned char *ptr);


      // Get STK RAW file information.
      bool getRawInfo(const char *fileName, unsigned int nChannels,
        FileReadFormat format, TonicFloat rate);

      // Get WAV file header information.
      bool getWavInfo(const char *fileName);

      // Get SND (AU) file header information.
      bool getSndInfo(const char *fileName);

      // Get AIFF file header information.
      bool getAifInfo(const char *fileName);

      // Get MAT-file header information.
      bool getMatInfo(const char *fileName);




      // Helper function for MAT-file parsing.
      bool findNextMatArray(SINT32 *chunkSize, SINT32 *rows, SINT32 *columns, SINT32 *nametype);

      FILE *fd_;
      bool byteswap_;
      bool wavFile_;
      unsigned long fileSize_;
      unsigned long dataOffset_;
      unsigned int channels_;
      FileReadFormat dataType_;
      TonicFloat fileRate_;
    };

  }

}

#undef  _CRT_SECURE_NO_WARNINGS

#endif /* defined(__TonicLib__AudioFileUtils__) */
