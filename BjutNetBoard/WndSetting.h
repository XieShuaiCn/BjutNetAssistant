#ifndef BNA_GUI_WNDSETTING_H
#define BNA_GUI_WNDSETTING_H

#include "../BjutNetService/common.h"
#include <QWidget>

class QWidget;
class QFrame;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QCheckBox;
class QLabel;
class QComboBox;
class QRadioButton;

namespace bna {
namespace gui {
class WndTrayIcon;
class HPanel;

class WndSetting : public QWidget
{
    Q_OBJECT
public:
    explicit WndSetting(WndTrayIcon *tray, QWidget *parent = Q_NULLPTR);

signals:

public slots:
    void initUI();
    void show();
    void on_chkAutoRun_clicked(bool checked);
    void on_chkDebug_clicked(bool checked);
    void on_rdFlowGraphPie2D_clicked(bool checked);
    void on_rdFlowGraphPie3D_clicked(bool checked);
    void on_btnApply_clicked();
    void on_btnApplyLogin_clicked();
    void updateAccountInfo(const QString &name, const QString &passwd, int type);

private:
    WndTrayIcon *m_tray;
    HPanel *m_frmBasic;
    QCheckBox *m_chkAutoRun;
    QCheckBox *m_chkAppDebug;
    QLabel *m_lblAutoRun;

    HPanel *m_frmGUI;
    HPanel *m_frmFlowGraph;
    QRadioButton *m_rdFlowGraphPie2D;
    QRadioButton *m_rdFlowGraphPie3D;

    HPanel *m_frmAccount;
    QLabel *m_lblAccount;
    QLabel *m_lblPassword;
    QLabel *m_lblType;
    QLineEdit *m_editAccount;
    QLineEdit *m_editPassword;
    QComboBox *m_cmbType;
    QPushButton *m_btnApply;
    QPushButton *m_btnApplyLogin;

    QLabel *m_lblURL;

    Q_DISABLE_COPY(WndSetting)
};

}
}


#endif // BNA_GUI_WNDSETTING_H
