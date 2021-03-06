#include "UISetting.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>

namespace bna {
namespace gui {

UISetting::UISetting()
{
    load();
}

UISetting::~UISetting()
{
    save();
}

UISetting& UISetting::I()
{
    static UISetting s_inst;
    return s_inst;
}

bool UISetting::load()
{
    // TODO: move Innerversion to BjutNetAssistant
#ifdef Q_OS_WIN
    QSettings gui_settings("HKEY_CURRENT_USER\\Software\\HrrcnTech\\BjutNetAssistant\\Board",
                           QSettings::NativeFormat);
#else
    QString conf_file = QDir::home().absoluteFilePath(".bjutnet/board.conf");
    QFileInfo conf_info(conf_file);
    if(!conf_info.exists()){
        QDir::home().mkpath(".bjutnet");
    }
    QSettings gui_settings(conf_file, QSettings::IniFormat);
#endif
    QVariant var;
    var = gui_settings.value("InnerVersion", 0);
    if(var.canConvert(QVariant::Int)){
        int innerver = var.toInt();
        m_bNewInstall = (innerver == 0);
        m_bNewUpdate = (innerver != BNA_INNER_VERSION);
    }
    else{
        m_bNewInstall = true;
        m_bNewUpdate = false;
    }
    var = gui_settings.value("ShowDetail", false);
    if(var.canConvert(QVariant::Bool)){
        m_bShowDetail = var.toBool();
    }
    var = gui_settings.value("ShowLog", false);
    if(var.canConvert(QVariant::Bool)){
        m_bShowLog = var.toBool();
    }
    var = gui_settings.value("FlowGraphType", 0);
    if(var.canConvert(QVariant::Int)){
        int v = var.toInt();
        if(v >= FLOW_GRAPH_DEFALUT && v < FLOW_GRAPH_TYPE_SIZE){
            m_typeFlowGraph = FlowGraphType(v);
        }
        else{
            m_typeFlowGraph = FLOW_GRAPH_DEFALUT;
        }
    }
    int nBjutWebSize = gui_settings.beginReadArray("BjutWebFrequency");
    for(int i = 0; i < nBjutWebSize; ++i){
        gui_settings.setArrayIndex(i);
        bool ok;
        int id =gui_settings.value("id").toInt(&ok);
        int freq = gui_settings.value("freq").toInt();
        if(ok){
            m_mapBjutWebFrequency.insert(id, freq);
        }
    }
    gui_settings.endArray();
    return true;
}

bool UISetting::save()
{
#ifdef Q_OS_WIN
    QSettings gui_settings("HKEY_CURRENT_USER\\Software\\HrrcnTech\\BjutNetAssistant\\Board",
                           QSettings::NativeFormat);
#else
    QString conf_file = QDir::home().absoluteFilePath(".bjutnet/board.conf");
    QFileInfo conf_info(conf_file);
    if(!conf_info.exists()){
        QDir::home().mkpath(".bjutnet");
    }
    QSettings gui_settings(conf_file, QSettings::IniFormat);
#endif
    if(m_bNewInstall || m_bNewUpdate){
        gui_settings.setValue("Version", BNA_VERSION);
        gui_settings.setValue("InnerVersion", BNA_INNER_VERSION);
    }
    gui_settings.setValue("ShowDetail", m_bShowDetail);
    gui_settings.setValue("ShowLog", m_bShowLog);
    gui_settings.setValue("FlowGraphType", int(m_typeFlowGraph));
    gui_settings.beginWriteArray("BjutWebFrequency", m_mapBjutWebFrequency.size());
    int nBjutWebIdx = 0;
    for(auto it = m_mapBjutWebFrequency.cbegin(), itend = m_mapBjutWebFrequency.cend();
        it != itend; ++it){
        gui_settings.setArrayIndex(nBjutWebIdx++);
        gui_settings.setValue("id", it.key());
        gui_settings.setValue("freq", it.value());
    }
    gui_settings.endArray();
    gui_settings.sync();
    return true;
}

}}
