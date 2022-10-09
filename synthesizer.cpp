#include <fstream>
#include <cstdlib>

struct SMinimalWaveFileHeader {
    //main chunk
    unsigned char m_szChunkID[4];
	uint32_t m_nChunkSize;
	unsigned char m_szFormat[4];

    //format chunk
    unsigned char m_szSubChunk1ID[4];
	uint32_t m_nSubChunk1Size;
	uint16_t m_nAudioFormat;
	uint16_t m_nNumChannels;
	uint32_t m_nSampleRate;
	uint32_t m_nByteRate;
	uint16_t m_nBlockAlign;
	uint16_t m_nBitsPerSample;

    //data
    unsigned char m_szSubChunk2ID[4];
	uint32_t m_nSubChunk2Size;
};

bool WriteWaveFile(const char *szFileName, void *pData, uint32_t nDataSize, short nNumChannels, uint32_t nSampleRate, uint32_t nBitsPerSample)
    {
        //open header
        FILE* file = fopen(szFileName, "w+b");
        if(!file)
        {
            return false;
        }

        SMinimalWaveFileHeader waveHeader;

        //fill header
        memcpy(waveHeader.m_szChunkID,"RIFF",4);
	    waveHeader.m_nChunkSize = nDataSize + 36;
	    memcpy(waveHeader.m_szFormat,"WAVE",4);

        //format chunk
        memcpy(waveHeader.m_szSubChunk1ID,"fmt ",4);
	    waveHeader.m_nSubChunk1Size = 16;
	    waveHeader.m_nAudioFormat = 1;
	    waveHeader.m_nNumChannels = nNumChannels;
	    waveHeader.m_nSampleRate = nSampleRate;
	    waveHeader.m_nByteRate = nSampleRate * nNumChannels * nBitsPerSample / 8;
	    waveHeader.m_nBlockAlign = nNumChannels * nBitsPerSample / 8;
	    waveHeader.m_nBitsPerSample = nBitsPerSample;

        //write data
        memcpy(waveHeader.m_szSubChunk2ID,"data",4);
	    waveHeader.m_nSubChunk2Size = nDataSize;

        //write header
        fwrite(&waveHeader,sizeof(SMinimalWaveFileHeader),1,file);

        //write data
        fwrite(pData,nDataSize,1,file);

        //close file, return success
        fclose(file);
        return true;
    }

int main()
{
    int nSampleRate = 44100;
    int nNumSeconds = 4;
    int nNumChannels = 2;
    int nNumSamples = nSampleRate * nNumChannels * nNumSeconds;
    uint32_t *pData = new uint32_t[nNumSamples];
    uint32_t nValue1 = 20;
    uint32_t nValue2 = 0;
    for(int nIndex = 0; nIndex < nNumSamples; nIndex+=2)
    {
        nValue1 += nValue2;
        nValue2 = nValue1;
        
        pData[nIndex] = nValue1;
        pData[nIndex+1] = nValue1;
    }

    WriteWaveFile("fibbonacci.wav",pData,nNumSamples * sizeof(pData[0]),nNumChannels,nSampleRate,sizeof(pData[0])*8);
    delete[] pData;
    return 0;
}
