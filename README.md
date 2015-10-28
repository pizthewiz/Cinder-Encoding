
# Cinder-Encoding
Simple CinderBlock for [SLIP](https://en.m.wikipedia.org/wiki/Serial_Line_Internet_Protocol) and [COBS](https://en.m.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing) encode / decode.

### EXAMPLE
```C++
const size_t size = 12;
uint8_t data[size] = {
    0x45, 0x00, 0x00, 0x2C, 0x4C, 0x79, 0x00, 0x00, 0x40, 0x06, 0x4F, 0x37
};

// COBS using ci::BufferRef
auto buffer = Buffer::create(data, size);
auto encodedBuffer = COBSEncode(buffer);
auto decodedBuffer = COBSDecode(encodedBuffer);
if (decodedBuffer->getSize() != buffer->getSize()) {
    // 💀
}

// SLIP using raw uint8_t*
size_t maxEncodedSize = 2 * size + 2;
uint8_t encodedData[maxEncodedSize];
size_t encodedSize = SLIPEncode(data, size, encodedData);

size_t maxDecodedSize = encodedSize - 1;
uint8_t decodedData[maxDecodedSize];
size_t decodedSize = SLIPDecode(data, size, decodedData);
if (decodedSize != size) {
    // 💀
}
```

### GREETZ
- [Christopher Baker](http://christopherbaker.net)'s excellent [openFrameworks](http://openframeworks.cc) addon [ofxIO](https://github.com/bakercp/ofxIO)
