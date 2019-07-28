#ifndef BNA_MESSAGECODDER_H
#define BNA_MESSAGECODDER_H

namespace bna{

class MessageCoder
{
public:
    static int Encrypt(const char *words, int wlen, char *data, int dlen);
    static int Decrypt(const char *data, int dlen, char *words, int wlen);
};
}

#endif // BNA_MESSAGECODDER_H
