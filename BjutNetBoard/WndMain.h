#ifndef BNA_GUI_MAINWND_H
#define BNA_GUI_MAINWND_H

#include "../BjutNetService/common.h"
#include "Updater.h"
#include "HLabel.h"
#include "HPanel.h"
#include "WndTrayIcon.h"
#include "ServiceBridge.h"
#include "UISetting.h"
#include <QMutex>
#include <tuple>
#include <mutex>

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
    void setFlowGraphType(UISetting::FlowGraphType type);

signals:
    void showed();
public slots:
    void initUI();
    void layoutUI();
    void initBjutWeb();
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
    void on_cmbListBook_highlight(int index);
    void on_btnSubmitBook_clicked();
    void on_lblShowMsg_clicked();
    void on_btnBjutWebCommon_clicked();
    void on_btnBjutWebMore_clicked();
    void on_lblVersion_clicked();
    void on_lblFeedback_clicked();
    void on_txtMsg_MessageWithTime(const QDateTime &time, const QString &info);
    void on_txtMsg_Message(const QString& info);
    void updateAccountStatus(bool login, int time, int flow, int fee);
    void updateOnlineDevices(const QVariant &var_info);
    // recive the info of account service
    void updateServiceInfo(const QString &name, int totalFlow);
    // recive the version of remote service
    void updateRemoteVerInfo(const QString &version, int inner_ver);
    //
    void updateBookedService(const QString &name);
protected slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void logRemoteDevice(int index, bool login);
    void updateFlowUsed(double used, double total);
protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void timerEvent(QTimerEvent *event);
    void showEvent(QShowEvent *event);

    void drawFlowPie(QPainter &painter, const QBrush &brushPie, double dFlowRate);
private:
    //界面所需变量
    QSize m_szFrameSimple;
    QSize m_szFrameAdvanced;
    QSize m_szFrameShowMsg;
    QFrame *m_frmGraph;
    QRect m_rectFlowGraph;
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
    bool m_bShowLog = false;
    bool m_bDraw2DFlowPie = false;
    QMutex m_mtxFlowPieFlash;
    int m_nFlowPieTimerID = 0;
    bool m_bFlowPieFlashOver = false;
    bool m_bFlowPieFlashEnter = false;
    double m_dFlowPieDegree = 0.;
    double m_dFlowPieDegreeMax = 0.;
    double m_dFlowPieDegreeSpeed = 0.;

    bool m_bNeedUpdate;
    QVector<BjutWebItemInfo> m_vecBjutWeb;
    //int m_strOnlineID[2];
    QList<OnlineDevice> m_lstOnline;
    bna::gui::Updater m_updater;
    QProgressDialog *m_dlgProgress;
    WndTrayIcon *m_tray;
    core::BjutNet *m_coreBjutNet;
    ServiceBridge m_guiServiceBridge;
};

inline void WndMain::setFlowGraphType(UISetting::FlowGraphType type)
{
    m_bDraw2DFlowPie = (type == UISetting::FLOW_GRAPH_PIE_2D);
}

}}


#endif // BNA_GUI_MAINWND_H
