#include "WndTrayIcon.h"
#include <QMenu>
#include <QApplication>
#include <QLocalSocket>
#include <QDesktopServices>
#include <QMessageBox>
#include "BjutNet.h"
#include "WndMain.h"
#include "WndSetting.h"

namespace bna {

namespace gui {

WndTrayIcon::WndTrayIcon(QApplication *app,QObject *parent):
    QSystemTrayIcon(parent),
    m_app(app),
    m_wndMain(Q_NULLPTR),
    m_wndSetting(Q_NULLPTR)
{
    if(m_vecBjutWeb.empty()){
        int id = 1;
        m_vecBjutWeb.append({id++, QString("学校首页"), QUrl("http://www.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({id++, QString("综合服务门户"), QUrl("https://my.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({id++, QString("自助服务系统"), QUrl("https://jfself.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({id++, QString("软件正版化平台"), QUrl("http://ca.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({id++, QString("邮件系统"), QUrl("https://mail.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({id++, QString("网关登陆页面"), QUrl("http://lgn.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({id++, QString("研究生管理信息系统"), QUrl("http://webrecdoc.bjut.edu.cn/pyxx/"), 0});
        m_vecBjutWeb.append({id++, QString("财务信息门户"), QUrl("http://172.27.12.88/WFManager/homesso.jsp"), 0});
    }

    this->setToolTip("北工大校园网助手");
    QIcon ico(":/icon/logo.ico");
    this->setIcon(ico);

    m_menuTray = new QMenu("托盘菜单");
    this->setContextMenu(m_menuTray);
    //
    m_actMenuShowMain = new QAction("显示主窗口");
    m_actMenuRestartDamon = new QAction("重新启动服务");
    m_actMenuLogin = new QAction("上线");
    m_actMenuLogout = new QAction("下线");
    m_actMenuSetting = new QAction("设置");
    m_actMenuQuitAll = new QAction("完全退出");
    m_actMenuQuit = new QAction("退出");
    m_menuBjutWeb = new QMenu("常用网址");
    m_menuTray->addAction(m_actMenuShowMain);
    m_menuTray->addAction(m_actMenuRestartDamon);
    m_menuTray->addMenu(m_menuBjutWeb);
    m_menuTray->addSeparator();
    m_menuTray->addAction(m_actMenuLogin);
    m_menuTray->addAction(m_actMenuLogout);
    m_menuTray->addSeparator();
    m_menuTray->addAction(m_actMenuSetting);
    m_menuTray->addAction(m_actMenuQuitAll);
    m_menuTray->addAction(m_actMenuQuit);
    //initBjutWebMenu();
    //双击间隔200ms
    m_tmClick.setInterval(200);
    //一次触发
    m_tmClick.setSingleShot(true);

    connect(this, &WndTrayIcon::activated, this, &WndTrayIcon::on_actived);
    connect(m_actMenuShowMain, &QAction::triggered, this, &WndTrayIcon::cmdShowMainWnd);
    connect(m_actMenuRestartDamon, &QAction::triggered, this, &WndTrayIcon::cmdRestartDamon);
    connect(m_actMenuLogin, &QAction::triggered, this, &WndTrayIcon::cmdLoginLgn);
    connect(m_actMenuLogout, &QAction::triggered, this, &WndTrayIcon::cmdLogoutLgn);
    connect(m_actMenuSetting, &QAction::triggered, this, &WndTrayIcon::cmdShowSettingWnd);
    connect(m_actMenuQuit, &QAction::triggered, this, &WndTrayIcon::cmdExitApp);
    connect(m_actMenuQuitAll, &QAction::triggered, this, &WndTrayIcon::cmdExitAll);
    connect(&m_tmClick, &QTimer::timeout, this, &WndTrayIcon::on_clicked);
    connect(m_menuBjutWeb, &QMenu::aboutToShow, this, &WndTrayIcon::initBjutWebMenu);

    m_bjutnet = new BjutNet;
    Q_ASSERT(m_bjutnet!=nullptr);
    m_bjutnet->start();
}

WndTrayIcon::~WndTrayIcon()
{
    if(m_bjutnet)
    {
        if(m_bjutnet->isRunning()){
            m_bjutnet->stop();
            if(!m_bjutnet->wait(500)){ // ms
                m_bjutnet->terminate();
            }
        }
        m_bjutnet->deleteLater();
        m_bjutnet = Q_NULLPTR;
    }
    if(m_wndSetting)
    {
        m_wndSetting->close();
    }
    if(m_wndMain)
    {
        m_wndMain->close();
    }
    delete m_menuTray;
}

void WndTrayIcon::on_clicked()
{
    QString status;
    bool online;
    int time;
    int flow;
    int fee;
    const char* flowUnit[] = {"KB", "MB", "GB", "TB"};
    int flowUnitIndex = 0;
    m_bjutnet->getNetInfo(online, flow, time, fee);
    float ftime = float(time) / 60;
    float fflow = float(flow);
    while(fflow > 1024)
    {
        fflow /= 1024;
        ++flowUnitIndex;
    }
    float ffee = float(fee) / 100;
    status.sprintf("已用时间：%.2f小时，已用流量：%.2f%s，剩余金额：%.2f元", ftime, fflow, flowUnit[flowUnitIndex], ffee);
    //return status;
    this->showMessage("北工大校园网助手", status, QSystemTrayIcon::NoIcon);
    m_tmClick.stop();
}

void WndTrayIcon::openBjutWeb_action()
{
    try{
        QAction *action = static_cast<QAction*>(sender());
        if(action->data().type() == QVariant::UserType && action->data().canConvert<BjutWebItemInfo>()){
            BjutWebItemInfo info = action->data().value<BjutWebItemInfo>();
            if(info.url.isValid()){
                QDesktopServices::openUrl(info.url);
                increaseBjutWebFrequency(info.id);
                return;
            }
        }
    }catch(...){  }
    QMessageBox mb;
    mb.setWindowTitle("常用网址");
    mb.setText("无法识别的调用");
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}

void WndTrayIcon::initBjutWebMenu()
{
    m_menuBjutWeb->clear();

    for(const BjutWebItemInfo &w : m_vecBjutWeb){
        QAction *act = new QAction(w.name);
        act->setData(QVariant::fromValue(w));
        m_menuBjutWeb->addAction(act);
        connect(act, &QAction::triggered, this, &WndTrayIcon::openBjutWeb_action);
    }

}

void WndTrayIcon::cmdShowMainWnd()
{
    if (!m_wndMain)
    {
        m_wndMain = new WndMain(this);
    }
    m_wndMain->show();
}

void WndTrayIcon::cmdRestartDamon()
{
    m_bjutnet->restartDaemon();
}

void WndTrayIcon::cmdShowSettingWnd()
{
    if(!m_wndSetting)
    {
        m_wndSetting = new WndSetting(this);
    }
    m_wndSetting->show();
}

void WndTrayIcon::cmdExitApp()
{
    if(m_app != Q_NULLPTR)
    {
        m_app->quit();
    }
}

void WndTrayIcon::cmdExitAll()
{
    m_bjutnet->stopDaemon();
    cmdExitApp();
}

void WndTrayIcon::cmdLoginLgn()
{
    m_bjutnet->sendLogin();
}

void WndTrayIcon::cmdLogoutLgn()
{
    m_bjutnet->sendLogout();
}

void WndTrayIcon::increaseBjutWebFrequency(int id, long freq_inc)
{
    for(auto &w : m_vecBjutWeb){
        if(w.id == id){
            w.frequency += freq_inc;
            break;
        }
    }
    std::stable_sort(m_vecBjutWeb.begin(), m_vecBjutWeb.end(), [](const BjutWebItemInfo &a, const BjutWebItemInfo &b){
        return a.frequency > b.frequency;
    });
}

void WndTrayIcon::updateBjutWebFrequency(int id, long frequency)
{
    for(auto &w : m_vecBjutWeb){
        if(w.id == id){
            w.frequency = frequency;
            break;
        }
    }
    std::stable_sort(m_vecBjutWeb.begin(), m_vecBjutWeb.end(), [](const BjutWebItemInfo &a, const BjutWebItemInfo &b){
        return a.frequency > b.frequency;
    });
}

void WndTrayIcon::on_actived(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Context:
        if(m_tmClick.isActive())
        {//双击
            m_tmClick.stop();
            m_menuTray->exec();
        }
        else
        {//单击计时器
            m_tmClick.start();
        }
        break;
    case QSystemTrayIcon::DoubleClick:
        cmdShowMainWnd();
        break;
    case QSystemTrayIcon::Trigger:
        if(m_tmClick.isActive())
        {//双击
            m_tmClick.stop();
            cmdShowMainWnd();
        }
        else
        {//单击计时器
            m_tmClick.start();
        }
        break;
    default:
        break;
    }
}

void WndTrayIcon::reciveMessage(const QString &msg)
{
    const char *m = msg.toStdString().data();
    if(0==strcmp(m, "ShowMainWnd")){
        cmdShowMainWnd();
    }
    else if(0 == strcmp(m, "ShowSettingWnd")) {
        cmdShowSettingWnd();
    }
    else if(0==strcmp(m, "ExitApp")){
        cmdExitApp();
    }
}
}}
