#pragma once
#include "plight/include/common/string.h"

#include <fstream>


namespace Plight
{
    class String;
}

namespace Plight::Sound
{
    /*
        IO Object for loading sound data into buffers step by step
    */
    class SoundBufferIO
    {
    public:

                                SoundBufferIO(String const&,
                                              size_t);

        void                    setLoopEnabled(bool);
        bool                    isLoopEnabled() const;

        bool                    hasReachedEnd() const;

        int32_t                 getSampleRate() const;

        std::vector<int16_t>    readNextBuffer();

    private:

        void                    initialize();

        template<typename Type>
        static Type             readNextElement(std::ifstream&);

        // Full path of the sound file
        String                  m_filePath;

        // Size of the buffers in bytes
        size_t                  m_bufferByteSize;

        // Stream position of the data chunk in the sound file
        std::streampos          m_dataChunkPosition;

        // Current byte position in the data chunk
        size_t                  m_currentPositionInDataChunk = 0;

        // Flag if looping is enabled
        bool                    m_loopEnabled = false;

        // Flag if the stream has reached the end of the data chunk
        bool                    m_reachedEnd = false;

        // Wave file RIFF header chunk data
        int32_t                 m_riffHeaderChunkId = 0;
        int32_t                 m_riffHeaderChunkSize = 0;
        int32_t                 m_format = 0;

        // Wave file format chunk data
        int32_t                 m_formatChunkId = 0;
        int32_t                 m_formatChunkSize = 0;
        int16_t                 m_audioFormat = 0;
        int16_t                 m_channelCount = 0;
        int32_t                 m_sampleRate = 0;
        int32_t                 m_byteRate = 0;
        int16_t                 m_bytesPerBlock = 0;
        int16_t                 m_bitDepth = 0;

        // Wave file data chunk data
        int32_t                 m_dataChunkId = 0;
        size_t                  m_dataChunkSize = 0;

    };
}
