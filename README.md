# WahlBit
Bit Parser for use in various algorithms requiring bit parsing including networking and compression

This library allows you to parse through bits like a C file reader. Simply give the BitParser your bytes, and you can traverse it by setting the current bit and byte.
For use in different algorithms, this library allows you getBits() and putBits() just like the original C functions. Bits are placed in the right most byte at all times. (Little endian)

For convenience in compression algorithms or searching for bit symbols, this library also includes a findBitString() method which sets the bitsLoc and bytesLoc values to just before the string's starting location.

