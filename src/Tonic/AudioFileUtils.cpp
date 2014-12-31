//
//  AudioFileUtils.cpp
//  TonicLib
//
//  Created by Morgan Packard on 10/26/13.
//  Copyright (c) 2013 Nick Donaldson. All rights reserved.
//

#include "AudioFileUtils.h"

// includes for FileRead class (copied from STK's fileread)
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <cmath>
#include <cstdio>
#include "TonicFrames.h"


#ifdef __APPLE__
#include <AudioToolbox/AudioToolbox.h>
#endif

namespace Tonic {


#ifdef __APPLE__

	void checkCAError(OSStatus error, const char *operation){
		if (error == noErr) return;
		char errorString[20];
		// See if it appears to be a 4-char-code
		*(UInt32 *)(errorString + 1) = CFSwapInt32HostToBig(error);
		if (isprint(errorString[1]) && isprint(errorString[2]) &&
			isprint(errorString[3]) && isprint(errorString[4])) {
			errorString[0] = errorString[5] = '\'';
			errorString[6] = '\0';
		}
		else {
			// No, format it as an integer
			sprintf(errorString, "%d", (int)error);
		}
		fprintf(stderr, "Error: %s (%s)\n", operation, errorString);
	}


	SampleTable loadAudioFile(string path, int numChannels){

		static const int BYTES_PER_SAMPLE = sizeof(TonicFloat);

		// Get the file handle
		ExtAudioFileRef inputFile;
		CFStringRef cfStringRef;
		cfStringRef = CFStringCreateWithCString(kCFAllocatorDefault, path.c_str(), kCFStringEncodingMacRoman);
		CFURLRef inputFileURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, cfStringRef, kCFURLPOSIXPathStyle, false);
		CFRelease(cfStringRef);

		checkCAError(ExtAudioFileOpenURL(inputFileURL, &inputFile), "ExtAudioFileOpenURL failed");
		CFRelease(inputFileURL);

		// Define the format for the data we want to extract from the audio file
		AudioStreamBasicDescription outputFormat;
		memset(&outputFormat, 0, sizeof(outputFormat));
		outputFormat.mSampleRate = 44100.0;
		outputFormat.mFormatID = kAudioFormatLinearPCM;
		outputFormat.mFormatFlags = kAudioFormatFlagIsFloat;
		outputFormat.mBytesPerPacket = BYTES_PER_SAMPLE * numChannels;
		outputFormat.mFramesPerPacket = 1;
		outputFormat.mBytesPerFrame = BYTES_PER_SAMPLE * numChannels;
		outputFormat.mChannelsPerFrame = numChannels;
		outputFormat.mBitsPerChannel = 32;
		OSStatus error = ExtAudioFileSetProperty(inputFile, kExtAudioFileProperty_ClientDataFormat, sizeof(AudioStreamBasicDescription), &outputFormat);
		checkCAError(error, "Error setting kExtAudioFileProperty_ClientDataFormat.");

		// Determine the length of the file, in frames
		SInt64 numFrames;
		UInt32 intSize = sizeof(SInt64);
		error = ExtAudioFileGetProperty(inputFile, kExtAudioFileProperty_FileLengthFrames, &intSize, &numFrames);
		checkCAError(error, "Error reading number of frames.");

		// change sampleTable numframes to long long
		SampleTable destinationTable = SampleTable((int)numFrames, numChannels);

		// wrap the destination buffer in an AudioBufferList
		AudioBufferList convertedData;
		convertedData.mNumberBuffers = 1;
		convertedData.mBuffers[0].mNumberChannels = outputFormat.mChannelsPerFrame;
		convertedData.mBuffers[0].mDataByteSize = (UInt32)destinationTable.size() * BYTES_PER_SAMPLE;
		convertedData.mBuffers[0].mData = destinationTable.dataPointer();

		UInt32 numFrames32 = (UInt32)numFrames;
		ExtAudioFileRead(inputFile, &numFrames32, &convertedData);

		ExtAudioFileDispose(inputFile);

		return destinationTable;

	}

#else

	SampleTable loadAudioFile(string path, int numChannels){
		Tonic::error("loadAudioFile is currently only implemented for Apple platforms.");
		return NULL;
	}


#endif


	namespace Tonic_ {

		const FileRead::FileReadFormat FileRead::FILEREAD_SINT8 = 0x1;
		const FileRead::FileReadFormat FileRead::FILEREAD_SINT16 = 0x2;
		const FileRead::FileReadFormat FileRead::FILEREAD_SINT24 = 0x4;
		const FileRead::FileReadFormat FileRead::FILEREAD_SINT32 = 0x8;
		const FileRead::FileReadFormat FileRead::FILEREAD_FLOAT32 = 0x10;
		const FileRead::FileReadFormat FileRead::FILEREAD_FLOAT64 = 0x20;

		FileRead::FileRead()
			: fd_(0), fileSize_(0), channels_(0), dataType_(0), fileRate_(0.0)
		{
		}

		FileRead::FileRead(std::string fileName, bool typeRaw, unsigned int nChannels,
			FileReadFormat format, TonicFloat rate)
			: fd_(0)
		{
			open(fileName, typeRaw, nChannels, format, rate);
		}

		FileRead :: ~FileRead()
		{
			if (fd_)
				fclose(fd_);
		}

		void FileRead::close(void)
		{
			if (fd_) fclose(fd_);
			fd_ = 0;
			wavFile_ = false;
			fileSize_ = 0;
			channels_ = 0;
			dataType_ = 0;
			fileRate_ = 0.0;
		}

		bool FileRead::isOpen(void)
		{
			if (fd_) return true;
			else return false;
		}

		void FileRead::open(std::string fileName, bool typeRaw, unsigned int nChannels,
			FileReadFormat format, TonicFloat rate)
		{
			// If another file is open, close it.
			close();

			// Try to open the file.
			fd_ = fopen(fileName.c_str(), "rb");
			if (!fd_) {
				Tonic::error("FileRead::open: could not open or find file (" + fileName + ")!");
			}

			// Attempt to determine file type from header (unless RAW).
			bool result = false;
			if (typeRaw)
				result = getRawInfo(fileName.c_str(), nChannels, format, rate);
			else {
				char header[12];
				if (fread(&header, 4, 3, fd_) != 3) goto error;
				if (!strncmp(header, "RIFF", 4) &&
					!strncmp(&header[8], "WAVE", 4))
					result = getWavInfo(fileName.c_str());
				else if (!strncmp(header, ".snd", 4))
					result = getSndInfo(fileName.c_str());
				else if (!strncmp(header, "FORM", 4) &&
					(!strncmp(&header[8], "AIFF", 4) || !strncmp(&header[8], "AIFC", 4)))
					result = getAifInfo(fileName.c_str());
				else {
					if (fseek(fd_, 126, SEEK_SET) == -1) goto error;
					if (fread(&header, 2, 1, fd_) != 1) goto error;
					if (!strncmp(header, "MI", 2) ||
						!strncmp(header, "IM", 2))
						result = getMatInfo(fileName.c_str());
					else {
						Tonic::error("FileRead::open: file(" + fileName + ") format unknown.");
					}
				}
			}

			// If here, we had a file type candidate but something else went wrong.
			if (result == false)
				Tonic::error("FileRead::open: unknown error");

			// Check for empty files.
			if (fileSize_ == 0) {
				Tonic::error("FileRead::open: file(" + fileName + ") data size is zero!");
			}

			return;

		error:
			Tonic::error("FileRead::open: error reading file(" + fileName + ")!");
		}

		bool FileRead::getRawInfo(const char *fileName, unsigned int nChannels, FileReadFormat format, TonicFloat rate)
		{
			// Use the system call "stat" to determine the file length.
			struct stat filestat;
			if (stat(fileName, &filestat) == -1) {
				Tonic::error("FileRead: Could not stat RAW file (" + string(fileName) + ").");
				return false;
			}
			if (nChannels == 0) {
				Tonic::error("FileRead: number of channels can't be 0 (" + string(fileName) + ").");
				return false;
			}

			// Rawwave files have no header and by default, are assumed to
			// contain a monophonic stream of 16-bit signed integers in
			// big-endian byte order at a sample rate of 22050 Hz.  However,
			// different parameters can be specified if desired.
			dataOffset_ = 0;
			channels_ = nChannels;
			dataType_ = format;
			fileRate_ = rate;
			int sampleBytes = 0;
			if (format == FILEREAD_SINT8) sampleBytes = 1;
			else if (format == FILEREAD_SINT16) sampleBytes = 2;
			else if (format == FILEREAD_SINT32 || format == FILEREAD_FLOAT32) sampleBytes = 4;
			else if (format == FILEREAD_FLOAT64) sampleBytes = 8;
			else {
				Tonic::error("FileRead: FileReadFormat  is invalid (" + string(fileName) + ").");
				return false;
			}

			fileSize_ = (long)filestat.st_size / sampleBytes / channels_;  // length in frames

			byteswap_ = false;
#ifdef __LITTLE_ENDIAN__
			byteswap_ = true;
#endif

			return true;
		}

		bool FileRead::getWavInfo(const char *fileName)
		{
			// Find "format" chunk ... it must come before the "data" chunk.
			char id[4];
			SINT32 chunkSize;
			if (fread(&id, 4, 1, fd_) != 1) goto error;
			while (strncmp(id, "fmt ", 4)) {
				if (fread(&chunkSize, 4, 1, fd_) != 1) goto error;
#ifndef __LITTLE_ENDIAN__
				swap32((unsigned char *)&chunkSize);
#endif
				if (fseek(fd_, chunkSize, SEEK_CUR) == -1) goto error;
				if (fread(&id, 4, 1, fd_) != 1) goto error;
			}

			// Check that the data is not compressed.
			unsigned short format_tag;
			if (fread(&chunkSize, 4, 1, fd_) != 1) goto error; // Read fmt chunk size.
			if (fread(&format_tag, 2, 1, fd_) != 1) goto error;
#ifndef __LITTLE_ENDIAN__
			swap16((unsigned char *)&format_tag);
			swap32((unsigned char *)&chunkSize);
#endif
			if (format_tag == 0xFFFE) { // WAVE_FORMAT_EXTENSIBLE
				dataOffset_ = ftell(fd_);
				if (fseek(fd_, 14, SEEK_CUR) == -1) goto error;
				unsigned short extSize;
				if (fread(&extSize, 2, 1, fd_) != 1) goto error;
#ifndef __LITTLE_ENDIAN__
				swap16((unsigned char *)&extSize);
#endif
				if (extSize == 0) goto error;
				if (fseek(fd_, 6, SEEK_CUR) == -1) goto error;
				if (fread(&format_tag, 2, 1, fd_) != 1) goto error;
#ifndef __LITTLE_ENDIAN__
				swap16((unsigned char *)&format_tag);
#endif
				if (fseek(fd_, dataOffset_, SEEK_SET) == -1) goto error;
			}
			if (format_tag != 1 && format_tag != 3) { // PCM = 1, FLOAT = 3
				std::stringstream ss;
				ss << format_tag;
				Tonic::error("FileRead: " + string(fileName) + " contains an unsupported data format type(" + ss.str() + ").");
				return false;
			}

			// Get number of channels from the header.
			SINT16 temp;
			if (fread(&temp, 2, 1, fd_) != 1) goto error;
#ifndef __LITTLE_ENDIAN__
			swap16((unsigned char *)&temp);
#endif
			channels_ = (unsigned int)temp;

			// Get file sample rate from the header.
			SINT32 srate;
			if (fread(&srate, 4, 1, fd_) != 1) goto error;
#ifndef __LITTLE_ENDIAN__
			swap32((unsigned char *)&srate);
#endif
			fileRate_ = (TonicFloat)srate;

			// Determine the data type.
			dataType_ = 0;
			if (fseek(fd_, 6, SEEK_CUR) == -1) goto error;   // Locate bits_per_sample info.
			if (fread(&temp, 2, 1, fd_) != 1) goto error;
#ifndef __LITTLE_ENDIAN__
			swap16((unsigned char *)&temp);
#endif
			if (format_tag == 1) {
				if (temp == 8)
					dataType_ = FILEREAD_SINT8;
				else if (temp == 16)
					dataType_ = FILEREAD_SINT16;
				else if (temp == 24)
					dataType_ = FILEREAD_SINT24;
				else if (temp == 32)
					dataType_ = FILEREAD_SINT32;
			}
			else if (format_tag == 3) {
				if (temp == 32)
					dataType_ = FILEREAD_FLOAT32;
				else if (temp == 64)
					dataType_ = FILEREAD_FLOAT64;
			}
			if (dataType_ == 0) {
				Tonic::error("FileRead: current bits per sample with data format are not supported (" + string(fileName) + ").");
				return false;
			}

			// Jump over any remaining part of the "fmt" chunk.
			if (fseek(fd_, chunkSize - 16, SEEK_CUR) == -1) goto error;

			// Find "data" chunk ... it must come after the "fmt" chunk.
			if (fread(&id, 4, 1, fd_) != 1) goto error;

			while (strncmp(id, "data", 4)) {
				if (fread(&chunkSize, 4, 1, fd_) != 1) goto error;
#ifndef __LITTLE_ENDIAN__
				swap32((unsigned char *)&chunkSize);
#endif
				chunkSize += chunkSize % 2; // chunk sizes must be even
				if (fseek(fd_, chunkSize, SEEK_CUR) == -1) goto error;
				if (fread(&id, 4, 1, fd_) != 1) goto error;
			}

			// Get length of data from the header.
			SINT32 bytes;
			if (fread(&bytes, 4, 1, fd_) != 1) goto error;
#ifndef __LITTLE_ENDIAN__
			swap32((unsigned char *)&bytes);
#endif
			fileSize_ = bytes / temp / channels_;  // sample frames
			fileSize_ *= 8;  // sample frames

			dataOffset_ = ftell(fd_);
			byteswap_ = false;
#ifndef __LITTLE_ENDIAN__
			byteswap_ = true;
#endif

			wavFile_ = true;
			return true;

		error:
			Tonic::error("FileRead: error reading WAV file (" + string(fileName) + ").");
			return false;
		}

		bool FileRead::getSndInfo(const char *fileName)
		{
			// Determine the data type.
			UINT32 format;
			if (fseek(fd_, 12, SEEK_SET) == -1) goto error;   // Locate format
			if (fread(&format, 4, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
			swap32((unsigned char *)&format);
#endif

			if (format == 2) dataType_ = FILEREAD_SINT8;
			else if (format == 3) dataType_ = FILEREAD_SINT16;
			else if (format == 4) dataType_ = FILEREAD_SINT24;
			else if (format == 5) dataType_ = FILEREAD_SINT32;
			else if (format == 6) dataType_ = FILEREAD_FLOAT32;
			else if (format == 7) dataType_ = FILEREAD_FLOAT64;
			else {
				Tonic::error("FileRead: data format in file " + string(fileName) + " is not supported.");
				return false;
			}

			// Get file sample rate from the header.
			UINT32 srate;
			if (fread(&srate, 4, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
			swap32((unsigned char *)&srate);
#endif
			fileRate_ = (TonicFloat)srate;

			// Get number of channels from the header.
			UINT32 chans;
			if (fread(&chans, 4, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
			swap32((unsigned char *)&chans);
#endif
			channels_ = chans;

			UINT32 offset;
			if (fseek(fd_, 4, SEEK_SET) == -1) goto error;
			if (fread(&offset, 4, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
			swap32((unsigned char *)&offset);
#endif
			dataOffset_ = offset;

			// Get length of data from the header.
			if (fread(&fileSize_, 4, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
			swap32((unsigned char *)&fileSize_);
#endif
			// Convert to sample frames.
			if (dataType_ == FILEREAD_SINT8)
				fileSize_ /= channels_;
			if (dataType_ == FILEREAD_SINT16)
				fileSize_ /= 2 * channels_;
			else if (dataType_ == FILEREAD_SINT24)
				fileSize_ /= 3 * channels_;
			else if (dataType_ == FILEREAD_SINT32 || dataType_ == FILEREAD_FLOAT32)
				fileSize_ /= 4 * channels_;
			else if (dataType_ == FILEREAD_FLOAT64)
				fileSize_ /= 8 * channels_;

			byteswap_ = false;
#ifdef __LITTLE_ENDIAN__
			byteswap_ = true;
#endif

			return true;

		error:
			Tonic::error("FileRead: Error reading SND file (" + string(fileName) + ").");
			return false;
		}

		bool FileRead::getAifInfo(const char *fileName)
		{
			bool aifc = false;
			char id[4];

			// Determine whether this is AIFF or AIFC.
			if (fseek(fd_, 8, SEEK_SET) == -1) goto error;
			if (fread(&id, 4, 1, fd_) != 1) goto error;
			if (!strncmp(id, "AIFC", 4)) aifc = true;

			// Find "common" chunk
			SINT32 chunkSize;
			if (fread(&id, 4, 1, fd_) != 1) goto error;
			while (strncmp(id, "COMM", 4)) {
				if (fread(&chunkSize, 4, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
				swap32((unsigned char *)&chunkSize);
#endif
				chunkSize += chunkSize % 2; // chunk sizes must be even
				if (fseek(fd_, chunkSize, SEEK_CUR) == -1) goto error;
				if (fread(&id, 4, 1, fd_) != 1) goto error;
			}

			// Get number of channels from the header.
			SINT16 temp;
			if (fseek(fd_, 4, SEEK_CUR) == -1) goto error; // Jump over chunk size
			if (fread(&temp, 2, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
			swap16((unsigned char *)&temp);
#endif
			channels_ = temp;

			// Get length of data from the header.
			SINT32 frames;
			if (fread(&frames, 4, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
			swap32((unsigned char *)&frames);
#endif
			fileSize_ = frames; // sample frames

			// Read the number of bits per sample.
			if (fread(&temp, 2, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
			swap16((unsigned char *)&temp);
#endif

			// Get file sample rate from the header.  For AIFF files, this value
			// is stored in a 10-byte, IEEE Standard 754 floating point number,
			// so we need to convert it first.
			unsigned char srate[10];
			unsigned char exp;
			unsigned long mantissa;
			unsigned long last;
			if (fread(&srate, 10, 1, fd_) != 1) goto error;
			mantissa = (unsigned long)*(unsigned long *)(srate + 2);
#ifdef __LITTLE_ENDIAN__
			swap32((unsigned char *)&mantissa);
#endif
			exp = 30 - *(srate + 1);
			last = 0;
			while (exp--) {
				last = mantissa;
				mantissa >>= 1;
			}
			if (last & 0x00000001) mantissa++;
			fileRate_ = (TonicFloat)mantissa;

			byteswap_ = false;
#ifdef __LITTLE_ENDIAN__
			byteswap_ = true;
#endif

			// Determine the data format.
			dataType_ = 0;
			if (aifc == false) {
				if (temp <= 8) dataType_ = FILEREAD_SINT8;
				else if (temp <= 16) dataType_ = FILEREAD_SINT16;
				else if (temp <= 24) dataType_ = FILEREAD_SINT24;
				else if (temp <= 32) dataType_ = FILEREAD_SINT32;
			}
			else {
				if (fread(&id, 4, 1, fd_) != 1) goto error;
				if (!strncmp(id, "sowt", 4)) { // uncompressed little-endian
					if (byteswap_ == false) byteswap_ = true;
					else byteswap_ = false;
				}
				if (!strncmp(id, "NONE", 4) || !strncmp(id, "sowt", 4)) {
					if (temp <= 8) dataType_ = FILEREAD_SINT8;
					else if (temp <= 16) dataType_ = FILEREAD_SINT16;
					else if (temp <= 24) dataType_ = FILEREAD_SINT24;
					else if (temp <= 32) dataType_ = FILEREAD_SINT32;
				}
				else if ((!strncmp(id, "fl32", 4) || !strncmp(id, "FL32", 4)) && temp == 32) dataType_ = FILEREAD_FLOAT32;
				else if ((!strncmp(id, "fl64", 4) || !strncmp(id, "FL64", 4)) && temp == 64) dataType_ = FILEREAD_FLOAT64;
			}
			if (dataType_ == 0) {
				Tonic::error("FileRead: AIFF/AIFC file (" + string(fileName) + ") has unsupported data type (" + id + ").");
				return false;
			}

			// Start at top to find data (SSND) chunk ... chunk order is undefined.
			if (fseek(fd_, 12, SEEK_SET) == -1) goto error;

			// Find data (SSND) chunk
			if (fread(&id, 4, 1, fd_) != 1) goto error;
			while (strncmp(id, "SSND", 4)) {
				if (fread(&chunkSize, 4, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
				swap32((unsigned char *)&chunkSize);
#endif
				chunkSize += chunkSize % 2; // chunk sizes must be even
				if (fseek(fd_, chunkSize, SEEK_CUR) == -1) goto error;
				if (fread(&id, 4, 1, fd_) != 1) goto error;
			}

			// Skip over chunk size, offset, and blocksize fields
			if (fseek(fd_, 12, SEEK_CUR) == -1) goto error;

			dataOffset_ = ftell(fd_);
			return true;

		error:
			Tonic::error("FileRead: Error reading AIFF file (" + string(fileName) + ").");
			return false;
		}

		bool FileRead::findNextMatArray(SINT32 *chunkSize, SINT32 *rows, SINT32 *columns, SINT32 *nametype)
		{
			// Look for the next data array element. The file pointer should be
			// at the data element type when this function is called.
			SINT32 datatype;
			*chunkSize = 0;
			do {
				if (fseek(fd_, *chunkSize, SEEK_CUR) == -1) return false;
				if (fread(&datatype, 4, 1, fd_) != 1) return false;
				if (byteswap_) swap32((unsigned char *)&datatype);
				if (fread(chunkSize, 4, 1, fd_) != 1) return false;
				if (byteswap_) swap32((unsigned char *)chunkSize);
			} while (datatype != 14);

			// Check dimension subelement size to make sure 2D
			if (fseek(fd_, 20, SEEK_CUR) == -1) return false;
			SINT32 size;
			if (fread(&size, 4, 1, fd_) != 1) return false;
			if (byteswap_) swap32((unsigned char *)&size);
			if (size != 8) return false;

			// Read dimensions data
			if (fread(rows, 4, 1, fd_) != 1) return false;
			if (byteswap_) swap32((unsigned char *)rows);
			if (fread(columns, 4, 1, fd_) != 1) return false;
			if (byteswap_) swap32((unsigned char *)columns);

			// Read array name subelement type
			if (fread(nametype, 4, 1, fd_) != 1) return false;
			if (byteswap_) swap32((unsigned char *)nametype);

			return true;
		}

		bool FileRead::getMatInfo(const char *fileName)
		{
			// MAT-file formatting information is available at:
			// http://www.mathworks.com/access/helpdesk/help/pdf_doc/matlab/matfile_format.pdf

			// Verify this is a version 5 MAT-file format.
			char head[5];
			if (fseek(fd_, 0, SEEK_SET) == -1) goto error;
			if (fread(&head, 4, 1, fd_) != 1) goto error;
			// If any of the first 4 characters of the header = 0, then this is
			// a Version 4 MAT-file.
			head[4] = '\0';
			if (strstr(head, "0")) {
				Tonic::error("FileRead: " + string(fileName) + " appears to be a Version 4 MAT-file, which is not currently supported.");
				return false;
			}

			// Determine the endian-ness of the file.
			char mi[2];
			byteswap_ = false;
			// Locate "M" and "I" characters in header.
			if (fseek(fd_, 126, SEEK_SET) == -1) goto error;
			if (fread(&mi, 2, 1, fd_) != 1) goto error;
#ifdef __LITTLE_ENDIAN__
			if (!strncmp(mi, "MI", 2))
				byteswap_ = true;
			else if (strncmp(mi, "IM", 2)) goto error;
#else
			if (!strncmp(mi, "IM", 2))
				byteswap_ = true;
			else if (strncmp(mi, "MI", 2)) goto error;
#endif

			// We are expecting a data element containing the audio data and an
			// optional data element containing the sample rate (with an array
			// name of "fs").  Both elements should be stored as a Matlab array
			// type (14).

			bool doneParsing, haveData, haveSampleRate;
			SINT32 chunkSize, rows, columns, nametype;
			long dataoffset;
			doneParsing = false;
			haveData = false;
			haveSampleRate = false;
			while (!doneParsing) {

				dataoffset = ftell(fd_); // save location in file
				if (findNextMatArray(&chunkSize, &rows, &columns, &nametype) == false) {
					// No more Matlab array type chunks found.
					if (!haveData) {
						Tonic::error("FileRead: No audio data found in MAT-file (" + string(fileName) + ").");
						return false;
					}
					else if (!haveSampleRate) {
						fileRate_ = 44100.0;
						Tonic::warning("FileRead: No sample rate found ... assuming 44100.0");
						return true;
					}
					else return true;
				}

				if (!haveSampleRate && rows == 1 && columns == 1) { // Parse for sample rate.

					SINT32 namesize = 4;
					if (nametype == 1) { // array name > 4 characters
						if (fread(&namesize, 4, 1, fd_) != 1) goto error;
						if (byteswap_) swap32((unsigned char *)&namesize);
						if (namesize != 2) goto tryagain; // expecting name = "fs"
						namesize = 8; // field must be padded to multiple of 8 bytes
					}
					char name[3]; name[2] = '\0';
					if (fread(&name, 2, 1, fd_) != 1) goto error;
					if (strncmp(name, "fs", 2)) goto tryagain;

					// Jump to real part data subelement, which is likely to be in a
					// small data format.
					if (fseek(fd_, namesize - 2, SEEK_CUR) == -1) goto error;
					UINT32 type;
					TonicFloat srate;
					if (fread(&type, 4, 1, fd_) != 1) goto error;
					if (byteswap_) swap32((unsigned char *)&type);
					if ((type & 0xffff0000) != 0) // small data format
						type = (type & 0x0000ffff);
					else
					if (fseek(fd_, 4, SEEK_CUR) == -1) goto error;
					if (type == 1) { // SINT8
						signed char rate;
						if (fread(&rate, 1, 1, fd_) != 1) goto error;
						srate = (TonicFloat)rate;
					}
					else if (type == 2) { // UINT8
						unsigned char rate;
						if (fread(&rate, 1, 1, fd_) != 1) goto error;
						srate = (TonicFloat)rate;
					}
					else if (type == 3) { // SINT16
						SINT16 rate;
						if (fread(&rate, 2, 1, fd_) != 1) goto error;
						if (byteswap_) swap16((unsigned char *)&rate);
						srate = (TonicFloat)rate;
					}
					else if (type == 4) { // UINT16
						UINT16 rate;
						if (fread(&rate, 2, 1, fd_) != 1) goto error;
						if (byteswap_) swap16((unsigned char *)&rate);
						srate = (TonicFloat)rate;
					}
					else if (type == 5) { // SINT32
						SINT32 rate;
						if (fread(&rate, 4, 1, fd_) != 1) goto error;
						if (byteswap_) swap32((unsigned char *)&rate);
						srate = (TonicFloat)rate;
					}
					else if (type == 6) { // UINT32
						UINT32 rate;
						if (fread(&rate, 4, 1, fd_) != 1) goto error;
						if (byteswap_) swap32((unsigned char *)&rate);
						srate = (TonicFloat)rate;
					}
					else if (type == 7) { // FLOAT32
						FLOAT32 rate;
						if (fread(&rate, 4, 1, fd_) != 1) goto error;
						if (byteswap_) swap32((unsigned char *)&rate);
						srate = (TonicFloat)rate;
					}
					else if (type == 9) { // FLOAT64
						FLOAT64 rate;
						if (fread(&rate, 8, 1, fd_) != 1) goto error;
						if (byteswap_) swap64((unsigned char *)&rate);
						srate = (TonicFloat)rate;
					}
					else
						goto tryagain;

					if (srate > 0) fileRate_ = srate;
					haveSampleRate = true;
				}
				else if (!haveData) { // Parse for data.

					// Assume channels = smaller of rows or columns.
					if (rows < columns) {
						channels_ = rows;
						fileSize_ = columns;
					}
					else {
						Tonic::error("FileRead: Transpose the MAT-file array so that audio channels fill matrix rows (not columns).");
						return false;
					}

					SINT32 namesize = 4;
					if (nametype == 1) { // array name > 4 characters
						if (fread(&namesize, 4, 1, fd_) != 1) goto error;
						if (byteswap_) swap32((unsigned char *)&namesize);
						namesize = (SINT32)ceil((float)namesize / 8);
						if (fseek(fd_, namesize * 8, SEEK_CUR) == -1) goto error;  // jump over array name
					}
					else {
						if (fseek(fd_, 4, SEEK_CUR) == -1) goto error;
					}

					// Now at real part data subelement
					SINT32 type;
					if (fread(&type, 4, 1, fd_) != 1) goto error;
					if (byteswap_) swap32((unsigned char *)&type);
					if (type == 1)  dataType_ = FILEREAD_SINT8;
					else if (type == 3) dataType_ = FILEREAD_SINT16;
					else if (type == 5) dataType_ = FILEREAD_SINT32;
					else if (type == 7) dataType_ = FILEREAD_FLOAT32;
					else if (type == 9) dataType_ = FILEREAD_FLOAT64;
					else {
						Tonic::error("FileRead: This MAT-file array data format  is not supported.");
						return false;
					}

					// Jump to the data.
					if (fseek(fd_, 4, SEEK_CUR) == -1) goto error;
					dataOffset_ = ftell(fd_);
					haveData = true;
				}

			tryagain:
				if (haveData && haveSampleRate) doneParsing = true;
				else // jump to end of data element and keep trying
				if (fseek(fd_, dataoffset + chunkSize + 8, SEEK_SET) == -1) goto error;
			}

			return true;

		error:
			Tonic::error("FileRead: Error reading MAT-file (" + string(fileName) + ") header.");
			return false;
		}

		void FileRead::read(Tonic::TonicFrames& buffer, unsigned long startFrame, bool doNormalize)
		{
			// Make sure we have an open file.
			if (fd_ == 0) {
				Tonic::error("FileRead::read: a file is not open!");
			}

			// Check the buffer size.
			unsigned long nFrames = buffer.frames();
			if (nFrames == 0) {
				Tonic::error("FileRead::read: TonicFrames buffer size is zero ... no data read!");
			}

			if (buffer.channels() != channels_) {
				Tonic::error("FileRead::read: TonicFrames argument has incompatible number of channels!");
			}

			if (startFrame >= fileSize_) {
				Tonic::error("FileRead::read: startFrame argument is greater than or equal to the file size!");
			}

			// Check for file end.
			if (startFrame + nFrames > fileSize_)
				nFrames = fileSize_ - startFrame;

			long i, nSamples = (long)(nFrames * channels_);
			unsigned long offset = startFrame * channels_;

			// Read samples into TonicFrames data buffer.
			if (dataType_ == FILEREAD_SINT16) {
				SINT16 *buf = (SINT16 *)&buffer[0];
				if (fseek(fd_, dataOffset_ + (offset * 2), SEEK_SET) == -1) goto error;
				if (fread(buf, nSamples * 2, 1, fd_) != 1) goto error;
				if (byteswap_) {
					SINT16 *ptr = buf;
					for (i = nSamples - 1; i >= 0; i--)
						swap16((unsigned char *)ptr++);
				}
				if (doNormalize) {
					TonicFloat gain = 1.0 / 32768.0;
					for (i = nSamples - 1; i >= 0; i--)
						buffer[i] = buf[i] * gain;
				}
				else {
					for (i = nSamples - 1; i >= 0; i--)
						buffer[i] = buf[i];
				}
			}
			else if (dataType_ == FILEREAD_SINT32) {
				SINT32 *buf = (SINT32 *)&buffer[0];
				if (fseek(fd_, dataOffset_ + (offset * 4), SEEK_SET) == -1) goto error;
				if (fread(buf, nSamples * 4, 1, fd_) != 1) goto error;
				if (byteswap_) {
					SINT32 *ptr = buf;
					for (i = nSamples - 1; i >= 0; i--)
						swap32((unsigned char *)ptr++);
				}
				if (doNormalize) {
					TonicFloat gain = 1.0 / 2147483648.0;
					for (i = nSamples - 1; i >= 0; i--)
						buffer[i] = buf[i] * gain;
				}
				else {
					for (i = nSamples - 1; i >= 0; i--)
						buffer[i] = buf[i];
				}
			}
			else if (dataType_ == FILEREAD_FLOAT32) {
				FLOAT32 *buf = (FLOAT32 *)&buffer[0];
				if (fseek(fd_, dataOffset_ + (offset * 4), SEEK_SET) == -1) goto error;
				if (fread(buf, nSamples * 4, 1, fd_) != 1) goto error;
				if (byteswap_) {
					FLOAT32 *ptr = buf;
					for (i = nSamples - 1; i >= 0; i--)
						swap32((unsigned char *)ptr++);
				}
				for (i = nSamples - 1; i >= 0; i--)
					buffer[i] = buf[i];
			}
			else if (dataType_ == FILEREAD_FLOAT64) {
				FLOAT64 *buf = (FLOAT64 *)&buffer[0];
				if (fseek(fd_, dataOffset_ + (offset * 8), SEEK_SET) == -1) goto error;
				if (fread(buf, nSamples * 8, 1, fd_) != 1) goto error;
				if (byteswap_) {
					FLOAT64 *ptr = buf;
					for (i = nSamples - 1; i >= 0; i--)
						swap64((unsigned char *)ptr++);
				}
				for (i = nSamples - 1; i >= 0; i--)
					buffer[i] = buf[i];
			}
			else if (dataType_ == FILEREAD_SINT8 && wavFile_) { // 8-bit WAV data is unsigned!
				unsigned char *buf = (unsigned char *)&buffer[0];
				if (fseek(fd_, dataOffset_ + offset, SEEK_SET) == -1) goto error;
				if (fread(buf, nSamples, 1, fd_) != 1) goto error;
				if (doNormalize) {
					TonicFloat gain = 1.0 / 128.0;
					for (i = nSamples - 1; i >= 0; i--)
						buffer[i] = (buf[i] - 128) * gain;
				}
				else {
					for (i = nSamples - 1; i >= 0; i--)
						buffer[i] = buf[i] - 128.0;
				}
			}
			else if (dataType_ == FILEREAD_SINT8) { // signed 8-bit data
				char *buf = (char *)&buffer[0];
				if (fseek(fd_, dataOffset_ + offset, SEEK_SET) == -1) goto error;
				if (fread(buf, nSamples, 1, fd_) != 1) goto error;
				if (doNormalize) {
					TonicFloat gain = 1.0 / 128.0;
					for (i = nSamples - 1; i >= 0; i--)
						buffer[i] = buf[i] * gain;
				}
				else {
					for (i = nSamples - 1; i >= 0; i--)
						buffer[i] = buf[i];
				}
			}
			else if (dataType_ == FILEREAD_SINT24) {
				// 24-bit values are harder to import efficiently since there is
				// no native 24-bit type.  The following routine works but is much
				// less efficient than that used for the other data types.
				SINT32 temp;
				unsigned char *ptr = (unsigned char *)&temp;
				TonicFloat gain = 1.0 / 2147483648.0;
				if (fseek(fd_, dataOffset_ + (offset * 3), SEEK_SET) == -1) goto error;
				for (i = 0; i < nSamples; i++) {
#ifdef __LITTLE_ENDIAN__
					if (byteswap_) {
						if (fread(ptr, 3, 1, fd_) != 1) goto error;
						temp &= 0x00ffffff;
						swap32((unsigned char *)ptr);
					}
					else {
						if (fread(ptr + 1, 3, 1, fd_) != 1) goto error;
						temp &= 0xffffff00;
					}
#else
					if (byteswap_) {
						if (fread(ptr + 1, 3, 1, fd_) != 1) goto error;
						temp &= 0xffffff00;
						swap32((unsigned char *)ptr);
					}
					else {
						if (fread(ptr, 3, 1, fd_) != 1) goto error;
						temp &= 0x00ffffff;
					}
#endif

					if (doNormalize) {
						buffer[i] = (TonicFloat)temp * gain; // "gain" also  includes 1 / 256 factor.
					}
					else
						buffer[i] = (TonicFloat)temp / 256;  // right shift without affecting the sign bit
				}
			}

			buffer.setDataRate(fileRate_);

			return;

		error:
			Tonic::error("FileRead: Error reading file data.");
		}



		void FileRead::swap16(unsigned char *ptr)
		{
			unsigned char val;

			// Swap 1st and 2nd bytes
			val = *(ptr);
			*(ptr) = *(ptr + 1);
			*(ptr + 1) = val;
		}

		void FileRead::swap32(unsigned char *ptr)
		{
			unsigned char val;

			// Swap 1st and 4th bytes
			val = *(ptr);
			*(ptr) = *(ptr + 3);
			*(ptr + 3) = val;

			//Swap 2nd and 3rd bytes
			ptr += 1;
			val = *(ptr);
			*(ptr) = *(ptr + 1);
			*(ptr + 1) = val;
		}

		void FileRead::swap64(unsigned char *ptr)
		{
			unsigned char val;

			// Swap 1st and 8th bytes
			val = *(ptr);
			*(ptr) = *(ptr + 7);
			*(ptr + 7) = val;

			// Swap 2nd and 7th bytes
			ptr += 1;
			val = *(ptr);
			*(ptr) = *(ptr + 5);
			*(ptr + 5) = val;

			// Swap 3rd and 6th bytes
			ptr += 1;
			val = *(ptr);
			*(ptr) = *(ptr + 3);
			*(ptr + 3) = val;

			// Swap 4th and 5th bytes
			ptr += 1;
			val = *(ptr);
			*(ptr) = *(ptr + 1);
			*(ptr + 1) = val;
		}
	} // end namespace Tonic_

}// end namespace Tonic
