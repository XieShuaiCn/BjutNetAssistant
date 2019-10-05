#ifndef BNA_GUI_UISETTING_H
#define BNA_GUI_UISETTING_H

#include "../BjutNetService/common.h"
#include <QMap>

namespace bna {
namespace gui {

class UISetting
{
public:
    enum FlowGraphType{
        FLOW_GRAPH_DEFALUT=0,
        FLOW_GRAPH_PIE_2D,
        FLOW_GRAPH_PIE_3D,
        FLOW_GRAPH_TYPE_SIZE
    };
    UISetting();
    ~UISetting();

    static UISetting &getInstance();
    static UISetting &I();

    bool load();
    bool save();

    /////////////////// seter and geter //////////////////
    void setFlowGraphType(FlowGraphType type);
    void setFlowGraphType(int type);
    FlowGraphType getFlowGraphType() const;

    void setBjutWebFrequency(int id, int freq);
    const QMap<int, int> &getBjutWebFrequency() const;
    int getBjutWebFrequency(int id) const;

    PROPERTY_READ_WRITE(bool, ShowDetail, m_bShowDetail)
    PROPERTY_READ_WRITE(bool, ShowLog, m_bShowLog)
    PROPERTY_READ(bool, NewInstall, m_bNewInstall)
    PROPERTY_READ(bool, NewUpdate, m_bNewUpdate)

private:
    FlowGraphType m_typeFlowGraph;
    bool m_bShowDetail;
    bool m_bShowLog;
    QMap<int, int> m_mapBjutWebFrequency;
    bool m_bNewInstall;
    bool m_bNewUpdate;
};

////////////////////// inline implements /////////////////////////


inline UISetting &UISetting::getInstance()
{
    return I();
}

inline void UISetting::setFlowGraphType(UISetting::FlowGraphType type)
{
    if(type >= FLOW_GRAPH_DEFALUT && type < FLOW_GRAPH_TYPE_SIZE)
        m_typeFlowGraph = type;
}
inline void UISetting::setFlowGraphType(int type)
{
    if(type >= FLOW_GRAPH_DEFALUT && type < FLOW_GRAPH_TYPE_SIZE)
        m_typeFlowGraph = FlowGraphType(type);
}
inline UISetting::FlowGraphType UISetting::getFlowGraphType() const
{
    return m_typeFlowGraph;
}

inline void UISetting::setBjutWebFrequency(int id, int freq)
{
    m_mapBjutWebFrequency[id] = freq;
}

inline const QMap<int, int> &UISetting::getBjutWebFrequency() const
{
    return m_mapBjutWebFrequency;
}

inline int UISetting::getBjutWebFrequency(int id) const
{
    return m_mapBjutWebFrequency.value(id, 0);
}

}
}


#endif // BNA_GUI_UISETTING_H
