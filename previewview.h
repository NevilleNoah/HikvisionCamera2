#ifndef PREVIEWVIEW_H
#define PREVIEWVIEW_H

#include "headinfo.h"
#include "database.h"
#include "config.h"

namespace Ui {
    class PreviewView;
}

class PreviewView : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewView(QWidget *parent = nullptr);
    ~PreviewView();
    //本类
    static PreviewView* previewView;
    //预览属性设置
    static LONG lUserID;//用户句柄
    static LONG lRealPlayHandle;//播放句柄
    //画面设置
    static WORD m_wPicSize;//预览画面大小设置
    static WORD m_wPicQuality;//预览画面质量设置
    //报警信息
    static NET_VCA_FACESNAP_MATCH_ALARM struFaceMatchAlarm;
    static ALARM_INFO alarmInfo;//存储当前报警信息
    static QList<ALARM_INFO> alarmList;//报警信息列表
    static QString alarmText;//打印输出的文本

    static QList<int> searchList;//存储搜索信息

    //人脸库图
    static char* avatar;
    static DWORD avatarLen;
    static DWORD avatarIDLen;
    //抓拍图
    static char* capture;
    static DWORD captureLen;
    //相似度
    static double similarity;
    //数据库
    static Database database;

    //是否点击搜索
    static bool isClickSearch;
    //搜索的名字
    static QString inputName;

    static QList<char*> avatarList;
    static QList<char*> captureList;
    static QString currentAlarmInfo;
    //住址
    static ADDRESS_INFO addressInfo;

    static int currentRow;
    static QString dirAvatar;
    static QString dirCapture;
    //static QString dirFace;
    //static QString dirStranger;
    static QString dirPicAvatar;
    static QString dirPicCapture;
    //static QString dirPicFace;
    //static QString dirPicStranger;
    static QString urlCapture;
    static QString urlAvatar;

    void initAlarmTable();
    //static void initConfig();

    static BOOL CALLBACK MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser);
    static void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
    static void convertUnCharToStr(BYTE *UnChar,char *hexStr, char *str, int len);
    static void setAlarmInfo(NET_VCA_FACESNAP_MATCH_ALARM struFaceMatchAlarm);
    static void setAlarmInfo();
    static void setAlarmText();
    static void saveToDatabase();
    static bool isSetAlarmText();
    static void initDatabase();


    //下载抓拍图并显示
    static void downLoadCapturePic();
    //下载人脸图并显示
    static void downLoadAvatarPic();
    //下载人脸子图并显示
    static void downLoadFacePic();
    //设置抓拍图的显示
    void setCapturePic(QImage imgCapture);
    //设置人脸图的显示
    void setAvatarPic(QImage imgAvatar);
    //设置人脸子图的显示
    void setFacePic(QImage imgFace);
    //设置标志的显示
    void setSymbolPic(QImage imgSymbol);
    //设置个人信息的显示
    void setEdPersonInfo(QString applicantInfo, QString sfzNo, QString similarity, QString address);
    //上传陌生人人脸子图
    static void uploadStrangerFacePic();

    //批量查询人脸比对库信息 输出为xml信息
    static void getNET_DVR_STDXMLConfig();


private:

signals:
    void clearAlarmList();
public slots:
    void showPersonInfo(int option);
    void showCapturePic(QNetworkReply*);
    void showAvatarPic(QNetworkReply*);
    void showFacePic(QNetworkReply*);
    void addAlarmItem();

    void loadPreview();

private slots:
    void on_alarmList_itemDoubleClicked(QListWidgetItem *item);

    void on_btnAlarmClear_clicked();

    void on_btnSearch_clicked();

private:
    Ui::PreviewView *ui;
};



#endif // PREVIEWVIEW_H
