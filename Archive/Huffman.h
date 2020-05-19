//
// Created by ready on 17.05.2020.
//

#ifndef ARCHRONIS_HUFFMAN_H
#define ARCHRONIS_HUFFMAN_H

#include <string>
#include <limits>

#include "../Vector/Vector.h"
#include "../Tree/Tree.h"
#include "File.h"

class Huffman {
public:

    // ASCII chars byte
    typedef unsigned char BYTE;

    void Compress(
            Vector<File> &filesNames,
            std::fstream &archive);

    Vector<std::string> Decompress(
                  std::fstream &archive,
            const InfoFile     &infoFile);

    struct Letter {
        size_t frequency;
        BYTE letter;

        explicit Letter(
                size_t freq = 0,
                BYTE let    = '\0')

                :frequency(freq),
                 letter(let){}

        bool operator>(const Letter & L) const {
            return frequency > L.frequency;
        }
    };

    class HuffmanException: public std::exception {
    protected:
        std::string error_;

    public:
        explicit HuffmanException(const std::string &error)
                : error_(error) {}

        const char* what() const noexcept override {
            return error_.c_str();
        }
    };

    class HuffmanExceptionInvalidArchive: public HuffmanException {
    public:
        explicit HuffmanExceptionInvalidArchive(const std::string &error)
                : HuffmanException(error) {}
    };

protected:
    //Number of ASCII chars
    static const size_t ALPHABET_SIZE = 1 + std::numeric_limits<BYTE>::max();
    static const size_t BYTE_SIZE = 8;
    static const size_t  BYTE_COUNT = 1 << BYTE_SIZE;

    void UpdateLettersFreqsAndFileSize(
            File           &file,
            Vector<size_t> &lettersFreqs);

    const Tree<Letter> * BuildEncodeTree(
            const Vector<size_t> &lettersFreqs);

    const Tree<Huffman::Letter> * BuildDecodeTree(
            const Vector<BYTE>         &letters,
            const Vector<Vector<bool>> &table);

    void CountLengthsOfCodes(
            const Tree<Letter>::Node *root,
                  Vector<BYTE>       &codeLengths,
                  BYTE               level);

    BYTE FindMaxCodeLength(const Vector<BYTE> &codeLengths);

    void FillNumberOfCodesForLengths(
            const Vector<BYTE> &codeLengths,
                  Vector<BYTE> &numberOfCodes);

    void FillLettersByLength(
            const Vector<BYTE>         &codeLengths,
            const Vector<size_t>       &lettersFreqs,
                  Vector<Vector<BYTE>> &lettersByLength);

    void ComputeFirstCodesForLengths(
            const Vector<BYTE > &numberOfCodes,
                  Vector<BYTE > &firstCode);

    void BuildTable(
            const Vector<BYTE>         &numberOfCodes,
            const Vector<BYTE>         &firstCode,
            const Vector<Vector<BYTE>> &lettersByLength,
                  Vector<Vector<bool>> &table);

    void WriteFilesSizes(
            const Vector<File> &files,
                  std::fstream &out);

    void WriteEncodingInfo(
            const Vector<BYTE>          &numberOfCodes,
            const Vector<Vector<BYTE >> &lettersByLength,
            const Vector<File>          &files,
                  std::fstream          &out);

    Vector<Vector<Huffman::BYTE>> ReadDecodingInfo(
            Vector<BYTE>   &numberOfCodes,
            Vector<BYTE>   &letters,
            Vector<size_t> &sizes,
            std::fstream   &in);

    void WriteEncoding(
            const Vector<Vector<bool>> &table,
                  std::fstream         &in,
                  std::fstream         &out);

    void WriteDecoding (
                  std::fstream       &in,
                  std::fstream       &out,
            const Tree<Letter>::Node *root,
                  size_t             size);

    class Binary {
        static const size_t MAX_NUMB = ULONG_MAX;
        static const size_t BYTES_NUMB = 4;
    public:
        int WriteBinary(
                std::fstream  &out,
                size_t        size);

        size_t ReadBinary(std::fstream &in);
    };


};

void CheckReadingUnexpectedEndOfFile(
        const std::string  &funcName,
        const std::fstream &in);

#endif //ARCHRONIS_HUFFMAN_H
