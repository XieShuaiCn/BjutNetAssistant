#include "WndInit.h"
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QProcess>

QString RandString(int length)
{
    qsrand(QTime::currentTime().msec()
           +QTime::currentTime().second()*1000
           +QTime::currentTime().minute()*60000);
    const QString set("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    QString result;
    for(int i = 0; i < length; ++i) {
        result.append(set[qrand() % 36]);
    }
    return result;
}

WndInit::WndInit(QApplication *app)
    : QProgressDialog("正在初始化更新", "取消", 0, 100, nullptr, Qt::Tool),
      m_bCancel(false),
      m_app(app)
{
    Q_ASSERT(app!=nullptr);
#ifdef Q_OS_WIN32
    m_strBinName = app->applicationName()+".exe";
#else
    m_strBinName = app->applicationName();
#endif
    m_strBinDir = app->applicationDirPath();
    m_strBinFile = QDir(m_strBinDir).absoluteFilePath(m_strBinName);
    this->setWindowTitle("网关登录器 更新");
    connect(this, &QProgressDialog::canceled, this, &WndInit::cancelInit);
    connect(&m_tmInit, &QTimer::timeout, this, &WndInit::initUpdater);
}

void WndInit::cancelInit()
{
    m_bCancel = true;
}

void WndInit::show()
{
    QProgressDialog::show();
    m_tmInit.setSingleShot(true);
    m_tmInit.setInterval(10);
    m_tmInit.start();
}

void WndInit::initUpdater()
{
    m_tmInit.stop();
    // create temp file.
    QString tmpName = QDir::temp().absoluteFilePath("BjutNetLogin_updater_.tmp");
    for(int i =0; i < 30; ++i)
    {
        QString rs = RandString(6);
        tmpName = QDir::temp().absoluteFilePath("BjutNetLogin_updater_" + rs + ".tmp");
        QDir tmpDir(tmpName);
        if(tmpDir.exists()){
            if(!tmpDir.rmpath(tmpName) && i > 10)
                break;
        }
        if(!tmpDir.exists()){
            break;
        }
    }
    if(QDir("/").mkpath(tmpName))
    {
        this->setValue(10);
    }
    else
    {
        this->setLabelText("临时目录创建失败");
        return;
    }
    m_strTmpDir = tmpName;
    QDir dirBin(m_strBinDir);
    QDir dirTmp(m_strTmpDir);
    m_strTmpFile = dirTmp.absoluteFilePath(m_strBinName);
    QStringList dependents{
#ifdef Q_OS_WIN32
        m_strBinName, "platforms\\qwindows.dll",
        "Qt5Core.dll", "Qt5Gui.dll", "Qt5Widgets.dll", "Qt5Network.dll",
        "libstdc++-6.dll", "libgcc_s_dw2-1.dll", "libwinpthread-1.dll"
#else
        m_strBinName, "platforms/libqlinuxfb.so", "platforms/libqxcb.so",
        "libQt5Core.so.5", "libQt5Gui.so.5", "libQt5Widgets.so.5", "libQt5Network.so.5",
        "libQt5XcbQpa.so.5", "libQt5DBus.so.5"
#endif
    };
    QStringList dependents_dir{ "platforms" };
    // copy file
    bool copy_fail = false;
    do{
        int sum = dependents.size() + dependents_dir.size();
        float step = 80.0 / sum;
        for(auto &dir : dependents_dir){
            if(!dirTmp.mkpath(dir)) {
                copy_fail = true;
                break;
            }
            this->setValue(int(this->value()+step));

        }
        if(copy_fail) break;
        for(auto &file : dependents)
        {
            auto strBinFile = dirBin.absoluteFilePath(file);
            auto strTmpFile = dirTmp.absoluteFilePath(file);
            if(!QFile::copy(strBinFile, strTmpFile)){
                copy_fail = true;
                break;
            }
            this->setValue(int(this->value()+step));
        }
    }while(false);
    if (copy_fail){
        this->setLabelText("更新初始化失败");
        return;
    }
    this->setValue(90);

    QStringList args;
    args.push_back("-update");
    args.push_back("-dir");
    args.push_back(m_strBinDir);
    if(QProcess::startDetached(m_strTmpFile, args, m_strBinDir))
    {
        this->setValue(100);
        m_app->exit(0);
    }
    else
    {
        this->setLabelText("更新启动失败");
        return;
    }
}
