//
//  Cinder-Encoding.h
//  Cinder-Encoding
//
//  Created by Jean-Pierre Mouilleseaux on 28 Oct 2015.
//  Copyright © 2015 Chorded Constructions. All rights reserved.
//

#pragma once

#include "cinder/Cinder.h"

namespace Cinder { namespace Encoding {

// Serial Line Internet Protocol: https://en.m.wikipedia.org/wiki/Serial_Line_Internet_Protocol
ci::BufferRef SLIPEncode(const ci::BufferRef& buffer);
// encodedData must have a minimum capacity of (2 * size + 2)
size_t SLIPEncode(const uint8_t* data, size_t size, uint8_t* encodedData);
ci::BufferRef SLIPDecode(const ci::BufferRef& buffer);
// decodedData must have a minimum capacity of (size - 1)
size_t SLIPDecode(const uint8_t* data, size_t size, uint8_t* decodedData);

// Consistent Overhead Byte Stuffing: https://en.m.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing
ci::BufferRef COBSEncode(const ci::BufferRef& buffer);
// encodedData must have a minimum capacity of (size + ceil(size / 254) + 1)
size_t COBSEncode(const uint8_t* data, size_t size, uint8_t* encodedData);
ci::BufferRef COBSDecode(const ci::BufferRef& buffer);
// decodedData must have a minimum capacity of (size - 1)
size_t COBSDecode(const uint8_t* data, size_t size, uint8_t* decodedData);

}}
