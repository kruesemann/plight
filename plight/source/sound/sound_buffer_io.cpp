#include "plight/include/sound/sound_buffer_io.h"

#include <errno.h>


namespace Plight::Sound
{
    /*
        Type of wave file chunk
    */
    enum class EChunkType
    {
        RiffHeader = 0x46464952,
        Format = 0x020746d66,
        Data = 0x61746164
    };

    SoundBufferIO::SoundBufferIO(String const& rFilePath,
                                 size_t bufferByteSize)
        : m_filePath(rFilePath)
        , m_bufferByteSize(bufferByteSize)
    {
        initialize();
    }

    /*
        Open the file and read all relevant data except for the actual sound data
    */
    void
    SoundBufferIO::initialize()
    {
        if (m_bufferByteSize % sizeof(int16_t) != 0)
            throw std::exception(String("Sound error: Sound buffer size must be a multiple of '%'").arg(sizeof(int16_t)).c_str());

        std::ifstream fileStream(m_filePath, std::ios::in | std::ios::binary);
        if (!fileStream)
            throw std::exception(String("IO error: Unable to open file '%'").arg(m_filePath).c_str());

        bool reachedDataChunk = false;

        while (fileStream &&
               !reachedDataChunk)
        {
            auto const chunkId = readNextElement<int32_t>(fileStream);
            auto const chunkSize = readNextElement<int32_t>(fileStream);

            switch (static_cast<EChunkType>(chunkId))
            {
            case EChunkType::RiffHeader:
                m_riffHeaderChunkId = chunkId;
                m_riffHeaderChunkSize = chunkSize;
                m_format = readNextElement<int32_t>(fileStream);
                break;
            case EChunkType::Format:
                m_formatChunkId = chunkId;
                m_formatChunkSize = chunkSize;
                m_audioFormat = readNextElement<int16_t>(fileStream);
                m_channelCount = readNextElement<int16_t>(fileStream);
                m_sampleRate = readNextElement<int32_t>(fileStream);
                m_byteRate = readNextElement<int32_t>(fileStream);
                m_bytesPerBlock = readNextElement<int16_t>(fileStream);
                m_bitDepth = readNextElement<int16_t>(fileStream);

                if (chunkSize == 18)
                    fileStream.ignore(readNextElement<int16_t>(fileStream));

                break;
            case EChunkType::Data:
            {
                m_dataChunkId = chunkId;
                m_dataChunkSize = static_cast<size_t>(chunkSize);
                m_dataChunkPosition = fileStream.tellg();
                reachedDataChunk = true;
                break;
            }
            default:
                fileStream.ignore(chunkSize);
                break;
            }
        }

        if (!reachedDataChunk)
            throw std::exception(String("IO error: File '%' may be corrupted").arg(m_filePath).c_str());

        if (m_bitDepth != 16 ||
            m_channelCount != 2)
            throw std::exception("Sound error: Sound format not supported");

        fileStream.close();
    }

    /*
        Read an element of given type
    */
    template<typename Type>
    Type
    SoundBufferIO::readNextElement(std::ifstream& rFileStream)
    {
        Type result;
        rFileStream.read(reinterpret_cast<char*>(&result), sizeof(Type));
        return result;
    }

    void
    SoundBufferIO::setLoopEnabled(bool loopEnabled)
    {
        m_loopEnabled = loopEnabled;
    }

    bool
    SoundBufferIO::isLoopEnabled() const
    {
        return m_loopEnabled;
    }

    bool
    SoundBufferIO::hasReachedEnd() const
    {
        return m_reachedEnd;
    }

    int32_t
    SoundBufferIO::getSampleRate() const
    {
        return m_sampleRate;
    }

    /*
        Read the next chunk of data into a buffer
    */
    std::vector<int16_t>
    SoundBufferIO::readNextBuffer()
    {
        std::ifstream fileStream(m_filePath, std::ios::in | std::ios::binary);
        if (!fileStream)
            throw std::exception(String("IO error: Unable to open file '%'").arg(m_filePath).c_str());

        std::vector<int16_t> result(m_bufferByteSize / sizeof(int16_t), 0);
        m_reachedEnd = false;

        size_t bytesReadForNextBuffer = 0;
        fileStream.seekg(m_dataChunkPosition + std::streampos(m_currentPositionInDataChunk));

        do
        {
            fileStream.read(reinterpret_cast<char*>(&result[bytesReadForNextBuffer / sizeof(int16_t)]),
                            std::min(m_bufferByteSize - bytesReadForNextBuffer,
                                     m_dataChunkSize - m_currentPositionInDataChunk));
            auto const bytesRead = fileStream.gcount();
            bytesReadForNextBuffer += bytesRead;
            m_currentPositionInDataChunk += bytesRead;

            // If the end of the data chunk was reached reset to the data chunk start position
            if (fileStream.eof() ||
                m_currentPositionInDataChunk == m_dataChunkSize)
            {
                fileStream.clear();
                fileStream.seekg(m_dataChunkPosition);
                m_currentPositionInDataChunk = 0;
                m_reachedEnd = true;
            }

            if (fileStream.fail())
            {
                char errmsg[94];
                strerror_s(errmsg, 94, errno);
                throw std::exception(String("IO error: Error while reading file '%': %").arg(m_filePath).arg(errmsg).c_str());
            }
        } while (m_loopEnabled && bytesReadForNextBuffer < m_bufferByteSize);

        // Trim the buffer to the bytes that were actually read
        if (bytesReadForNextBuffer < m_bufferByteSize)
            result.resize(bytesReadForNextBuffer / sizeof(int16_t));

        return result;
    }
}
