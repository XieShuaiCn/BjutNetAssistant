
#include "Setting.h"
#include "common.h"
#include <QDir>
#include <QFile>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QProcess>

namespace bna{
namespace core{

#define BNA_AUTOSTART_LINUX_FILENAME "BjutNetService.sh.desktop"
#define BNA_AUTOSTART_MAC_FILENAME "BjutNetService.plist"

bool Setting::setAutoRun(bool bAutoRun)
{
    try{
#ifdef Q_OS_WIN
        QSettings autoRun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                          QSettings::NativeFormat);
        if(bAutoRun)
        {
            autoRun.setValue(BNS_NAME,
                             QApplication::applicationFilePath().replace(QChar('/'), QChar('\\')));
        }
        else
        {
            autoRun.remove(BNS_NAME);
        }
        return autoRun.contains(BNS_NAME) == bAutoRun;
#elif defined(Q_OS_LINUX)
        QDir autoStart = QDir::home();
        if(autoStart.mkpath(".config/autostart")) {
            if(autoStart.cd(".config/autostart")) {
                QFileInfo startScriptNew(autoStart.absoluteFilePath(BNA_AUTOSTART_LINUX_FILENAME));
                if(bAutoRun){
                    // create desktop file in autostart folder
                    QFile startScript(QDir(g_strAppDirPath).absoluteFilePath(BNA_AUTOSTART_LINUX_FILENAME));
                    if(startScript.exists()){
                        if(startScriptNew.exists()){
                            autoStart.remove(startScriptNew.filePath());
                        }
                        if(!startScript.copy(startScriptNew.filePath())) {
                            g_debugTool.setInfo(startScript.errorString());
                            if(g_bAppDebug){
                                g_debugTool.writeInfo(DebugTool::STATUS_FAIL, startScript.errorString());
                            }
                            return false;
                        }
                        return true;
                    }
                }
                else{
                    if(startScriptNew.exists()){
                        return autoStart.remove(BNA_AUTOSTART_LINUX_FILENAME);
                    }
                }
            }
        }
#elif defined(Q_OS_MAC)
        QDir autoStart = QDir::home();
        if(autoStart.mkpath("Library/LaunchAgents")) {
            if(autoStart.cd("Library/LaunchAgents")) {
                QFile startScriptNew(autoStart.absoluteFilePath(BNA_AUTOSTART_MAC_FILENAME));
                if(bAutoRun){
                    // create desktop file in autostart folder
                    if(startScriptNew.exists()){
                        autoStart.remove(startScriptNew.fileName());
                    }
                    if(!startScriptNew.open(QFile::WriteOnly)){
                        g_debugTool.setInfo("Can not create script file.");
                        if(g_bAppDebug){
                            g_debugTool.writeInfo(DebugTool::STATUS_FAIL, "Can not open file:"+startScriptNew.fileName());
                        }
                        return false;
                    }
                    QString content(
                                  "<?xml version=\"1.0\"encoding=\"utf-8\"?>\n"
                                  "<!DOCTYPE plist PUBLIC\"-//Apple//DTD PLIST 1.0//EN\"\n"
                                  "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
                                  "<plist version=\"1.0\">\n"
                                  "    <dict>\n"
                                  "       <key>KeepAlive</key>\n"
                                  "       <false/>\n"
                                  "       <key>RunAtLoad</key>\n"
                                  "       <true/>\n"
                                  "       <key>Label</key>\n"
                                  "       <string>com.hrrcn.xieshuai.BjutNetAssistant</string>\n"
                                  "        <key>ProgramArguments</key>\n"
                                  "        <array>\n"
                                  "            <string>"+QApplication::applicationFilePath()+"</string>\n"
                                  "        </array>\n"
                                  "        <key>WorkingDirectory</key>\n"
                                  "        <string>"+QApplication::applicationDirPath()+"</string>\n"
                                  "    </dict>\n"
                                  "</plist>"
                                );
                    startScriptNew.write(content.toUtf8());
                    startScriptNew.close();
                    QProcess procRegPlist;
                    QStringList procArg;
                    procArg.append("load");
                    procArg.append(startScriptNew.fileName());
                    procRegPlist.setProgram("launchctl");
                    procRegPlist.setArguments(procArg);
                    procRegPlist.start();
                    if(!procRegPlist.waitForFinished()){
                        g_debugTool.setInfo("Can not regist autostart.");
                        g_debugTool.setInfo(procRegPlist.errorString());
                        if(g_bAppDebug){
                            g_debugTool.writeInfo(DebugTool::STATUS_FAIL, "Can not regist autostart:"+startScriptNew.fileName());
                            g_debugTool.writeInfo(DebugTool::STATUS_INFO, procRegPlist.errorString());
                        }
                        return false;
                    }
                    //std::string cmd("launchctl load "+startScriptNew.fileName());
                    //system(cmd);
                    return procRegPlist.exitCode() == 0;
                }
                else{
                    if(startScriptNew.exists()){
                        QProcess procRegPlist;
                        QStringList procArg;
                        procArg.append("unload");
                        procArg.append(startScriptNew.fileName());
                        procRegPlist.setProgram("launchctl");
                        procRegPlist.start("launchctl", procArg);
                        if(!procRegPlist.waitForFinished()){
                            g_debugTool.setInfo("Can not unregist autostart.");
                            if(g_bAppDebug){
                                g_debugTool.writeInfo(DebugTool::STATUS_FAIL, "Can not unregist autostart:"+startScriptNew.fileName());
                            }
                            return false;
                        }
                    }
                    return true;
                }
            }
        }
#else
    (void)bAutoRun;
#endif
    }
    catch(...){ }
    return false;
}

bool Setting::getAutoRun()
{
    try{
#ifdef Q_OS_WIN
        QSettings autoRun("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                          QSettings::NativeFormat);
        return autoRun.contains(BNS_NAME);
#elif defined(Q_OS_LINUX)
        QDir autoStart = QDir::home();
        if(autoStart.cd(".config/autostart")) {
            return QFileInfo(autoStart.filePath(BNA_AUTOSTART_LINUX_FILENAME)).exists();
        }
#elif defined(Q_OS_MAC)
        QDir autoStart = QDir::home();
        if(autoStart.cd("Library/LaunchAgents")) {
            return QFileInfo(autoStart.filePath(BNA_AUTOSTART_MAC_FILENAME)).exists();
        }
#else

#endif
    }catch(...){ }
    return false;
}

#undef BNA_AUTOSTART_LINUX_FILENAME

}}
