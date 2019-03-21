/*
 * E-Lib
 * Copyright (C) 2019 EnAccess
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <cstdint>

namespace EnAccess {

/*!
 * \class CircularBuffer
 *
 * Implementation of a circular buffer.
 */

template <typename T, uint16_t BUFFER_SIZE>
class CircularBuffer
{
  public:
    CircularBuffer() :
        _writeHead(0),
        _readHead(0),
        _availableData(0),
        _buffer()
    { }

    virtual ~CircularBuffer()
    { }

    /*!
     * Push data into the buffer. Data is copied.
     * \param pointer to the data to be copied into the buffer
     * \param size number of elements in data
     */
    //TODO: Check if virtual is appropriate
    virtual uint16_t push(const T* data, uint16_t size) volatile
    {
        if (size > availableSpace())
            size = availableSpace();

        uint16_t writeCount = 0;

        while (writeCount < size) {
            _buffer[_writeHead] = data[writeCount++];
            incrementOrResetHead(_writeHead);
        }
        _availableData += writeCount;

        return writeCount;
    }

    /*!
     * Pushes one elementinto the buffer. This function
     * does not check for available space in the buffer.
     * If there is no available space, an existing element
     * will be overwritten.
     * \param data Element to push into the buffer
     */
    virtual void push(T data) volatile
    {
        _buffer[_writeHead] = data;
        incrementOrResetHead(_writeHead);
        if (_availableData < BUFFER_SIZE)
            _availableData++;
    }

    /*!
     * Pull data from the buffer
     * \param data Pointer where pulled data will be stored
     * \param size Maximum size to pull
     * \return Actual number of elements pulled from the buffer
     */
    virtual uint16_t pull(T* data, uint16_t size) volatile
    {
        if (size > availableData())
            size = availableData();

        uint16_t readCount = 0;

        while (readCount < size) {
            data[readCount++] = _buffer[_readHead];
            incrementOrResetHead(_readHead);
        }
        _availableData -= readCount;

        return readCount;
    }

    /*!
     * Pull a single element from the buffer. This function does not
     * check if the buffer is empty, in which case old data will
     * be returned.
     * \return The element pulled from the buffer
     */
    virtual T pull() volatile
    {
        T data = _buffer[_readHead];
        incrementOrResetHead(_readHead);
        if (_availableData > 0)
            _availableData--;

        return data;
    }

    /*!
     * Read a single element without removing it from the buffer.
     * This function does not check if the buffer is empty,
     * in which case old data will be returned.
     * \return The element read from the buffer
     */
    virtual T read() volatile
    {
        return _buffer[_readHead];
    }

    /*!
     * Empties the buffer by resetting all counters to zero.
     */
    virtual void flush() volatile
    {
        _writeHead = 0;
        _readHead = 0;
        _availableData = 0;
    }

    /*!
     * \return true if the buffer is empty, false if there is data in it
     */
    virtual bool isEmpty() const volatile
    {
        return _availableData == 0;
    }

    /*!
     * \return true if the buffer is full, false if there is still space
     */
    virtual bool isFull() const volatile
    {
        return _availableData == BUFFER_SIZE;
    }

    /*!
     * \return Number of available elements in the buffer
     */
    virtual uint16_t availableData() const volatile
    {
        return _availableData;
    }

    /*!
     * \return Number of available space in the buffer
     */
    virtual uint16_t availableSpace() const volatile
    {
        return BUFFER_SIZE - _availableData;
    }

    /*!
     * \return size of the buffer, which was specified at compile time
     */
    virtual uint16_t size() const volatile
    {
        return BUFFER_SIZE;
    }

  private:
    volatile uint16_t _writeHead;
    volatile uint16_t _readHead;
    volatile uint16_t _availableData;
    volatile T _buffer[BUFFER_SIZE];

    void incrementOrResetHead(volatile uint16_t& head) volatile
    {
        head++;
        if (head >= BUFFER_SIZE)
            head = 0;
    }
};

}

#endif
