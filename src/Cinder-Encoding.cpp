//
//  Cinder-Encoding.cpp
//  Cinder-Encoding
//
//  Created by Jean-Pierre Mouilleseaux on 28 Oct 2015.
//  Copyright © 2015 Chorded Constructions. All rights reserved.
//

#include "Cinder-Encoding.h"
#include "cinder/CinderMath.h"

using namespace ci;

namespace Cinder { namespace Encoding {

static const uint8_t SLIP_END = 0xC0;
static const uint8_t SLIP_ESC = 0xDB;
static const uint8_t SLIP_ESC_END = 0xDC;
static const uint8_t SLIP_ESC_ESC = 0xDD;

BufferRef SLIPEncode(const BufferRef& buffer) {
    size_t maxEncodedSize = 2 * buffer->getSize() + 2;
    BufferRef encodeBuffer = Buffer::create(maxEncodedSize);
    size_t encodedSize = SLIPEncode(reinterpret_cast<const uint8_t*>(buffer->getData()), buffer->getSize(), reinterpret_cast<uint8_t*>(encodeBuffer->getData()));
    encodeBuffer->resize(encodedSize);
    return encodeBuffer;
}

size_t SLIPEncode(const uint8_t* data, size_t size, uint8_t* encodedData) {
    size_t readIDX = 0, writeIDX = 0;

    // double-ENDed variant, will flush any accumulated line noise
    encodedData[writeIDX++] = SLIP_END;

    while (readIDX < size) {
        uint8_t value = data[readIDX++];

        if (value == SLIP_END) {
            encodedData[writeIDX++] = SLIP_ESC;
            encodedData[writeIDX++] = SLIP_ESC_END;
        } else if (value == SLIP_ESC) {
            encodedData[writeIDX++] = SLIP_ESC;
            encodedData[writeIDX++] = SLIP_ESC_ESC;
        } else {
            encodedData[writeIDX++] = value;
        }
    }
    encodedData[writeIDX++] = SLIP_END;

    return writeIDX;
}

BufferRef SLIPDecode(const BufferRef& buffer) {
    // should not assume double-ENDed variant
    size_t maxDecodedSize = buffer->getSize() - 1;
    BufferRef decodeBuffer = Buffer::create(maxDecodedSize);
    size_t decodedSize = SLIPDecode(reinterpret_cast<const uint8_t*>(buffer->getData()), buffer->getSize(), reinterpret_cast<uint8_t*>(decodeBuffer->getData()));
    decodeBuffer->resize(decodedSize);
    return decodeBuffer;
}

size_t SLIPDecode(const uint8_t* data, size_t size, uint8_t* decodedData) {
    size_t readIDX = 0, writeIDX = 0;

    while (readIDX < size) {
        uint8_t value = data[readIDX++];

        if (value == SLIP_END) {
            // flush or done
        } else if (value == SLIP_ESC) {
            value = data[readIDX++];
            if (value == SLIP_ESC_END) {
                decodedData[writeIDX++] = SLIP_END;
            } else if (value == SLIP_ESC_ESC) {
                decodedData[writeIDX++] = SLIP_ESC;
            } else {
                // protocol violation
            }
        } else {
            decodedData[writeIDX++] = value;
        }
    }

    return writeIDX;
}

BufferRef COBSEncode(const BufferRef& buffer) {
    size_t maxEncodedSize = buffer->getSize() + ceil(static_cast<float>(buffer->getSize()) / 254) + 1;
    BufferRef encodeBuffer = Buffer::create(maxEncodedSize);
    size_t encodedSize = COBSEncode(reinterpret_cast<const uint8_t*>(buffer->getData()), buffer->getSize(), reinterpret_cast<uint8_t*>(encodeBuffer->getData()));
    encodeBuffer->resize(encodedSize);
    return encodeBuffer;
}

size_t COBSEncode(const uint8_t* data, size_t size, uint8_t* encodedData) {
    size_t readIDX = 0, writeIDX = 1, codeIDX = 0;
    uint8_t code = 0x01;

    while (readIDX < size) {
        uint8_t value = data[readIDX++];

        if (value == 0) {
            encodedData[codeIDX] = code;
            codeIDX = writeIDX++;
            code = 0x01;
        } else {
            encodedData[writeIDX++] = value;
            if (++code == 0xFF) {
                encodedData[codeIDX] = code;
                codeIDX = writeIDX++;
                code = 0x01;
            }
        }
    }
    encodedData[codeIDX] = code;

    return writeIDX;
}

BufferRef COBSDecode(const BufferRef& buffer) {
    size_t maxDecodedSize = buffer->getSize() - 1;
    BufferRef decodeBuffer = Buffer::create(maxDecodedSize);
    size_t decodedSize = COBSDecode(reinterpret_cast<const uint8_t*>(buffer->getData()), buffer->getSize(), reinterpret_cast<uint8_t*>(decodeBuffer->getData()));
    decodeBuffer->resize(decodedSize);
    return decodeBuffer;
}

size_t COBSDecode(const uint8_t* data, size_t size, uint8_t* decodedData) {
    size_t readIDX = 0, writeIDX = 0;

    while (readIDX < size) {
        uint8_t code = data[readIDX++];

        for (uint8_t idx = 1; readIDX < size && idx < code; idx++) {
            decodedData[writeIDX++] = data[readIDX++];
        }
        if (code != 0xFF && readIDX != size) {
            decodedData[writeIDX++] = 0x00;
        }
    }

    return writeIDX;
}


}}
