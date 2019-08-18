#ifndef WNDTRAYICON_H
#define WNDTRAYICON_H

#include "common.h"
#include <QUrl>
#include <QAction>
#include <QVector>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QLocalServer>

class bna_gui_WndMain;
class bna_gui_WndSetting;
class bna_gui_BjutNet;

namespace bna {
namespace gui {

class BjutNet;
class WndMain;
class WndSetting;

struct BjutWebItemInfo{
    int id;
    QString name;
    QUrl url;
    unsigned long frequency;
};

class WndTrayIcon : public QSystemTrayIcon
{
public:
    WndTrayIcon(QApplication *app, QObject *parent = Q_NULLPTR);
    ~WndTrayIcon();
    PROPERTY_READ(BjutNet*, BjutNet, m_bjutnet)
    PROPERTY_READ_WRITE(WndMain*, MainWindow, m_wndMain)
    PROPERTY_READ_CONST(QVector<BjutWebItemInfo>, BjutWebList, m_vecBjutWeb)

Q_SIGNALS:
    void quitApp();

public slots:
    void reciveMessage(const QString &msg);
    void cmdExitApp();
    void cmdShowMainWnd();
    void cmdShowSettingWnd();
    void cmdLoginLgn();
    void cmdLogoutLgn();

    void increaseBjutWebFrequency(int id, long freq_inc = 1);
    void updateBjutWebFrequency(int id, long frequency);

protected slots:
    void on_actived(QSystemTrayIcon::ActivationReason reason);
    void on_clicked();
    void openBjutWeb_action();
    void initBjutWebMenu();

    QTimer m_tmClick;

    QApplication *m_app;
    WndMain *m_wndMain;
    WndSetting *m_wndSetting;
    BjutNet *m_bjutnet;
    QMenu *m_menuTray;
    QAction *m_actMenuShowMain;
    QAction *m_actMenuLogout;
    QAction *m_actMenuLogin;
    QAction *m_actMenuSetting;
    QAction *m_actMenuQuit;

    QMenu *m_menuBjutWeb;

    QVector<BjutWebItemInfo> m_vecBjutWeb;
};

}}

Q_DECLARE_METATYPE(bna::gui::BjutWebItemInfo)

#endif // WNDTRAYICON_H
