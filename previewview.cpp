﻿#include <cstring>
#include "previewview.h"
#include "ui_previewview.h"

const int OPTION_FACE_COMPARE = 0;
const int OPTION_DOUBLE_CLICK = 1;
const QString UNKNOW = QString::fromLocal8Bit("未知");

//预览属性设置
LONG PreviewView::lUserID;//用户句柄
LONG PreviewView::lRealPlayHandle;//播放句柄
//画面设置
WORD PreviewView::m_wPicSize;//预览画面大小设置
WORD PreviewView::m_wPicQuality;//预览画面质量设置
//人脸库图
char* PreviewView::avatar;
DWORD PreviewView::avatarLen;
DWORD PreviewView::avatarIDLen;
//抓拍图
char* PreviewView::capture;
DWORD PreviewView::captureLen;
//相似度
double PreviewView::similarity;
int PreviewView::currentRow;
//保存路径
QString PreviewView::dirAvatar;
QString PreviewView::dirCapture;
//QString PreviewView::dirFace;
//QString PreviewView::dirStranger;
QString PreviewView::dirPicAvatar;
QString PreviewView::dirPicCapture;
//QString PreviewView::dirPicFace;
//QString PreviewView::dirPicStranger;
//报警信息
QList<ALARM_INFO> PreviewView::alarmList;
ALARM_INFO PreviewView::alarmInfo;
QString PreviewView::alarmText;
//搜索信息
QList<int> PreviewView::searchList;
//信息列表
//QList<char*> PreviewView::avatarList;
//QList<char*> PreviewView::captureList;
QString PreviewView::currentAlarmInfo;
//URL
QString PreviewView::urlCapture;
QString PreviewView::urlAvatar;
//人脸比对结果报警上传结构体。
NET_VCA_FACESNAP_MATCH_ALARM PreviewView::struFaceMatchAlarm = {0};
//数据库
Database PreviewView::database;

PreviewView* PreviewView::previewView = nullptr;

//是否点击搜索按钮
bool PreviewView::isClickSearch = false;
//搜索的名字
QString PreviewView::inputName = "";

//住址
ADDRESS_INFO PreviewView::addressInfo;


PreviewView::PreviewView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewView)
{
    previewView = this;
    ui->setupUi(this);

    //connect(this, SIGNAL(clearAlarmList()), this, SLOT(on_btnAlarmClear_clicked()));
    loadPreview();
}

PreviewView::~PreviewView() {
    delete ui;
}

void PreviewView::initDatabase() {
    database.setQSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
}

void PreviewView::setCapturePic(QImage imgCapture) {
    ui->picCapture->setPixmap(QPixmap::fromImage(imgCapture).scaled(ui->picCapture->size(),
                              Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void PreviewView::setAvatarPic(QImage imgAvatar) {
    ui->picAvatar->setPixmap(QPixmap::fromImage(imgAvatar).scaled(ui->picAvatar->size(),
                             Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void PreviewView::setFacePic(QImage imgFace) {
    ui->picFace->setPixmap(QPixmap::fromImage(imgFace).scaled(ui->picFace->size(),
                           Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void PreviewView::setSymbolPic(QImage imgSymbol) {
    ui->picSymbol->setPixmap(QPixmap::fromImage(imgSymbol).scaled(ui->picSymbol->size(),
                             Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void PreviewView::setEdPersonInfo(QString applicantInfo, QString sfzNo, QString similarity, QString address) {
    ui->edName->setText(applicantInfo);
    ui->edId->setText(sfzNo);
    ui->edSimilarity->setText(similarity);
    ui->edAddress->setText(address);
}

void CALLBACK PreviewView::g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser) {
    char tempbuf[256] = { 0 };
    switch (dwType) {
    case EXCEPTION_RECONNECT:    //预览时重连
        printf_s("----------reconnect--------%d\n", time(NULL));
        break;
    default:
        break;
    }
}

//报警回调函数
BOOL CALLBACK PreviewView::MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
    qDebug() << "PreviewView: Callback start";

    //报警列表显示的报警总数上限控制
    if(alarmList.size()>=100) {
        emit previewView->clearAlarmList();
    }

    switch(lCommand) {
        case COMM_SNAP_MATCH_ALARM: //人脸比对结果信息
        {
            qDebug() << "PreviewView: Detected face";
            memcpy(&struFaceMatchAlarm, pAlarmInfo, sizeof(NET_VCA_FACESNAP_MATCH_ALARM));
            //设置报警信息，用于记录所需用到的的报警数据
            setAlarmInfo(struFaceMatchAlarm);
            //设置报警文本，用于在下方列表区显示
            setAlarmText();
            //数据库操作
            saveToDatabase();
            break;
        }
        return TRUE;
    }
}

void PreviewView::downLoadCapturePic() {
    capture = (char*)struFaceMatchAlarm.pSnapPicBuffer;
    int captureCutIndex = QString(capture).indexOf("SEl");
    urlCapture = QString(capture).mid(0, captureCutIndex);
    QEventLoop eventLoop;
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QUrl url(urlCapture);
    url.setUserName(Config::getCameraInfoUserName());
    url.setPassword(Config::getCameraInfoPassWord());
    QNetworkReply* reply = manager->get(QNetworkRequest(url));
    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    connect(manager, SIGNAL(finished(QNetworkReply*)), previewView, SLOT(showCapturePic(QNetworkReply*)));
    eventLoop.exec();
}

void PreviewView::downLoadAvatarPic() {
    avatar = (char*)struFaceMatchAlarm.struBlackListInfo.pBuffer1;
    int avatarCutIndex = QString(avatar).indexOf("http://",1);
    urlAvatar = QString(avatar).mid(0, avatarCutIndex);
    QEventLoop eventLoop;
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QUrl url(urlAvatar);
    url.setUserName(Config::getCameraInfoUserName());
    url.setPassword(Config::getCameraInfoPassWord());
    QNetworkReply* reply = manager->get(QNetworkRequest(url));
    connect(manager, SIGNAL(finished(QNetworkReply*)), previewView, SLOT(showAvatarPic(QNetworkReply*)));
    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
}

void PreviewView::downLoadFacePic() {
    QString facePic = QString::fromLocal8Bit((char*)struFaceMatchAlarm.struSnapInfo.pBuffer1);
    int faceCutIndex = facePic.mid(6).indexOf("http://")+6;
    QString urlFacePic = facePic.mid(0, faceCutIndex);
    QEventLoop eventLoop;
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QUrl url(urlFacePic);
    url.setUserName(Config::getCameraInfoUserName());
    url.setPassword(Config::getCameraInfoPassWord());
    QNetworkReply* reply = manager->get(QNetworkRequest(url));
    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    connect(manager, SIGNAL(finished(QNetworkReply*)), previewView, SLOT(showFacePic(QNetworkReply*)));
    eventLoop.exec();
}

/**
 * @brief PreviewView::setAlarmInfo
 * @param struFaceMatchAlarm
 */

void PreviewView::setAlarmInfo(NET_VCA_FACESNAP_MATCH_ALARM struFaceMatchAlarm) {
    qDebug() << "PreviewView: setAlarmInfo start";

    /***********************************************设置时间********************************************/
    alarmInfo.dwYear = GET_YEAR(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    alarmInfo.dwMonth = GET_MONTH(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    alarmInfo.dwDay = GET_DAY(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    alarmInfo.dwHour = GET_HOUR(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    alarmInfo.dwMinute = GET_MINUTE(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    alarmInfo.dwSecond = GET_SECOND(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    /*********************************************设置时间 END******************************************/

    /*********************************************设置个人信息******************************************/
    QString strName = "";
    bool hasUpload = true;//如果是陌生人，判断是否已经上传至超脑进行建模
    if(struFaceMatchAlarm.fSimilarity > Config::getCompareInfoSimilarity()) {
        QString applicantName = QString::fromLocal8Bit((char*)struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.byName);
        if(applicantName[applicantName.length()-1] == 'm') {
            //设置为陌生人
            alarmInfo.isStranger = true;
            //相似度
            alarmInfo.similarity = struFaceMatchAlarm.fSimilarity;
            //姓名
            initDatabase();
            //int strQuantity = database.selectStrQuantityByTime(alarmInfo.dwYear, alarmInfo.dwMonth, alarmInfo.dwDay);
            //int idx = applicantName.lastIndexOf('m');
            //alarmInfo.applicant = applicantName + (QString::number(strQuantity).sprintf("%06d",strQuantity));
            alarmInfo.applicant = applicantName.mid(0, applicantName.length()-1);
            alarmInfo.idAvatar = alarmInfo.applicant;
            alarmInfo.isUpload = true;
        } else {
            //设置不为陌生人
            alarmInfo.isStranger = false;
            //相似度
            alarmInfo.similarity = struFaceMatchAlarm.fSimilarity*100;
            //获取身份证号
            alarmInfo.sfzNo = QString::fromLocal8Bit((char*)struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.byName);
            //姓名
            initDatabase();
            alarmInfo.applicant = database.selectApplicantInfoBySfzNo(alarmInfo.sfzNo).applicant;
            //住址
            addressInfo = database.selectAddress(alarmInfo.applicant, alarmInfo.sfzNo);
            qDebug() << "addressInfo: " << addressInfo.community;
            //身份证号
            alarmInfo.idAvatar = alarmInfo.sfzNo;
        }
        //人脸库头像图
        downLoadAvatarPic();

        /*
        //设置不为陌生人
        alarmInfo.isStranger = false;
        //相似度
        alarmInfo.similarity = struFaceMatchAlarm.fSimilarity*100;
        //身份证号
        alarmInfo.sfzNo = QString::fromLocal8Bit((char*)struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.byName);
        //姓名
        initDatabase();
        alarmInfo.applicant = database.selectApplicantInfoBySfzNo(alarmInfo.sfzNo).applicant;
        //住址
        addressInfo = database.selectAddress(alarmInfo.applicant, alarmInfo.sfzNo);
        qDebug() << "addressInfo: " << addressInfo.community;
        //人脸库头像图
        alarmInfo.idAvatar = alarmInfo.sfzNo;
        downLoadAvatarPic();
        */
    } else {
        hasUpload = false;
        strName = QString::number(alarmInfo.dwYear)+QString::number(alarmInfo.dwMonth).sprintf("%02d",alarmInfo.dwMonth)+
                        QString::number(alarmInfo.dwDay).sprintf("%02d",alarmInfo.dwDay)+
                        QString::number(alarmInfo.dwHour).sprintf("%02d",alarmInfo.dwHour)+QString::number(alarmInfo.dwMinute).sprintf("%02d",alarmInfo.dwMinute)+
                        QString::number(alarmInfo.dwSecond).sprintf("%02d",alarmInfo.dwSecond);
        //姓名
        //initDatabase();
        //int strQuantity = database.selectStrQuantityByTime(alarmInfo.dwYear, alarmInfo.dwMonth, alarmInfo.dwDay);
        //alarmInfo.applicant = strName + QString::number(strQuantity).sprintf("%06d",strQuantity);
        alarmInfo.applicant = strName;
        qDebug() << "strName: " << strName;
        //设置为陌生人
        alarmInfo.isStranger = true;
        //相似度
        alarmInfo.similarity = struFaceMatchAlarm.fSimilarity;
        alarmInfo.isUpload = false;
    }

    Sleep(300);

    //--------------------
    //抓拍图
    alarmInfo.idCapture = QString::number(struFaceMatchAlarm.struSnapInfo.dwSnapFacePicID);
    downLoadCapturePic();

    //人脸子图
    downLoadFacePic();

    //等图片下载至本地后再进行上传
    Sleep(300);
    //如果是陌生人那么把人脸子图上传至超脑进行建模
    if(hasUpload == false) {
        QString dirFace = Config::getDirInfoStranger();
        dirFace.append(alarmInfo.idCapture + ".jpg");
        qDebug() << "xxx: dirFace path: " << dirFace;
        bool isSuccess = uploadStrangerFacePic(strName+"m", dirFace);
        qDebug() << "isSuccess: " << isSuccess;
    }
    //显示个人信息
    emit previewView->showPersonInfo(OPTION_FACE_COMPARE);

    alarmList.append(alarmInfo);
    searchList.append(alarmList.length()-1);

    /*********************************************设置个人信息 END******************************************/
}

//判断是否设置报警信息
bool PreviewView::isSetAlarmText() {
    if(isClickSearch == false) {
        return true;
    }
    if(isClickSearch == true && (inputName.compare(QString::fromLocal8Bit(alarmInfo.name)) == 0)) {
        return true;
    }
    if(isClickSearch == true && inputName.compare(QString::fromLocal8Bit("陌生人")) == 0
            && alarmInfo.isStranger == true) {
        return true;
    }
    return false;
}

void PreviewView::setAlarmText() {
    if(isSetAlarmText()){
        //报警信息
        alarmText = QString::asprintf("%4.4d.%2.2d.%2.2d %2.2d:%2.2d:%2.2d   ",
                                  alarmInfo.dwYear,
                                  alarmInfo.dwMonth,
                                  alarmInfo.dwDay,
                                  alarmInfo.dwHour,
                                  alarmInfo.dwMinute,
                                  alarmInfo.dwSecond);
        if(!alarmInfo.isStranger) {
            alarmText.append(QString::fromLocal8Bit("   姓名："));
            alarmText.append(alarmInfo.applicant);

            alarmText.append(QString::fromLocal8Bit("   编号："));
            alarmText.append(alarmInfo.sfzNo);
        } else {
            alarmText.append(QString::fromLocal8Bit("   陌生人"));
            alarmText.append(QString::fromLocal8Bit("   编号："));
            alarmText.append(alarmInfo.applicant);
        }
        emit previewView->addAlarmItem();
    }
}

void PreviewView::getNET_DVR_STDXMLConfig() {
    NET_DVR_XML_CONFIG_INPUT  lpInputParam = {0};
    NET_DVR_XML_CONFIG_OUTPUT lpOutputParam = {0};
    qDebug() << "enter getNET_DVR_STDXMLConfig";//不要删除这条输出
    lpInputParam.dwSize = sizeof(lpInputParam);
    char szUrl[256] = { 0 };
    sprintf(szUrl, "GET /ISAPI/Intelligent/FDLib\r\n");
    lpInputParam.lpRequestUrl = szUrl;
    lpInputParam.dwRequestUrlLen = strlen(szUrl);
    lpInputParam.lpInBuffer = NULL;
    lpInputParam.dwInBufferSize = 0;
    lpInputParam.dwRecvTimeOut = 0;
    lpInputParam.byForceEncrpt = 0;
    lpInputParam.byNumOfMultiPart = 0;
    memset(lpInputParam.byRes, 0, sizeof (lpInputParam.byRes));
    /********************lpOutBuffer********************/
    void  *lpOutBuffer;
    lpOutputParam.dwSize = sizeof(lpOutputParam);
    lpOutputParam.lpOutBuffer = lpOutBuffer;
    lpOutputParam.dwOutBufferSize = 1024;
    DWORD  dwReturnedXMLSize;
    lpOutputParam.dwReturnedXMLSize = dwReturnedXMLSize;
    void  *lpStatusBuffer;
    lpOutputParam.lpStatusBuffer = lpStatusBuffer;
    lpOutputParam.dwStatusSize = 0;
    memset(lpOutputParam.byRes, 0, sizeof (lpOutputParam.byRes));
    /********************lpOutBuffer********************/

    bool f = NET_DVR_STDXMLConfig(lUserID, &lpInputParam, &lpOutputParam);
    qDebug() << "dwReturnedXMLSize: " << dwReturnedXMLSize;
    qDebug() << "lpOutBuffer: " << (char*)lpOutBuffer;
    qDebug() << "lpStatusBuffer: " << (char*)lpStatusBuffer;
    qDebug() << "flag: " << f << " errorCode: " << NET_DVR_GetLastError();
}


bool PreviewView::uploadStrangerFacePic(QString stName, QString picFileName) {
    XMLSet::setUploadStrangerXML(stName);
    qDebug() << "enter uploadStrangerFacePic";
    char FDID[256] = "0A949258191A4154B09E16FE95DF6FE1";
    QByteArray qArray = picFileName.toLocal8Bit();
    char szPicFileName[256];
    strcpy(szPicFileName, qArray.data());

    qArray = XMLSet::strangerXMLFile.toLocal8Bit();
    char szXMLFileName[256];
    strcpy(szXMLFileName, qArray.data());

    NET_DVR_FACELIB_COND struFaceLibCond = { 0 };
    struFaceLibCond.dwSize = sizeof(NET_DVR_FACELIB_COND);
    strcpy(struFaceLibCond.szFDID, FDID);
    struFaceLibCond.byConcurrent = 0;
    struFaceLibCond.byCover = 0;
    struFaceLibCond.byCustomFaceLibID = 0;
    //建立连接
    LONG m_lUploadHandle = NET_DVR_UploadFile_V40(lUserID, IMPORT_DATA_TO_FACELIB, &struFaceLibCond, sizeof(NET_DVR_FACELIB_COND),
                                                  NULL, NULL, 0);
    NET_DVR_SEND_PARAM_IN m_struSendParam;
    memset(&m_struSendParam, 0, sizeof(m_struSendParam));
    BYTE    *pSendAppendData = nullptr;
    BYTE    *pSendPicData = nullptr;
    //读图片文件
    //D:/Hikvision/Camera/stranger/3977.jpg
    //D:/Hikvision/Camera/stranger/3962.jpg
    qDebug() << "szPicFileName: " << szPicFileName;
    QFileInfo fileinfo(szPicFileName);
    DWORD picFileSize = fileinfo.size();
    pSendPicData = new BYTE[picFileSize];
    QFile picFile(szPicFileName);
    if(!picFile.open(QIODevice::ReadOnly)) {
        qDebug()<<"Can't open the pic file!"<<endl;
        return false;
    }
    QByteArray qbt = picFile.readAll();
    picFile.close();
    pSendPicData = reinterpret_cast<byte*>(qbt.data());

    //qDebug() << "picFileSize: " << picFileSize;
    m_struSendParam.pSendData = pSendPicData;
    m_struSendParam.dwSendDataLen = picFileSize;
    m_struSendParam.byPicType = 1;

    //读XML文件
    QFileInfo xmlinfo(szXMLFileName);
    DWORD xmlFileSize = xmlinfo.size();
    pSendAppendData = new BYTE[xmlFileSize];
    QFile xmlFile(szXMLFileName);
    if(!xmlFile.open(QIODevice::ReadOnly)) {
        qDebug()<<"Can't open the xml file!"<<endl;
        return false;
    }
    QByteArray xmlArray = xmlFile.readAll();
    pSendAppendData = (byte*)(xmlArray.data());
    m_struSendParam.pSendAppendData = pSendAppendData;
    m_struSendParam.dwSendAppendDataLen = xmlFileSize;
    //上传文件
    LONG flag = NET_DVR_UploadSend(m_lUploadHandle, &m_struSendParam, NULL);
    /*qDebug() << "flag: " << flag;
    qDebug() << "errorCode: " << NET_DVR_GetLastError();*/
    LONG iStatus = -1;
    while(1) {
        DWORD dwProgress = 0;
        iStatus = NET_DVR_GetUploadState(m_lUploadHandle, &dwProgress);
        if (1 == iStatus) {
            /*NET_DVR_UPLOAD_FILE_RET m_struPicRet;
            memset(&m_struPicRet, 0, sizeof(m_struPicRet));
            NET_DVR_GetUploadResult(m_lUploadHandle, &m_struPicRet, sizeof(m_struPicRet));
            qDebug() << "sUrl: " << (char*)(m_struPicRet.sUrl);*/
            break;
        } else if(iStatus != 2) {
            break;
        }
       /* else if ((iStatus >= 3 && iStatus <= 10) || iStatus == 31 || iStatus == -1) {
            break;
        }*/
    }
    NET_DVR_UploadClose(m_lUploadHandle);
    return iStatus == 1;
}

void PreviewView::loadPreview() {
    qDebug() << "PreviewView: loadPreview exec";
    //重置图标
    setSymbolPic(QImage(""));

    //---------------------------------------
    //关闭预览
    if (lRealPlayHandle >= 0) {
        NET_DVR_StopRealPlay(lRealPlayHandle);
        lRealPlayHandle = -1;
    }
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();

    if(Config::getCameraInfoIP().length()>0 && Config::getCameraInfoPort()>0 &&
            Config::getCameraInfoUserName().length()>0 && Config::getCameraInfoPassWord().length()>0) {
        qDebug("information is filled");
        //---------------------------------------
        // 初始化
        NET_DVR_Init();
        //设置连接时间与重连时间
        NET_DVR_SetConnectTime(2000, 1);
        NET_DVR_SetReconnect(10000, true);

        //---------------------------------------
        //设置异常消息回调函数
        NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

        //---------------------------------------
        // 注册设备

        //登录参数，包括设备地址、登录用户、密码等
        NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
        struLoginInfo.bUseAsynLogin = 0; //同步登录方式

        strcpy(struLoginInfo.sDeviceAddress, Config::getCameraInfoIP().toLatin1().data()); //设备IP地址
        struLoginInfo.wPort = Config::getCameraInfoPort(); //设备服务端口
        strcpy(struLoginInfo.sUserName, Config::getCameraInfoUserName().toLatin1().data()); //设备登录用户名
        strcpy(struLoginInfo.sPassword, Config::getCameraInfoPassWord().toLatin1().data()); //设备登录密码

        //设备信息, 输出参数
        NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

        PreviewView::lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);

        qDebug()<<"lUserID is "<<lUserID<<endl;

        if (lUserID < 0) {
            printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
            NET_DVR_Cleanup();
            return;
        }

        int *channels = Config::getCameraInfoChannel();
        for(int i = 0; i < 4; i++)
            qDebug() << "previewView.cpp channels: " << channels[i];
        //---------------1-----------------------
        //启动预览并设置回调数据流
        HWND hWnd1 = (HWND)ui->picPreview1->winId();
        NET_DVR_PREVIEWINFO struPlayInfo1 = {0};
        struPlayInfo1.hPlayWnd = hWnd1;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
        struPlayInfo1.lChannel     = channels[0];       //预览通道号
        struPlayInfo1.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
        struPlayInfo1.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
        struPlayInfo1.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流

        //开始播放
        lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo1, NULL, NULL);
        /*if (lRealPlayHandle < 0)
        {
            printf("NET_DVR_RealPlay_V40 error\n");
            NET_DVR_Logout(lUserID);
            NET_DVR_Cleanup();
            //return;
        }*/

        //---------------2-----------------------
        //启动预览并设置回调数据流
        HWND hWnd2 = (HWND)ui->picPreview2->winId();
        NET_DVR_PREVIEWINFO struPlayInfo2 = {0};
        struPlayInfo2.hPlayWnd     = hWnd2;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
        struPlayInfo2.lChannel     = channels[1];       //预览通道号
        struPlayInfo2.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
        struPlayInfo2.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
        struPlayInfo2.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流

        //开始播放
        lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo2, NULL, NULL);

        //---------------3-----------------------
        //启动预览并设置回调数据流
        /*HWND hWnd3 = (HWND)ui->picPreview3->winId();
        NET_DVR_PREVIEWINFO struPlayInfo3 = {0};
        struPlayInfo3.hPlayWnd = hWnd3;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
        struPlayInfo3.lChannel     = channels[2];       //预览通道号
        struPlayInfo3.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
        struPlayInfo3.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
        struPlayInfo3.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流

        //开始播放
        lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo3, NULL, NULL);

        //---------------4-----------------------
        //启动预览并设置回调数据流
        HWND hWnd4 = (HWND)ui->picPreview4->winId();
        NET_DVR_PREVIEWINFO struPlayInfo4 = {0};
        struPlayInfo4.hPlayWnd = hWnd4;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
        struPlayInfo4.lChannel     = channels[3];       //预览通道号
        struPlayInfo4.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
        struPlayInfo4.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
        struPlayInfo4.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流

        //开始播放
        lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo4, NULL, NULL);
        */

        //设置报警回调函数
        NET_DVR_SetDVRMessageCallBack_V31(MessageCallback, NULL);
        //启用布防
        LONG lHandle;
        NET_DVR_SETUPALARM_PARAM  struAlarmParam={0};
        struAlarmParam.dwSize=sizeof(struAlarmParam);
        //其他报警布防参数不需要设置

        lHandle = NET_DVR_SetupAlarmChan_V41(lUserID, & struAlarmParam);
        if (lHandle < 0)
        {
            printf("NET_DVR_SetupAlarmChan_V41 error, %d\n", NET_DVR_GetLastError());
            NET_DVR_Logout(lUserID);
            NET_DVR_Cleanup();
            return;
        }
    }
}


//更新Ui
void PreviewView::showPersonInfo(int option) {
    qDebug() << "PreviewView: showPersonInfo exec";

    /*********************************************显示个人信息******************************************/
    switch(option) {
    case OPTION_FACE_COMPARE:

        if(!alarmInfo.isStranger) {
            QString address = addressInfo.community+" "+addressInfo.building+" "+addressInfo.unit+" "+addressInfo.house;
            setEdPersonInfo(alarmInfo.applicant, alarmInfo.sfzNo, QString::number(alarmInfo.similarity), address);
            setSymbolPic(QImage(":/icon/correct.png", "PNG"));
        } else {
            setEdPersonInfo(QString::fromLocal8Bit("陌生人"), alarmInfo.applicant, UNKNOW, UNKNOW);
            setSymbolPic(QImage(":/icon/error.png", "PNG"));
            setAvatarPic(QImage(""));
        }
        break;

    case OPTION_DOUBLE_CLICK:
        setCapturePic(QImage (dirPicCapture, "JPG"));
        if(!alarmInfo.isStranger) {
            //setFacePic(QImage(dirPicFace, "JPG"));
            setFacePic(QImage(Config::getDirInfoFace().append(alarmInfo.idCapture), "JPG"));
        } else {
            setFacePic(QImage(Config::getDirInfoStranger().append(alarmInfo.idCapture), "JPG"));
        }


        if(!alarmInfo.isStranger) {
            setAvatarPic(QImage(dirPicAvatar, "JPG"));
            setSymbolPic(QImage(":/icon/correct.png", "PNG"));
            QString address = addressInfo.community+" "+addressInfo.building+" "+addressInfo.unit+" "+addressInfo.house;
            setEdPersonInfo(alarmInfo.applicant, alarmInfo.sfzNo, QString::number(alarmInfo.similarity), address);
        } else {
            setAvatarPic(QImage(""));
            setSymbolPic(QImage(":/icon/error.png", "PNG"));
            setEdPersonInfo(QString::fromLocal8Bit("陌生人"), alarmInfo.applicant, UNKNOW, UNKNOW);
        }
        break;
    }
    /*********************************************显示个人信息 END******************************************/
}

/**
 * 双击报警列表后 设置报警信息
 */
void PreviewView::setAlarmInfo() {
    dirPicCapture = dirCapture;
    dirPicCapture.append(alarmInfo.idCapture + ".jpg");
    QString dirPicFace = Config::getDirInfoFace();
    //dirPicFace.append(alarmInfo.idCapture + ".jpg");

    if(!alarmInfo.isStranger) {
        //dirPicFace = dirFace;
        dirPicAvatar = dirAvatar;
        dirPicAvatar.append(alarmInfo.idAvatar + ".jpg");
    } //else if(alarmInfo.isUpload == true) {
      //  dirPicAvatar = dirAvatar;
       // dirPicAvatar.append(alarmInfo.idAvatar + ".jpg");
    //}
    else {
        dirPicFace = Config::getDirInfoStranger();
       // dirPicFace = dirStranger;
    }
    dirPicFace.append(alarmInfo.idCapture + ".jpg");
    emit previewView->showPersonInfo(OPTION_DOUBLE_CLICK);
}

/**
 * @brief PreviewView::addAlarmItem 在报警列表中添加报警信息
 */
void PreviewView::addAlarmItem() {
    qDebug() << "PreviweView: addAlarmItem exec";

    if(!alarmInfo.isStranger) {
        qDebug() << "is not Stranger";
        QIcon icon(":/icon/info.png");
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(alarmText);
        item->setIcon(icon);
        ui->alarmList->insertItem(0, item);
    }
    else {
        qDebug() << "is Stranger";
        QIcon icon(":/icon/warn.png");
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(alarmText);
        item->setIcon(icon);
        ui->alarmList->insertItem(0, item);
    }
}

/**
 * @brief PreviewView::convertUnCharToStr 16进制字符串转字符串
 * @param UnChar 16进制数字
 * @param hexStr 16进制字符串
 * @param str 字符串
 * @param len 长度
 */
void PreviewView::convertUnCharToStr(BYTE *UnChar,char *hexStr, char *str, int len)
{
    //将16进制数据转化为对应的16进制字符
    for(int i = 0; i < len; i++)
    {
        if(UnChar[i]>127) {
            sprintf(hexStr + i * 2, "%02x", UnChar[i]);
        }
    }

    //将16进制字符转化为字符串
    int i,v;
    i=0;
    while (1) {
        if (1!=sscanf(hexStr+i*2,"%2x",&v)) break;
        str[i]=(char)v;
        i++;
    }
    str[i]=0;
}

void PreviewView::saveToDatabase() {
    initDatabase();
    if(!alarmInfo.isStranger) {
        database.addRecord(alarmInfo.applicant, alarmInfo.idCapture, alarmInfo.idAvatar, alarmInfo.idCapture, alarmInfo.isStranger, alarmInfo.similarity);
    } else {
        database.addRecord("", alarmInfo.idCapture, alarmInfo.applicant, alarmInfo.idCapture,  alarmInfo.isStranger, alarmInfo.similarity);
    }
}


void PreviewView::on_alarmList_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug() << "PreviewView: on_alarmList_itemDoubleClicked exec";
    currentRow = ui->alarmList->currentRow();
    //alarmInfo = alarmList[alarmList.size()-1-currentRow];
    alarmInfo = alarmList[searchList[searchList.size()-1-currentRow]];
    addressInfo = database.selectAddress(alarmInfo.applicant, alarmInfo.idAvatar);
    setAlarmInfo();
}

void PreviewView::on_btnAlarmClear_clicked()
{
    //getNET_DVR_STDXMLConfig();
    //uploadStrangerFacePic("2019-03-01m", "D:/Hikvision/Camera/stranger/3962.jpg");
    //initDatabase();
    //qDebug() << "strangerQuantity: " << database.selectStrQuantityByTime(2019, 3, 5);
    //清空报警列表
    ui->alarmList->clear();
    //清空个人信息
    ui->edId->setText("");
    ui->edName->setText("");
    ui->edSimilarity->setText("");
    ui->edAddress->setText("");
    //清空图片
    setCapturePic(QImage(""));
    setAvatarPic(QImage(""));
    setSymbolPic(QImage(""));
    setFacePic(QImage(""));

    alarmList.clear();
}

void PreviewView::showCapturePic(QNetworkReply* reply) {
    qDebug() << "PreviewView:: showCapturePic exec";

    //显示抓拍图片
    QPixmap pix;
    QByteArray bytes = reply->readAll();
    pix.loadFromData(bytes);
    ui->picCapture->setPixmap(pix);

    //抓拍图片路径设置
    dirCapture = Config::getDirCaptureConfig();

    QDir qdirCapture(dirCapture);
    if(!qdirCapture.exists(dirCapture)) {
        qdirCapture.mkpath(dirCapture);
    }

    dirPicCapture = dirCapture;
    dirPicCapture.append(alarmInfo.idCapture + ".jpg");

    //保存抓拍图片文件
    QFile file(dirPicCapture);
    if(file.open(QIODevice::WriteOnly)) {
        file.write(bytes);
        file.close();
    }
}


void PreviewView::showFacePic(QNetworkReply* reply) {
    qDebug() << "PreviewView:: showFacePic exec";
    //显示人脸子图
    QPixmap pix;
    QByteArray bytes = reply->readAll();
    pix.loadFromData(bytes);
    ui->picFace->setPixmap(pix);
    QString dirFace;// = Config::getDirInfoFace();
    //QString dirStranger = Config::getDirInfoStranger();

    //如果是陌生人，将人脸子图存至陌生人文件夹，否则存入人脸子图(face)文件夹
    if(!alarmInfo.isStranger) {
        dirFace = Config::getDirInfoFace();
        qDebug() << "dirFace: " << dirFace;
    } else {
        dirFace = Config::getDirInfoStranger();
        qDebug() << "dirFace: " << dirFace;
    }
    QDir qdirCapture(dirFace);
    if(!qdirCapture.exists(dirFace)) {
        qdirCapture.mkpath(dirFace);
    }
    //dirPicFace = dirFace;
    dirFace.append(alarmInfo.idCapture + ".jpg");

    qDebug() << "dirPicFace: " << dirFace;
    //保存人脸子图文件
    QFile file(dirFace);
    if(file.open(QIODevice::WriteOnly)) {
        file.write(bytes);
        file.close();
    }
}

void PreviewView::showAvatarPic(QNetworkReply* reply) {
    qDebug() << "PreviewView:: showAvatarPic exec";

    //显示人脸图片
    QPixmap pix;
    QByteArray bytes = reply->readAll();
    pix.loadFromData(bytes);
    ui->picAvatar->setPixmap(pix);

    //人脸图片路径设置
    dirAvatar = Config::getDirInfoAvatar();
    QDir qdirAvatar(dirAvatar);
    if(!qdirAvatar.exists(dirAvatar)) {
        qdirAvatar.mkpath(dirAvatar);
    }
    dirPicAvatar = dirAvatar;
    dirPicAvatar.append(alarmInfo.idAvatar + ".jpg");

    qDebug() << "dirPicAvatar:" << dirPicAvatar;

    //保存人脸图片
    QFile file(dirPicAvatar);
    if(file.open(QIODevice::WriteOnly)) {
        file.write(bytes);
        file.close();
    }
}

void PreviewView::on_btnSearch_clicked()
{
    inputName = ui->edSearch->text();
    ui->alarmList->clear();
    searchList.clear();
    qDebug().noquote() << "inputName: " << inputName;

    //重置报警列表数据
    if(inputName.compare("") == 0 || inputName.length() <= 0) {
        isClickSearch = false;
        //获取全部信息
        for(int i = 0; i < alarmList.size(); i++) {
            searchList.append(i);
        }    
    } else if(inputName.compare(QString::fromLocal8Bit("陌生人")) == 0) {
        isClickSearch = true;
        for(int i = 0; i < alarmList.size(); i++) {
            if(alarmList[i].isStranger) {
                 searchList.append(i);
            }
        }     
    } else {
        isClickSearch = true;
        //获取相应信息
        for(int i = 0; i < alarmList.size(); i++) {
            if(inputName.compare(alarmList[i].applicant) == 0 && (!alarmList[i].isStranger)) {
                 searchList.append(i);
            }
        }
    }

    //添加报警列表信息
    for(int i = 0; i < searchList.size(); i++) {
        int index = searchList[i];
        alarmText = QString::asprintf("%4.4d.%2.2d.%2.2d %2.2d:%2.2d:%2.2d   ",
                                      alarmList[index].dwYear,
                                      alarmList[index].dwMonth,
                                      alarmList[index].dwDay,
                                      alarmList[index].dwHour,
                                      alarmList[index].dwMinute,
                                      alarmList[index].dwSecond);
        if(!alarmList[index].isStranger) {
            alarmText.append(QString::fromLocal8Bit("   姓名："));
            alarmText.append(alarmList[index].applicant);

            alarmText.append(QString::fromLocal8Bit("   编号："));
            alarmText.append(alarmList[index].sfzNo);
        } else {
            alarmText.append(QString::fromLocal8Bit("   陌生人"));
        }
        alarmInfo = alarmList[index];
        emit previewView->addAlarmItem();
    }
}
