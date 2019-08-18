#ifndef MAINWND_H
#define MAINWND_H

#include "common.h"
#include "Updater.h"
#include "HLabel.h"
#include "HPanel.h"
#include "WndTrayIcon.h"
#include <tuple>

class QAction;
class QApplication;
class QButtonGroup;
class QComboBox;
class QHeaderView;
class QLCDNumber;
class QLabel;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QWidget;
class QMenu;
class QShowEvent;
class QCloseEvent;
class QPaintEvent;
class QProgressDialog;

namespace bna{
namespace gui{

class WndTrayIcon;
class BjutNet;

class WndMain : public QWidget
{
    Q_OBJECT

    using OnlineDevice = std::tuple<int, QString, QString, QString, bool, bool>;
    enum OnlineDeviceIdx{
        OLDEV_ID,
        OLDEV_IPV4,
        OLDEV_IPV6,
        OLDEV_MAC,
        OLDEV_ONLINE,
        OLDEV_LOCAL
    };

public:
    WndMain(WndTrayIcon *tray, QWidget *parent = Q_NULLPTR);
    ~WndMain();
    void show();

Q_SIGNALS:
    void showed();
public slots:
    void initUI();
    void initBjutWeb();
    void on_show();
    void on_btnRefresh_clicked();
    void on_btnSetting_clicked();
    void on_btnHelp_clicked();
    void on_btnDetail_clicked();
    void on_btnLogout_clicked();
    void on_btnLogin_clicked();
    void on_btnOnline1_clicked();
    void on_btnOnline2_clicked();
    void on_btnOffline1_clicked();
    void on_btnOffline2_clicked();
    void on_btnForceOffline1_clicked();
    void on_btnForceOffline2_clicked();
    void on_lblClientaddr_doubleClicked();
    void on_btnRefreshBook_clicked();
    void on_btnSubmitBook_clicked();
    void on_lblShowMsg_clicked();
    void on_btnBjutWebCommon_clicked();
    void on_btnBjutWebMore_clicked();
    void on_lblVersion_clicked();
    void on_txtMsg_message(const QString& info);
    void on_account_status(bool login, int flow, int time, int fee);
    void on_online_status(const QVariant &var_info);
    // recive the info of account service
    void on_serviceInfo(const QString &name, int totalFlow);
    // recive the version of remote service
    void on_remoteVersion(const QString &version, int inner_ver);
    //
    void on_allServices(const QVariant &services);
    //
    void on_bookedService(const QString &name);
protected slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    //界面所需变量
    QSize m_szFrameSimple;
    QSize m_szFrameAdvanced;
    QSize m_szFrameShowMsg;
    QFrame *m_frmGraph;
    QRect m_rectFlowGraphTop;
    QRect m_rectFlowGraphBottom;
    int m_nFlowGraphThickness;
    QFrame *m_frmFlowGraph;
    QLabel *m_lblService;
    QLabel *m_lblFlowUsed;
    QLabel *m_lblStatus;
    QLabel *m_lblStatusFlag;
    QPushButton *m_btnRefresh;
    QPushButton *m_btnSetting;
    QPushButton *m_btnHelp;

    QFrame *m_frmInfo;
    QLabel *m_lblInfoTime;
    QLabel *m_lblInfoFlow;
    QLabel *m_lblInfoFee;
    QLCDNumber *m_lcdNumTime;
    QLCDNumber *m_lcdNumFlow;
    QLCDNumber *m_lcdNumFee;
    QLabel *m_lblTimeUnit;
    QLabel *m_lblFlowUnit;
    QLabel *m_lblFeeUnit;

    QPushButton *m_btnLogin;
    QPushButton *m_btnLogout;
    QPushButton *m_btnDetail;

    HPanel *m_frmOnline;
    //QLabel *m_lblCopyTip;
    QLabel *m_lblClent1_ip4;
    QLabel *m_lblClent1_ip6;
    HLabel *m_lblClent1_addr4;
    HLabel *m_lblClent1_addr6;
    QPushButton *m_btnForceOffline1;
    QPushButton *m_btnOffline1;
    QPushButton *m_btnOnline1;
    QLabel *m_lblClent2_ip4;
    QLabel *m_lblClent2_ip6;
    HLabel *m_lblClent2_addr4;
    HLabel *m_lblClent2_addr6;
    QPushButton *m_btnForceOffline2;
    QPushButton *m_btnOffline2;
    QPushButton *m_btnOnline2;

    HPanel *m_frmBjutWeb;
    QPushButton *m_btnBjutWeb1;
    QPushButton *m_btnBjutWeb2;
    QPushButton *m_btnBjutWeb3;
    QPushButton *m_btnBjutWeb4;
    QPushButton *m_btnBjutWeb5;
    QPushButton *m_btnBjutWebMore;
    QMenu *m_menuBjutWeb;
    HPanel *m_frmOperation;
    QLabel *m_lblCurrentBook;
    QComboBox *m_cmbListBook;
    QPushButton *m_btnSubmitBook;
    QPushButton *m_btnRefreshBook;

    HLabel *m_lblShowMsg;
    QTextEdit *m_txtMsg;
    HLabel *m_lblFeedback;
    HLabel *m_lblVersion;

    //逻辑控制所需变量
    bool m_bShowDetail = false;
    bool m_bShowMsg = false;
    bool m_bApplyLogin;
    bool m_bNeedUpdate;
    QVector<BjutWebItemInfo> m_vecBjutWeb;
    //int m_strOnlineID[2];
    QList<OnlineDevice> m_lstOnline;
    bna::gui::Updater m_updater;
    QProgressDialog *m_dlgProgress;
    WndTrayIcon *m_tray;
    BjutNet *m_net;

};

}}


#endif // MAINWND_H
