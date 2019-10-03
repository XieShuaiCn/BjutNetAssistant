#ifndef BNA_GUI_UISETTING_H
#define BNA_GUI_UISETTING_H

#include "../BjutNetService/common.h"

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

    void setShowDetail(bool show);
    bool getShowDetail() const;

    void setShowLog(bool show);
    bool getShowLog() const;

private:
    FlowGraphType m_typeFlowGraph;
    bool m_bShowDetail;
    bool m_bShowLog;
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

inline void UISetting::setShowDetail(bool show)
{
    m_bShowDetail = show;
}
inline bool UISetting::getShowDetail() const
{
    return m_bShowDetail;
}

inline void UISetting::setShowLog(bool show)
{
    m_bShowLog = show;
}

inline bool UISetting::getShowLog() const
{
    return m_bShowLog;
}
}
}


#endif // BNA_GUI_UISETTING_H
