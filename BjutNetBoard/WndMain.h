#ifndef MAINWND_H
#define MAINWND_H

#include "common.h"
#include "Updater.h"
#include "HLabel.h"

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

class QServiceBridge;
class WndTrayIcon;

class WndMain : public QWidget
{
    Q_OBJECT
public:
    WndMain(WndTrayIcon *tray, QWidget *parent = Q_NULLPTR);
    ~WndMain();
    void show();

Q_SIGNALS:
    void showed();
public slots:
    void initUI();
    void on_show();
    void on_btnRefresh_clicked();
    void on_btnDetail_clicked();
    void on_btnLogout_clicked();
    void on_btnLogin_clicked();
    void on_btnOffline1_clicked();
    void on_btnOffline2_clicked();
    void on_lblClient1addr4_doubleClicked();
    void on_lblClient1addr6_doubleClicked();
    void on_lblClient2addr4_doubleClicked();
    void on_lblClient2addr6_doubleClicked();
    void on_btnOffcicalWeb_clicked();
    void on_btnRefreshBook_clicked();
    void on_btnSubmitBook_clicked();
    void on_lblVersion_clicked();
    void on_txtMsg_message(const QDateTime& time, const QString& info);
    void on_account_status(bool login, int time, int flow, int fee);
    void on_online_status(const QVector<OnlineClientInfo> &info);
protected slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    //界面所需变量
    QFrame *m_frmGraph;
    QRect m_rectFlowGraph;
    QFrame *m_frmFlowGraph;
    QLabel *m_lblService;
    QLabel *m_lblFlowUsed;
    QLabel *m_lblStatus;
    QLabel *m_lblStatusFlag;
    QPushButton *m_btnRefresh;

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

    QFrame *m_frmOnline;
    QLabel *m_lblCopyTip;
    QLabel *m_lblClent1_ip4;
    QLabel *m_lblClent1_ip6;
    HLabel *m_lblClent1_addr4;
    HLabel *m_lblClent1_addr6;
    QPushButton *m_btnOffline1;
    QLabel *m_lblClent2_ip4;
    QLabel *m_lblClent2_ip6;
    HLabel *m_lblClent2_addr4;
    HLabel *m_lblClent2_addr6;
    QPushButton *m_btnOffline2;

    QFrame *m_frmOperation;
    QPushButton *m_btnOfficalWeb;
    QLabel *m_lblCurrentBook;
    QComboBox *m_cmbListBook;
    QPushButton *m_btnSubmitBook;
    QPushButton *m_btnRefreshBook;

    QTextEdit *m_txtMsg;
    HLabel *m_lblVersion;

    //逻辑控制所需变量
    bool m_bShowDetail = false;
    bool m_bApplyLogin;
    bool m_bNeedUpdate;
    QString m_strOnlineID[2];
    Updater m_updater;
    QProgressDialog *m_dlgProgress;
    WndTrayIcon *m_tray;
    QServiceBridge *m_service;

};
}
#endif // MAINWND_H
