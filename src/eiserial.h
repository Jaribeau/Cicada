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

#ifndef EISERIAL_H
#define EISERIAL_H

enum EConnectionStatus
{
    notConnected,
    connecting,
    connected,
    connectionError
};

class EISerial
{
public:
    virtual ~EISerial() { }

    /*!
     * Opens the serial device.
     * \return true if port was opened sucessfully, false otherwise
     */
    virtual bool open() = 0;

    /*!
     * Returns the opening state of the device.
     * \return true if the device is open, false otherwise.
     */
    virtual bool isOpen() = 0;

    /*!
     * Sets the serial device parameters.
     * \param baudRate One of the valid serial baud rates
     * \param dataBits Bit depth, usually 5, 6, 7, or 8
     * \return true if configuration could be applied sucessfully, false otherwise
     */
    virtual bool setSerialConfig(uint32_t baudRate, uint8_t dataBits) = 0;

    /*!
     * Closes the device
     */
    virtual void close() = 0;

protected:
    /*!
     * Number of bytes available for reading.
     * \return Number of bytes available for reading
     */
    virtual uint16_t rawBytesAvailable() const = 0;

    /*!
     * Reads data from the device.
     * \param data Buffer to store data. Must be large enough to store
     * maxSize bytes.
     * \param maxSize Maximum number of bytes to store into data. The actual
     * number of bytes stored can be smaller than maxSize.
     * \return Number of bytes actually copied to data
     */
    virtual uint16_t rawRead(uint8_t* data, uint16_t maxSize) = 0;

    /*!
     * Writes data to the device.
     * \param data Buffer with data written to the device
     * \param size Number of bytes to write
     * \return Actual number of bytes written
     */
    virtual uint16_t rawWrite(const uint8_t* data, uint16_t size) = 0;
};

#endif
