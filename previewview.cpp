#include "previewview.h"
#include "ui_previewview.h"

using namespace std;

//时间解析宏定义
#define GET_YEAR(_time_)      (((_time_)>>26) + 2000)
#define GET_MONTH(_time_)     (((_time_)>>22) & 15)
#define GET_DAY(_time_)       (((_time_)>>17) & 31)
#define GET_HOUR(_time_)      (((_time_)>>12) & 31)
#define GET_MINUTE(_time_)    (((_time_)>>6)  & 63)
#define GET_SECOND(_time_)    (((_time_)>>0)  & 63)

const int OPTION_FACE_COMPARE = 0;
const int OPTION_DOUBLE_CLICK = 1;

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
double PreviewView::SIMILARITY;
//保存路径
QString PreviewView::dirAvatar;
QString PreviewView::dirCapture;
QString PreviewView::dirPicAvatar;
QString PreviewView::dirPicCapture;
//报警信息
QList<ALARM_INFO> PreviewView::alarmList;
ALARM_INFO PreviewView::alarmInfo;
QString PreviewView::alarmText;
//数据库
//Database PreviewView::database;
//信息列表
QList<char*> PreviewView::avatarList;
QList<char*> PreviewView::captureList;
QString PreviewView::currentAlarmInfo;
//URL
QString PreviewView::urlCapture;
QString PreviewView::urlAvatar;

NET_VCA_FACESNAP_MATCH_ALARM PreviewView::struFaceMatchAlarm = {0};

PreviewView* PreviewView::previewView = nullptr;

PreviewView::PreviewView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewView)
{
    previewView = this;
    ui->setupUi(this);

    //ui->picSymbol->raise();

    loadPreview();


}



PreviewView::~PreviewView()
{
    delete ui;
}



void CALLBACK PreviewView::g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = { 0 };
    switch (dwType)
    {
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


    switch(lCommand)
    {
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

/**
 * @brief PreviewView::setAlarmInfo
 * @param struFaceMatchAlarm
 */
void PreviewView::setAlarmInfo(NET_VCA_FACESNAP_MATCH_ALARM struFaceMatchAlarm) {
    qDebug() << "PreviewView: setAlarmInfo start";

    QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);
    QString CMUsername = config->value("/Camera/username").toString();
    QString CMPassword = config->value("/Camera/password").toString();
    SIMILARITY = config->value("/Compare/similarity").toFloat();

    /***********************************************设置时间********************************************/
    alarmInfo.dwYear = GET_YEAR(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    alarmInfo.dwMonth = GET_MONTH(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    alarmInfo.dwDay = GET_DAY(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    alarmInfo.dwHour = GET_HOUR(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    alarmInfo.dwMinute = GET_MINUTE(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    alarmInfo.dwSecond = GET_SECOND(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
    /*********************************************设置时间 END******************************************/

    /*********************************************设置个人信息******************************************/
    if(struFaceMatchAlarm.fSimilarity > SIMILARITY) {

        //--------------------
        //设置不为陌生人
        alarmInfo.isStranger = false;

        //--------------------
        //相似度
        alarmInfo.similarity = struFaceMatchAlarm.fSimilarity;

        //--------------------
        //人脸库头像图
        alarmInfo.idAvatar = QString::fromLocal8Bit((char*)struFaceMatchAlarm.struBlackListInfo.pPID);
        int idAvatarCutIndex = alarmInfo.idAvatar.indexOf("<FDDescription>");
        alarmInfo.idAvatar = alarmInfo.idAvatar.mid(0, idAvatarCutIndex);

        avatar = (char*)struFaceMatchAlarm.struBlackListInfo.pBuffer1;
        int avatarCutIndex = QString(avatar).indexOf("http://",1);
        urlAvatar = QString(avatar).mid(0, avatarCutIndex);
        qDebug() << "urlAvatar is " << urlAvatar;


        QEventLoop eventLoop;
        QNetworkAccessManager *manager = new QNetworkAccessManager();

        QUrl url(urlAvatar);

        url.setUserName(CMUsername);
        url.setPassword(CMPassword);
        QNetworkReply* reply = manager->get(QNetworkRequest(url));
        connect(manager, SIGNAL(finished(QNetworkReply*)), previewView, SLOT(showAvatarPic(QNetworkReply*)));
        connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        eventLoop.exec();

        //--------------------
        //姓名
        char nameHexStr[32];
        BYTE nameBytes[NAME_LEN];
        for(int i =0 ;i<NAME_LEN; i++) {
            nameBytes[i] = struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.byName[i];
        }
        convertUnCharToStr(nameBytes, nameHexStr, alarmInfo.name, sizeof(nameBytes));

        //--------------------
        //性别
        switch(struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.bySex) {
        case 0x0:
            strcpy(alarmInfo.sex, "男");
            break;
        case 0x1:
            strcpy(alarmInfo.sex, "女");
            break;
        case 0xff:
            strcpy(alarmInfo.sex, "未知");
            break;
        }


        //--------------------
        //编号（不得以0结尾）
        /*for(int i = 0; i < NAME_LEN; i++) {
                QString strTmp;
                bool zero = true;//当前元素的后续元素是否都为0
                for(int j = 0;j < NAME_LEN-i;j++) {
                    //若存在后续元素不为0，则zero为false
                    if(struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.byCertificateNumber[i+j]!=0) {
                        bool zero = false;
                    }
                }
                //后续元素不全为0，则赋值
                if(!zero) {
                    strTmp.sprintf("%d", struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.byCertificateNumber[i]);
                    alarmInfo.id.append(strTmp);
                }
            }*/
        BYTE idBytes[NAME_LEN];
        memcpy(idBytes, struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.byCertificateNumber, NAME_LEN);
        alarmInfo.id = QString::fromLocal8Bit((char*)idBytes);
        if(alarmInfo.id.length()==0) {
            alarmInfo.id = QString::fromLocal8Bit("未知");
        }

    } else {

        //--------------------
        //设置为陌生人
        alarmInfo.isStranger = true;
        //--------------------
        //相似度
        alarmInfo.similarity = struFaceMatchAlarm.fSimilarity;
        qDebug() << "alarmInfo.similarity is " << QString::number(struFaceMatchAlarm.fSimilarity);


        /*switch(struFaceMatchAlarm.struSnapInfo.bySex) {
        case 0x0:
            strcpy(alarmInfo.sex, "男");
            break;
        case 0x1:
            strcpy(alarmInfo.sex, "女");
            break;
        case 0xff:
            strcpy(alarmInfo.sex, "未知");
            break;
        }*/

    }

    Sleep(300);
    //--------------------
    //抓拍图
    alarmInfo.idCapture = QString::number(struFaceMatchAlarm.struSnapInfo.dwSnapFacePicID);
    //captureLen = struFaceMatchAlarm.dwSnapPicLen;
    //capture = (char*)malloc(struFaceMatchAlarm.dwSnapPicLen);
    //memcpy(capture, struFaceMatchAlarm.pSnapPicBuffer, captureLen);

    capture = (char*)struFaceMatchAlarm.pSnapPicBuffer;
    int captureCutIndex = QString(capture).indexOf("SEl");
    urlCapture = QString(capture).mid(0, captureCutIndex);
    qDebug() << "urlCapture is " << urlCapture;

    QEventLoop eventLoop;
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QUrl url(urlCapture);

    url.setUserName(CMUsername);
    url.setPassword(CMPassword);
    QNetworkReply* reply = manager->get(QNetworkRequest(url));
    connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    connect(manager, SIGNAL(finished(QNetworkReply*)), previewView, SLOT(showCapturePic(QNetworkReply*)));
    eventLoop.exec();

    //显示个人信息
    emit previewView->showPersonInfo(OPTION_FACE_COMPARE);

    alarmList.append(alarmInfo);




    /*if(struFaceMatchAlarm.fSimilarity>SIMILARITY) {
            //人脸库头像图
            avatarLen = struFaceMatchAlarm.struBlackListInfo.dwBlackListPicLen;
            avatar = (char*)malloc(struFaceMatchAlarm.struBlackListInfo.dwBlackListPicLen);
            memcpy(avatar, avatarList[index], avatarLen);

            //抓拍图
            captureLen = struFaceMatchAlarm.dwSnapPicLen;
            capture = (char*)malloc(struFaceMatchAlarm.dwSnapPicLen);
            memcpy(capture, captureList[index], captureLen);
        } else {
            //抓拍图
            captureLen = struFaceMatchAlarm.dwSnapPicLen;
            capture = (char*)malloc(struFaceMatchAlarm.dwSnapPicLen);
            memcpy(capture, captureList[index], captureLen);
        }*/

    /*********************************************设置个人信息 END******************************************/

}



void PreviewView::setAlarmText() {


    //报警信息
    alarmText = QString::asprintf("%d   %4.4d.%2.2d.%2.2d %2.2d:%2.2d:%2.2d   ",
                                  alarmList.length(),
                                  alarmInfo.dwYear,
                                  alarmInfo.dwMonth,
                                  alarmInfo.dwDay,
                                  alarmInfo.dwHour,
                                  alarmInfo.dwMinute,
                                  alarmInfo.dwSecond);
    if(!alarmInfo.isStranger) {

        alarmText.append(QString::fromLocal8Bit("   姓名："));
        alarmText.append(QString::fromLocal8Bit(alarmInfo.name));

        alarmText.append(QString::fromLocal8Bit("   性别："));
        alarmText.append(QString::fromLocal8Bit(alarmInfo.sex));

        alarmText.append(QString::fromLocal8Bit("   编号："));
        alarmText.append(alarmInfo.id);

    } else {

        alarmText.append(QString::fromLocal8Bit("   陌生人"));
    }

    emit previewView->addAlarmItem();

}


void PreviewView::loadPreview() {

    //---------------------------------------
    //关闭预览
    if (lRealPlayHandle >= 0) {
        NET_DVR_StopRealPlay(lRealPlayHandle);
        lRealPlayHandle = -1;
    }
    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();

    //---------------------------------------
    //配置信息
    QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);

    QString ip = config->value("/Camera/ip").toString();
    int port = config->value("/Camera/port").toInt();
    int channel = config->value("/Camera/channel").toInt();
    QString username = config->value("/Camera/username").toString();
    QString password = config->value("/Camera/password").toString();


    if(ip.length()>0 && port>0 && username.length()>0 && password.length()>0) {
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
        strcpy(struLoginInfo.sDeviceAddress, ip.toLatin1().data()); //设备IP地址
        struLoginInfo.wPort = port; //设备服务端口
        strcpy(struLoginInfo.sUserName, username.toLatin1().data()); //设备登录用户名
        strcpy(struLoginInfo.sPassword, password.toLatin1().data()); //设备登录密码

        //设备信息, 输出参数
        NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

        PreviewView::lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);

        qDebug()<<"lUserID is "<<lUserID<<endl;

        if (lUserID < 0)
        {
            printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
            NET_DVR_Cleanup();
            return;
        }

        //---------------------------------------
        //启动预览并设置回调数据流
        HWND hWnd = (HWND)ui->picPreview->winId();
        NET_DVR_PREVIEWINFO struPlayInfo = {0};
        struPlayInfo.hPlayWnd = hWnd;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
        struPlayInfo.lChannel     = channel;       //预览通道号
        struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
        struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
        struPlayInfo.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流

        //开始播放
        lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL);
        if (lRealPlayHandle < 0)
        {
            printf("NET_DVR_RealPlay_V40 error\n");
            NET_DVR_Logout(lUserID);
            NET_DVR_Cleanup();
            return;
        }


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

            ui->edName->setText(QString::fromLocal8Bit(alarmInfo.name));
            ui->edSex->setText(QString::fromLocal8Bit(alarmInfo.sex));
            ui->edId->setText(alarmInfo.id);
            ui->edSimilarity->setText(QString::number(alarmInfo.similarity*100));

            QImage imgCapture(":/icon/info.png", "PNG");
            QPixmap pixCapture = QPixmap::fromImage(imgCapture);
            ui->picSymbol->setPixmap(pixCapture.scaled(ui->picSymbol->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));

        } else {

            ui->edName->setText(QString::fromLocal8Bit("未知"));
            ui->edSex->setText(QString::fromLocal8Bit("未知"));
            ui->edId->setText(QString::fromLocal8Bit("未知"));
            ui->edSimilarity->setText(QString::fromLocal8Bit("未知"));

            QImage imgCapture(":/icon/warn.png", "PNG");
            QPixmap pixCapture = QPixmap::fromImage(imgCapture);
            ui->picSymbol->setPixmap(pixCapture.scaled(ui->picSymbol->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));

        }
        break;

    case OPTION_DOUBLE_CLICK:

        QImage imgCapture(dirPicCapture, "JPG");
        QPixmap pixCapture = QPixmap::fromImage(imgCapture);
        ui->picCapture->setPixmap(pixCapture.scaled(ui->picCapture->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));

        if(!alarmInfo.isStranger) {

            QImage imgAvatar(dirPicAvatar, "JPG");
            QPixmap pixAvatar = QPixmap::fromImage(imgAvatar);
            ui->picAvatar->setPixmap(pixAvatar.scaled(ui->picAvatar->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));

            ui->edName->setText(QString::fromLocal8Bit(alarmInfo.name));
            ui->edSex->setText(QString::fromLocal8Bit(alarmInfo.sex));
            ui->edId->setText(alarmInfo.id);
            ui->edSimilarity->setText(QString::number(alarmInfo.similarity*100));

        } else {

            ui->edName->setText(QString::fromLocal8Bit("未知"));
            ui->edSex->setText(QString::fromLocal8Bit("未知"));
            ui->edId->setText(QString::fromLocal8Bit("未知"));
            ui->edSimilarity->setText(QString::fromLocal8Bit("未知"));

            QImage imgAvatar("");
            QPixmap pixAvatar = QPixmap::fromImage(imgAvatar);
            ui->picAvatar->setPixmap(pixAvatar);

        }
        break;
    }

    /*********************************************显示个人信息 END******************************************/

}

void PreviewView::setAlarmInfo() {

    dirPicCapture = dirCapture;
    dirPicCapture.append(alarmInfo.idCapture);
    dirPicCapture.append(".jpg");

    if(!alarmInfo.isStranger) {
        dirPicAvatar = dirAvatar;
        dirPicAvatar.append(alarmInfo.idAvatar);
        dirPicAvatar.append(".jpg");
    } else {

    }

    emit previewView->showPersonInfo(OPTION_DOUBLE_CLICK);
}




/**
 * @brief PreviewView::addAlarmItem 在报警列表中添加报警信息
 */
void PreviewView::addAlarmItem() {
    qDebug() << "PreviweView: addAlarmItem exec";

    if(!alarmInfo.isStranger) {
        qDebug() << "is Stranger";
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

    Database database = Database();
    QSqlDatabase qSqlDatabase = QSqlDatabase::addDatabase("QMYSQL");
    database.setQSqlDatabase(qSqlDatabase);
    database.openConnect();

    if(!alarmInfo.isStranger) {
        database.addRecord(alarmInfo.name, alarmInfo.sex, alarmInfo.id, alarmInfo.idCapture, alarmInfo.idAvatar, alarmInfo.isStranger, alarmInfo.similarity);
    } else {
        database.addRecord("", "", "", alarmInfo.idCapture, "", alarmInfo.isStranger, alarmInfo.similarity);
    }
    database.closeConnect();

}


void PreviewView::on_alarmList_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug() << "PreviewView: on_alarmList_itemDoubleClicked exec";


    currentRow = ui->alarmList->currentRow();
    alarmInfo = alarmList[alarmList.size()-1-currentRow];

    setAlarmInfo();
}

void PreviewView::on_btnAlarmClear_clicked()
{

    ui->alarmList->clear();

    ui->edId->setText("");
    ui->edName->setText("");
    ui->edSex->setText("");
    ui->edSimilarity->setText("");

    QImage imgCapture("");
    QPixmap pixCapture = QPixmap::fromImage(imgCapture);
    ui->picCapture->setPixmap(pixCapture);

    QImage imgAvatar("");
    QPixmap pixAvatar = QPixmap::fromImage(imgAvatar);
    ui->picAvatar->setPixmap(pixAvatar);

    alarmList.clear();
    avatarList.clear();
    captureList.clear();
}

void PreviewView::showCapturePic(QNetworkReply* reply) {
    qDebug() << "PreviewView:: showCapturePic exec";

    //显示抓拍图片
    QPixmap pix;
    QByteArray bytes = reply->readAll();
    pix.loadFromData(bytes);
    ui->picCapture->setPixmap(pix);

    //抓拍图片路径设置
    QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);
    dirCapture = config->value("/Dir/dirCapture").toString();

    QDir qdirCapture(dirCapture);
    if(!qdirCapture.exists(dirCapture)) {
        qdirCapture.mkpath(dirCapture);
    }

    dirPicCapture = dirCapture;
    dirPicCapture.append(alarmInfo.idCapture);
    dirPicCapture.append(".jpg");

    //保存抓拍图片文件
    QFile file(dirPicCapture);
    if(file.open(QIODevice::WriteOnly)) {
        file.write(bytes);
        file.close();
    }
    /*
    PicThread *picThread = new PicThread(dirPicCapture, bytes);
    picThread->start();
    */
}

void PreviewView::showAvatarPic(QNetworkReply* reply) {
    qDebug() << "PreviewView:: showAvatarPic exec";

    //显示人脸图片
    QPixmap pix;
    QByteArray bytes = reply->readAll();
    pix.loadFromData(bytes);
    ui->picAvatar->setPixmap(pix);

    //人脸图片路径设置
    QSettings *config = new QSettings(":/config/config.ini", QSettings::IniFormat);
    dirAvatar = config->value("/Dir/dirAvatar").toString();

    QDir qdirAvatar(dirAvatar);
    if(!qdirAvatar.exists(dirAvatar)) {
        qdirAvatar.mkpath(dirAvatar);
    }
    dirPicAvatar = dirAvatar;
    dirPicAvatar.append(alarmInfo.idAvatar);
    dirPicAvatar.append(".jpg");


    qDebug() << "dirPicAvatar:" << dirPicAvatar;

    //保存人脸图片
    QFile file(dirPicAvatar);
    if(file.open(QIODevice::WriteOnly)) {
        file.write(bytes);
        file.close();
    }
    /*PicThread *picThread = new PicThread(dirPicAvatar, bytes);
    picThread->start();*/

    delete config;
}

