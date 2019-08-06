/*
 *  @file       midiCommonInterface.h
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
            
    class MidiCommonInterface
    {
    protected:
        int _runningStatus;
        bool _thruActivated;
        
    public:
		MidiCommonInterface()
        {
        }
        
    protected:
        void (*_noteOnCallback)(byte channel, byte note, byte velocity) = NULL;
        void (*_noteOffCallback)(byte channel, byte note, byte velocity) = NULL;
        void (*_afterTouchPolyCallback)(byte channel, byte note, byte velocity) = NULL;
        void (*_controlChangeCallback)(byte channel, byte, byte) = NULL;
        void (*_programChangeCallback)(byte channel, byte) = NULL;
        void (*_afterTouchChannelCallback)(byte channel, byte) = NULL;
        void (*_pitchBendCallback)(byte channel, int) = NULL;
        void (*_songPositionCallback)(unsigned short beats) = NULL;
        void (*_songSelectCallback)(byte songnumber) = NULL;
        void (*_tuneRequestCallback)(void) = NULL;
        void (*_timeCodeQuarterFrameCallback)(byte data) = NULL;
        void (*_sysExCallback)(const byte* array, uint16_t size) = NULL;
        void (*_clockCallback)(void) = NULL;
        void (*_startCallback)(void) = NULL;
        void (*_continueCallback)(void) = NULL;
        void (*_stopCallback)(void) = NULL;
        void (*_activeSensingCallback)(void) = NULL;
        void (*_resetCallback)(void) = NULL;
        
    public:
        // sending
        void sendNoteOn(DataByte note, DataByte velocity, Channel channel) {
            sendChannelMessage(MidiType::NoteOn, note, velocity, channel);
        }
        
        void sendNoteOff(DataByte note, DataByte velocity, Channel channel) {
            sendChannelMessage(MidiType::NoteOff, note, velocity, channel);
        }
        
        void sendProgramChange(DataByte number, Channel channel) {
            sendChannelMessage(MidiType::ProgramChange, number, 0, channel);
        }
        
        void sendControlChange(DataByte number, DataByte value, Channel channel) {
            sendChannelMessage(MidiType::ControlChange, number, value, channel);
        }
        
        void sendPitchBend(int value, Channel channel) {
            const unsigned bend = unsigned(value - int(MIDI_PITCHBEND_MIN));
            sendChannelMessage(MidiType::PitchBend, (bend & 0x7f), (bend >> 7) & 0x7f, channel);
        }
        
        void sendPitchBend(double pitchValue, Channel channel) {
            const int scale = pitchValue > 0.0 ? MIDI_PITCHBEND_MAX : MIDI_PITCHBEND_MIN;
            const int value = int(pitchValue * double(scale));
            sendPitchBend(value, channel);
        }
        
        void sendPolyPressure(DataByte note, DataByte pressure, Channel channel) {
            sendChannelMessage(MidiType::AfterTouchPoly, note, pressure, channel);
        }
        
        void sendAfterTouch(DataByte pressure, Channel channel) {
            sendChannelMessage(MidiType::AfterTouchChannel, pressure, 0, channel);
        }
        
        void sendAfterTouch(DataByte note, DataByte pressure, Channel channel) {
            sendChannelMessage(MidiType::AfterTouchChannel, note, pressure, channel);
        }
        
        void sendSysEx(const byte*, uint16_t inLength) {
            // TODO
        }
        
        void sendTimeCodeQuarterFrame(DataByte typeNibble, DataByte valuesNibble) {
            const byte data = byte((((typeNibble & 0x07) << 4) | (valuesNibble & 0x0f)));
            sendTimeCodeQuarterFrame(data);
        }
        
        void sendTimeCodeQuarterFrame(DataByte data) {
            sendSystemCommonMessage(MidiType::TimeCodeQuarterFrame, data);
        }
        
        void sendSongPosition(unsigned short beats) {
            byte data1 = beats & 0x7f;
            byte data2 = (beats >> 7) & 0x7f;
            
            sendSystemCommonMessage(MidiType::SongPosition, data1, data2);
        }
        
        void sendSongSelect(DataByte number) {
            sendSystemCommonMessage(MidiType::SongSelect, number & 0x7f);
        }
        
        void sendTuneRequest() {
            sendSystemCommonMessage(MidiType::TuneRequest);
        }
        
        void sendActiveSensing() {
			sendRealTimeMessage(MidiType::ActiveSensing);
        }    
        
        void sendStart() {
            sendRealTimeMessage(MidiType::Start);
        }
        
        void sendContinue() {
            sendRealTimeMessage(MidiType::Continue);
        }
        
        void sendStop() {
            sendRealTimeMessage(MidiType::Stop);
        }
        
        void sendClock() {
            sendRealTimeMessage(MidiType::Clock);
        }
        
        void sendTick() {
            sendRealTimeMessage(MidiType::Tick);
        }
        
        void sendReset() {
            sendRealTimeMessage(MidiType::SystemReset);
        }
        
        // callbacks for receiving events

        void setHandleNoteOff(void (*fptr)(byte channel, byte note, byte velocity)) {
            _noteOffCallback = fptr;
        }
        void setHandleNoteOn(void (*fptr)(byte channel, byte note, byte velocity)) {
            _noteOnCallback = fptr;
        }
        void setHandleAfterTouchPoly(void (*fptr)(byte channel, byte note, byte pressure)) {
            _afterTouchPolyCallback = fptr;
        }
        void setHandleControlChange(void (*fptr)(byte channel, byte number, byte value)) {
            _controlChangeCallback = fptr;
        }
        void setHandleProgramChange(void (*fptr)(byte channel, byte number)) {
            _programChangeCallback = fptr;
        }
        void setHandleAfterTouchChannel(void (*fptr)(byte channel, byte pressure)) {
            _afterTouchChannelCallback = fptr;
        }
        void setHandlePitchBend(void (*fptr)(byte channel, int bend)) {
            _pitchBendCallback = fptr;
        }
        void setHandleSysEx(void (*fptr)(const byte * data, uint16_t size)) {
            _sysExCallback = fptr;
        }
        void setHandleTimeCodeQuarterFrame(void (*fptr)(byte data)) {
            _timeCodeQuarterFrameCallback = fptr;
        }
        void setHandleSongPosition(void (*fptr)(unsigned short beats)) {
            _songPositionCallback = fptr;
        }
        void setHandleSongSelect(void (*fptr)(byte songnumber)) {
            _songSelectCallback = fptr;
        }
        void setHandleTuneRequest(void (*fptr)(void)) {
            _tuneRequestCallback = fptr;
        }
        void setHandleClock(void (*fptr)(void)) {
            _clockCallback = fptr;
        }
        void setHandleStart(void (*fptr)(void)) {
            _startCallback = fptr;
        }
        void setHandleContinue(void (*fptr)(void)) {
            _continueCallback = fptr;
        }
        void setHandleStop(void (*fptr)(void)) {
            _stopCallback = fptr;
        }
        void setHandleActiveSensing(void (*fptr)(void)) {
            _activeSensingCallback = fptr;
        }
        void setHandleReset(void (*fptr)(void)) {
            _resetCallback = fptr;
        }
        
    protected:
        
        // Channel messages
        virtual void sendChannelMessage(MidiType type, DataByte data1, DataByte data2, Channel channel)
        {
            // Then test if channel is valid
            if (channel >= MIDI_CHANNEL_OFF  ||
                channel == MIDI_CHANNEL_OMNI ||
                type < 0x80)
            {
				return; // Don't send anything
            }
            
            if (type <= MidiType::PitchBend)
            {
                // Channel messages
                
                // Protection: remove MSBs on data
                data1 &= 0x7f;
                data2 &= 0x7f;
                
                const StatusByte status = getStatus(type, channel);
                
                if (type != MidiType::ProgramChange && type != MidiType::AfterTouchChannel)
                {
                    write(status, data1, data2);
                }
                else
                {
                    write(status, data1);
                }
            }
            else if (type >= MidiType::Clock && type <= MidiType::SystemReset)
            {
                sendRealTimeMessage(type); // System Real-time and 1 byte.
            }
        }
        
        // SystemCommon message
        virtual void sendSystemCommonMessage(MidiType type, DataByte data1 = 0, DataByte data2 = 0)
        {
            
        }
        
        // RealTime messages
        virtual void sendRealTimeMessage(MidiType type)
        {
            // Do not invalidate Running Status for real-time messages
            // as they can be interleaved within any message.
            
            switch (type)
            {
                case Clock:
                case Start:
                case Stop:
                case Continue:
                case ActiveSensing:
                case SystemReset:
                    write(type);
                    break;
                default:
                    // Invalid Real Time marker
                    break;
            }
        }
        
		// Abstract ----- Must be overwritten -----------------------------------
        
        // Read
        virtual void read() = 0;
        
        // Write
        virtual void write(DataByte) = 0;
        virtual void write(DataByte, DataByte) = 0;
        virtual void write(DataByte, DataByte, DataByte) = 0;
    };
}

