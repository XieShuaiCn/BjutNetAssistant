#include "ParamParse.h"

namespace bna{

bool ParamParse::parse(int argc, char* argv[])
{
    int idx = 1;
    if(argc > 1 && argv[1][0] != '-'){
        m_strOperator.assign(argv[1]);
        ++idx;
    }
    std::string lastArg;
    for(; idx < argc; ++idx)
    {
        if(argv[idx][0] == '-') //key or option, insert temporarily
        {
            std::string tmp(argv[idx]);
            lastArg = tmp.substr(1);
            m_mapArgv[lastArg] = std::string();
        }
        else //set value
        {
            if(lastArg.size() && m_mapArgv.find(lastArg) != m_mapArgv.cend()){
                m_mapArgv[lastArg] = std::string(argv[idx]);
                lastArg.clear();
            }else {
                m_strLastError.append("Error parameter: Unexpected '");
                m_strLastError.append(argv[idx]);
                m_strLastError.append("' after '");
                m_strLastError.append(argv[idx-1]);
                m_strLastError.append("'.");
                return false;
            }
        }
    }
    if(hasSeted("host")){
        m_strHost = m_mapArgv["host"];
        m_mapArgv.erase("host");
    }
    return true;
}

}
