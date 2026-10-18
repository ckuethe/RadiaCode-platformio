/**********************************************************************************/
/* Copyright (c) 2025 Markus Geiger                                               */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef Decoders_h
#define Decoders_h

#include "RadiaCodeTypes.h"
#include "BytesBuffer.h"
#include <vector>

// Decode a single DATA_BUF record from the front of the buffer.
//
// On success *consumed is set to true and the return value is a heap-allocated
// DataItem (which may be nullptr for record types that carry no useful data but
// must still be skipped).  The caller owns and must delete any non-null return.
//
// If the buffer does not yet hold a complete record, or the record type is not
// recognized, *consumed is set to false, the buffer position is left unchanged
// and nullptr is returned so the caller can wait for more bytes or stop.
DataItem* decodeDataBufRecord(BytesBuffer& br, uint32_t base_time_sec, bool* consumed);

// Decode a whole DATA_BUF buffer into a list of DataItems (backwards-compatible
// wrapper around decodeDataBufRecord).
std::vector<DataItem*> decodeDataBuf(BytesBuffer& br, uint32_t base_time_sec);

// Decode spectrum data directly into an existing Spectrum object
void decodeSpectrum(BytesBuffer& br, uint8_t format_version, Spectrum& spectrum);

#endif
