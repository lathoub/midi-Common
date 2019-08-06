/*
 *  @file       midi_Defs.h
 *  Project     Arduino MIDI Library
 *  @brief      MIDI Library for the Arduino - Definitions
 *  @author
 *  @date
 *  @license    MIT - Copyright (c) 2019
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to dea
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

namespace Midi {
        
    /*! \brief Extract an enumerated MIDI type from a status byte
     */
    static MidiType getTypeFromStatusByte(byte status)
    {
        if ((status  < 0x80) ||
            (status == 0xf4) ||
            (status == 0xf5) ||
            (status == 0xf9) ||
            (status == 0xfD))
        {
            // Data bytes and undefined.
            return MidiType::InvalidType;
        }
        if (status < 0xf0)
        {
            // Channel message, remove channel nibble.
            return MidiType(status & 0xf0);
        }
        
        return MidiType(status);
    }
    
    /*! \brief Returns type + channel
     */
    static StatusByte getStatus(MidiType type, Channel channel)
    {
        return ( type & 0xf0) | ((channel - 1) & 0x0f);
    }
    
    /*! \brief Returns channel in the range 1-16
     */
    static Channel getChannelFromStatusByte(byte status)
    {
        return Channel((status & 0x0f) + 1);
    }
    
    /*! \brief check if channel is in the range 1-16
     */
    static bool isChannelMessage(MidiType type)
    {
        return (type == MidiType::NoteOff           ||
                type == MidiType::NoteOn            ||
                type == MidiType::ControlChange     ||
                type == MidiType::AfterTouchPoly    ||
                type == MidiType::AfterTouchChannel ||
                type == MidiType::PitchBend         ||
                type == MidiType::ProgramChange);
    }
}

