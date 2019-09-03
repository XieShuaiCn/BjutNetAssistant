#include "MessageCoder.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>

namespace bna {


int MessageCoder::Encrypt(const char *words, int wlen, char *data, int dlen)
{
    // empty data
    if(words == nullptr || wlen == 0){
        return 0;
    }
    if(wlen < 0){
        wlen = strlen(words);
    }
    // retrun th buffer size needed
    int nsize = wlen + 1;
    if(data == nullptr){
        return nsize;
    }
    // too small buffer size
    if(dlen < nsize){
        return 0;
    }
    // encrypt
    static const char chEncryptHash[256] = {
    '\xbc', '\xb9', '\x09', '\x91', '\xf8', '\x9f', '\x26', '\x03', '\xba', '\xef', '\x5f', '\x7f', '\x5d', '\x6b', '\xfe', '\x3f',
    '\x59', '\x90', '\x3d', '\x67', '\xc5', '\x25', '\x1b', '\xc9', '\x8d', '\x2f', '\x9c', '\x22', '\xca', '\x48', '\x1c', '\xf9',
    '\x60', '\x44', '\x0b', '\x12', '\x14', '\xe1', '\xf0', '\x05', '\xa8', '\x38', '\xe6', '\x64', '\x83', '\x6c', '\xb6', '\xb2',
    '\x0f', '\x75', '\x58', '\x2b', '\x71', '\x81', '\x33', '\x3e', '\x2e', '\x0a', '\xaa', '\xd2', '\x40', '\x46', '\xe2', '\xf5',
    '\x21', '\xad', '\x72', '\x7a', '\x97', '\x56', '\x30', '\x3c', '\x5a', '\x73', '\xb3', '\x0c', '\x4a', '\x87', '\x5b', '\xa4',
    '\x0e', '\x8e', '\x07', '\xd0', '\xbf', '\xc3', '\x89', '\xb4', '\x7b', '\x34', '\xec', '\x88', '\xea', '\xda', '\xce', '\xd6',
    '\x16', '\x76', '\xd4', '\x4b', '\x9b', '\x29', '\x51', '\x95', '\x94', '\xa5', '\xe9', '\xf1', '\x41', '\x02', '\x4d', '\xc6',
    '\xcb', '\x27', '\x42', '\xc7', '\x35', '\x17', '\x15', '\xf6', '\x00', '\x6a', '\xf4', '\x5c', '\xe7', '\x93', '\x7e', '\x3b',
    '\x06', '\xdc', '\x1a', '\xc8', '\xe8', '\xf7', '\xa0', '\x1f', '\x6f', '\x01', '\xd5', '\x8a', '\x36', '\x3a', '\xe0', '\x9d',
    '\x78', '\xae', '\x45', '\x5e', '\xf3', '\x23', '\x63', '\xbb', '\xe3', '\xd1', '\x4c', '\xac', '\xcd', '\x24', '\x54', '\x1d',
    '\x39', '\x31', '\x08', '\xe5', '\xb1', '\xfc', '\x53', '\x65', '\x4f', '\x49', '\x19', '\xde', '\xd3', '\xa9', '\x50', '\x62',
    '\x2d', '\x20', '\xf2', '\xab', '\x7d', '\x82', '\x52', '\xdb', '\xed', '\xa6', '\xd8', '\xb8', '\x0d', '\x84', '\xfa', '\xb5',
    '\x80', '\xeb', '\x13', '\x8f', '\x2c', '\x66', '\x74', '\x86', '\x9e', '\xee', '\xc4', '\x85', '\xc0', '\xb7', '\x69', '\xaf',
    '\x2a', '\xc1', '\x8c', '\x8b', '\x92', '\x9a', '\x55', '\x04', '\xa3', '\xa7', '\x98', '\x79', '\xdd', '\x10', '\xa2', '\x11',
    '\x70', '\xcc', '\x32', '\xc2', '\xcf', '\x28', '\xd9', '\xfb', '\xff', '\x6d', '\x37', '\x18', '\xbd', '\x7c', '\x99', '\x61',
    '\x6e', '\xbe', '\xa1', '\xdf', '\x57', '\xfd', '\x43', '\x96', '\x47', '\x77', '\xb0', '\x68', '\x4e', '\xd7', '\xe4', '\x1e'};

    char bias = rand() % 128;
    data[0] = bias;

    for(int i = 0; i < wlen; ++i){
        data[i+1] = static_cast<char>((
                    static_cast<int>(static_cast<unsigned char>(
                                          chEncryptHash[static_cast<unsigned char>(words[i])]))
                + bias) % 256);
    }
    //memcpy(data, words, sizeof(char)*wlen);

    return nsize;
}

int MessageCoder::Decrypt(const char *data, int dlen, char *words, int wlen)
{
    // empty data
    if(data == nullptr || dlen == 0){
        return 0;
    }
    if(dlen < 0){
        dlen = strlen(data);
    }
    // retrun th buffer size needed
    int nsize = dlen-1;
    if(words == nullptr){
        return nsize;
    }
    // too small buffer size
    if(wlen < nsize){
        return 0;
    }
    // decrypt
    static const char chDecryptHash[256] = {
    '\x78', '\x89', '\x6d', '\x07', '\xd7', '\x27', '\x80', '\x52', '\xa2', '\x02', '\x39', '\x22', '\x4b', '\xbc', '\x50', '\x30',
    '\xdd', '\xdf', '\x23', '\xc2', '\x24', '\x76', '\x60', '\x75', '\xeb', '\xaa', '\x82', '\x16', '\x1e', '\x9f', '\xff', '\x87',
    '\xb1', '\x40', '\x1b', '\x95', '\x9d', '\x15', '\x06', '\x71', '\xe5', '\x65', '\xd0', '\x33', '\xc4', '\xb0', '\x38', '\x19',
    '\x46', '\xa1', '\xe2', '\x36', '\x59', '\x74', '\x8c', '\xea', '\x29', '\xa0', '\x8d', '\x7f', '\x47', '\x12', '\x37', '\x0f',
    '\x3c', '\x6c', '\x72', '\xf6', '\x21', '\x92', '\x3d', '\xf8', '\x1d', '\xa9', '\x4c', '\x63', '\x9a', '\x6e', '\xfc', '\xa8',
    '\xae', '\x66', '\xb6', '\xa6', '\x9e', '\xd6', '\x45', '\xf4', '\x32', '\x10', '\x48', '\x4e', '\x7b', '\x0c', '\x93', '\x0a',
    '\x20', '\xef', '\xaf', '\x96', '\x2b', '\xa7', '\xc5', '\x13', '\xfb', '\xce', '\x79', '\x0d', '\x2d', '\xe9', '\xf0', '\x88',
    '\xe0', '\x34', '\x42', '\x49', '\xc6', '\x31', '\x61', '\xf9', '\x90', '\xdb', '\x43', '\x58', '\xed', '\xb4', '\x7e', '\x0b',
    '\xc0', '\x35', '\xb5', '\x2c', '\xbd', '\xcb', '\xc7', '\x4d', '\x5b', '\x56', '\x8b', '\xd3', '\xd2', '\x18', '\x51', '\xc3',
    '\x11', '\x03', '\xd4', '\x7d', '\x68', '\x67', '\xf7', '\x44', '\xda', '\xee', '\xd5', '\x64', '\x1a', '\x8f', '\xc8', '\x05',
    '\x86', '\xf2', '\xde', '\xd8', '\x4f', '\x69', '\xb9', '\xd9', '\x28', '\xad', '\x3a', '\xb3', '\x9b', '\x41', '\x91', '\xcf',
    '\xfa', '\xa4', '\x2f', '\x4a', '\x57', '\xbf', '\x2e', '\xcd', '\xbb', '\x01', '\x08', '\x97', '\x00', '\xec', '\xf1', '\x54',
    '\xcc', '\xd1', '\xe3', '\x55', '\xca', '\x14', '\x6f', '\x73', '\x83', '\x17', '\x1c', '\x70', '\xe1', '\x9c', '\x5e', '\xe4',
    '\x53', '\x99', '\x3b', '\xac', '\x62', '\x8a', '\x5f', '\xfd', '\xba', '\xe6', '\x5d', '\xb7', '\x81', '\xdc', '\xab', '\xf3',
    '\x8e', '\x25', '\x3e', '\x98', '\xfe', '\xa3', '\x2a', '\x7c', '\x84', '\x6a', '\x5c', '\xc1', '\x5a', '\xb8', '\xc9', '\x09',
    '\x26', '\x6b', '\xb2', '\x94', '\x7a', '\x3f', '\x77', '\x85', '\x04', '\x1f', '\xbe', '\xe7', '\xa5', '\xf5', '\x0e', '\xe8'};

    char bias = data[0];

    for(int i = 0; i < dlen; ++i){
        words[i] = chDecryptHash[
                (static_cast<int>(static_cast<unsigned char>(data[i+1])) + 256 - bias) % 256
                ];
    }
    //memcpy(words, data, sizeof(char)*dlen);

    return nsize;
}

}
