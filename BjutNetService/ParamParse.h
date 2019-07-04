#ifndef BNS_PARAMPARSE_H
#define BNS_PARAMPARSE_H

#include <string>
#include <map>

namespace bna{

/**
 * parameter parser.
*/
class ParamParse
{
public:
    ParamParse() = default;

    bool parse(int argc, char* argv[]);

    bool hasSeted(const std::string &key) const;

    std::string getValue(const std::string &key) const;

    void setValue(const std::string &key, const std::string &value);

    size_t getSize() const;

    const std::string &getOperator() const;

    const std::string &getHost() const;

    const std::string &getLastError() const;

private:
    std::map<std::string, std::string> m_mapArgv;
    std::string m_strOperator;
    std::string m_strHost;
    std::string m_strLastError;
};

inline bool ParamParse::hasSeted(const std::string &key) const
{
    return m_mapArgv.find(key) != m_mapArgv.cend();
}

inline std::string ParamParse::getValue(const std::string &key) const
{
    return hasSeted(key) ? m_mapArgv.at(key) : std::string();
}

inline void ParamParse::setValue(const std::string &key, const std::string &value)
{
    m_mapArgv[key] = value;
}

inline size_t ParamParse::getSize() const
{
    return m_mapArgv.size() + (m_strOperator.size() ? 1 : 0);
}

inline const std::string &ParamParse::getOperator() const
{
    return m_strOperator;
}

inline const std::string &ParamParse::getHost() const
{
    return m_strHost;
}

inline const std::string &ParamParse::getLastError() const
{
    return m_strLastError;
}

}
#endif // BNS_PARAMPARSE_H
