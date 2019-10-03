#ifndef BNA_GUI_WNDTRAYICON_H
#define BNA_GUI_WNDTRAYICON_H

#include "../BjutNetService/common.h"
#include <QUrl>
#include <QAction>
#include <QVector>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QLocalServer>

namespace bna {
namespace core {
class BjutNet;
}
namespace gui {
class WndMain;
class WndSetting;
class WndHelp;

struct BjutWebItemInfo{
    int id;
    QString name;
    QUrl url;
    unsigned long frequency;
};

class WndTrayIcon : public QSystemTrayIcon
{
public:
    WndTrayIcon(QApplication *app, bna::core::BjutNet *core_bjut, QObject *parent = Q_NULLPTR);
    ~WndTrayIcon();
    PROPERTY_READ(bna::core::BjutNet*, BjutNet, m_coreBjutNet)
    PROPERTY_READ_WRITE(WndMain*, MainWindow, m_wndMain)
    PROPERTY_READ_CONST(QVector<BjutWebItemInfo>, BjutWebList, m_vecBjutWeb)

signals:
    void quitApp();

public slots:
    void reciveMessage(const QString &msg);
    void cmdExitApp();
    void cmdExitAll();
    void cmdShowMainWnd();
    void cmdShowSettingWnd();
    void cmdShowHelpWnd();
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
    bna::core::BjutNet *m_coreBjutNet;
    WndMain *m_wndMain;
    WndSetting *m_wndSetting;
    WndHelp *m_wndHelp;
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

#endif // BNA_GUI_WNDTRAYICON_H
