#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include "HCNetSDK.h"
#include "plaympeg4.h"
#include <stdio.h>
#include <iostream>
#include <time.h>
#include "Windows.h"
#include <QBuffer>
#include <qimage.h>
#include <qimagereader.h>

using namespace std;

typedef HWND (WINAPI *PROCGETCONSOLEWINDOW)();
PROCGETCONSOLEWINDOW GetConsoleWindowAPI;

//时间解析宏定义
#define GET_YEAR(_time_)      (((_time_)>>26) + 2000)
#define GET_MONTH(_time_)     (((_time_)>>22) & 15)
#define GET_DAY(_time_)       (((_time_)>>17) & 31)
#define GET_HOUR(_time_)      (((_time_)>>12) & 31)
#define GET_MINUTE(_time_)    (((_time_)>>6)  & 63)
#define GET_SECOND(_time_)    (((_time_)>>0)  & 63)

static WORD m_wPicSize = 0;//预览画面大小设置
static WORD m_wPicQuality = 1;//预览画面质量设置
static LPCSTR AppName = "HikvisionCamera";//应用名称
static LONG lUserID;//用户句柄
static LONG lRealPlayHandle;//播放句柄
static HWND hMain;//主窗口句柄
static BOOL isLogin = false;//登录状态

static char name[32];
static char sex[2];
static char certificateType[8];
static char birthDate[MAX_HUMAN_BIRTHDATE_LEN];
static char certificateNumber[NAME_LEN];
static char ageGroup[8];
static char nativePlace[256];
static char* avatar;
static DWORD avatarLen;

NET_VCA_FACESNAP_MATCH_ALARM struFaceMatchAlarm = {0};
MainWindow *MainWindow::pMainWindow = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pMainWindow = this;
    connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(clickBtnLogin()));
    connect(this, SIGNAL(signalPersonInfo()), this, SLOT(setPersonInfo()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPersonInfo() {
    if(struFaceMatchAlarm.struBlackListInfo.dwBlackListPicLen>0){
        QByteArray bytearray = QByteArray(avatar, avatarLen);

        QBuffer buffer(&bytearray);
        buffer.open(QIODevice::ReadOnly);

        QImageReader reader(&buffer, "JPG");
        QImage img = reader.read();

        QPixmap pix = QPixmap::fromImage(img);
        ui->labAvatar->setPixmap(pix.scaled(ui->labAvatar->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else {
        ui->labAvatar->setText(QString::fromLocal8Bit("图片缺失"));
    }


    ui->edName->setText(QString::fromLocal8Bit(name));
    ui->edSex->setText(QString::fromLocal8Bit(sex));
    ui->edCertificateType->setText(QString::fromLocal8Bit(certificateType));
    ui->edAgeGroup->setText(QString::fromLocal8Bit(ageGroup));


    /*QPixmap qPixmap;
    qPixmap.loadFromData((const char*)struFaceMatchAlarm.struBlackListInfo.pBuffer1);
    qDebug()<<"image data"<<struFaceMatchAlarm.struBlackListInfo.pBuffer1;
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(qPixmap);

    ui->picAvatar->setScene(scene);*/

}

void convertUnCharToStr(BYTE *UnChar,char *hexStr, char *str, int len)
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



void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
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

BOOL CALLBACK MainWindow::MessageCallback(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void* pUser)
{
    emit pMainWindow->signalPersonInfo();
    qDebug("Callback start");
    switch(lCommand)
    {
    case COMM_SNAP_MATCH_ALARM: //人脸比对结果信息
    {
        qDebug("Detected face");
        memcpy(&struFaceMatchAlarm, pAlarmInfo, sizeof(NET_VCA_FACESNAP_MATCH_ALARM));

        NET_DVR_TIME struAbsTime = {0};
        struAbsTime.dwYear = GET_YEAR(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
        struAbsTime.dwMonth = GET_MONTH(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
        struAbsTime.dwDay = GET_DAY(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
        struAbsTime.dwHour = GET_HOUR(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
        struAbsTime.dwMinute = GET_MINUTE(struFaceMatchAlarm.struSnapInfo.dwAbsTime);
        struAbsTime.dwSecond = GET_SECOND(struFaceMatchAlarm.struSnapInfo.dwAbsTime);

        printf("人脸比对结果上传[0x%x]：[%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d]fSimilarity[%f]FaceID[%d]BlackListID[%d]"
               "Sex[%d]Glasses[%d]Age[%d]MatchPicNum[%d]PicTransType[%d]\n", lCommand,
               struAbsTime.dwYear, struAbsTime.dwMonth, struAbsTime.dwDay, struAbsTime.dwHour, struAbsTime.dwMinute,
               struAbsTime.dwSecond, struFaceMatchAlarm.fSimilarity, struFaceMatchAlarm.struSnapInfo.dwSnapFacePicID,
               struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.dwRegisterID, struFaceMatchAlarm.struSnapInfo.bySex,
               struFaceMatchAlarm.struSnapInfo.byGlasses, struFaceMatchAlarm.struSnapInfo.byAge,
               struFaceMatchAlarm.byMatchPicNum, struFaceMatchAlarm.byPicTransType);

        /****************个人信息******************/
        //头像
        avatarLen = struFaceMatchAlarm.struBlackListInfo.dwBlackListPicLen;
        //avatar = (char*)struFaceMatchAlarm.struBlackListInfo.pBuffer1;
        avatar = (char*)malloc(struFaceMatchAlarm.struBlackListInfo.dwBlackListPicLen);
        memcpy(avatar, struFaceMatchAlarm.struBlackListInfo.pBuffer1, struFaceMatchAlarm.struBlackListInfo.dwBlackListPicLen);
        Sleep(1000);
        //姓名
        char nameHexStr[32];
        BYTE nameBytes[NAME_LEN];
        for(int i =0 ;i<NAME_LEN; i++) {
            nameBytes[i] = struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.byName[i];
        }
        convertUnCharToStr(nameBytes, nameHexStr, name, sizeof(nameBytes));
        //性别
        switch(struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.bySex) {
        case 0x0:
            strcpy(sex, "男");
            break;
        case 0x1:
            strcpy(sex, "女");
            break;
        case 0xff:
            strcpy(sex, "未知");
            break;
        default:
            break;

        }
        //证件类型
        switch(struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.byCertificateType) {
        case 0x0:
            strcpy(certificateType, "身份证");
            break;
        case 0x1:
            strcpy(certificateType, "警官证");
            break;
        case 0x2:
            strcpy(certificateType, "军官证");
            break;
        case 0x3:
            strcpy(certificateType, "护照");
            break;
        case 0x4:
            strcpy(certificateType, "其他");
            break;
        case 0xff:
            strcpy(certificateType, "未知");
            break;
        default:
            break;

        }
        //年龄段
        switch(struFaceMatchAlarm.struBlackListInfo.struBlackListInfo.struAttribute.byAgeGroup) {  
        case HUMAN_AGE_GROUP_ENUM::ENUM_AGE_GROUP_INFANT:
            strcpy(ageGroup, "婴幼儿");
            break;
        case HUMAN_AGE_GROUP_ENUM::ENUM_AGE_GROUP_CHILD:
            strcpy(ageGroup, "儿童");
            break;
        case HUMAN_AGE_GROUP_ENUM::ENUM_AGE_GROUP_YOUNGSTER:
            strcpy(ageGroup, "少年");
            break;
        case HUMAN_AGE_GROUP_ENUM::ENUM_AGE_GROUP_ADOLESCENT:
            strcpy(ageGroup, "青少年");
            break;
        case HUMAN_AGE_GROUP_ENUM::ENUM_AGE_GROUP_YOUTH:
            strcpy(ageGroup, "青年");
            break;
        case HUMAN_AGE_GROUP_ENUM::ENUM_AGE_GROUP_PRIME:
            strcpy(ageGroup, "壮年");
            break;
        case HUMAN_AGE_GROUP_ENUM::ENUM_AGE_GROUP_MIDLIFE:
            strcpy(ageGroup, "中年");
            break;
        case HUMAN_AGE_GROUP_ENUM::ENUM_AGE_GROUP_MIDAGE:
            strcpy(ageGroup, "中老年");
            break;
        case HUMAN_AGE_GROUP_ENUM::ENUM_AGE_GROUP_OLD:
            strcpy(ageGroup, "老年");
            break;
        default:
            strcpy(ageGroup, "未知");
            break;
        }
        //身份证号
        



        //以图搜图
        /*NET_DVR_XML_CONFIG_INPUT lpInputParam1 = {0};
        lpInputParam1.lpRequestUrl = (void*)"POST /ISAPI/Intelligent/analysisImage/face";

        //lpInputParam1.lpInBuffer = struFaceMatchAlarm.struSnapInfo.pBuffer1;
        //lpInputParam1.lpInBuffer = struFaceMatchAlarm.pSnapPicBuffer;

        NET_DVR_XML_CONFIG_OUTPUT lpOutput1 = {0};

        if(NET_DVR_STDXMLConfig(lUserID, &lpInputParam1, &lpOutput1)) {
           qDebug("Anaylze success!");
           qDebug()<<"lpOutBuffer is "<<(LPSTR)lpOutput1.lpOutBuffer<<endl;
        } else {
           qDebug("NET_DVR_STDXMLConfig fail");
        }*/



        //保存抓拍图片
        if (struFaceMatchAlarm.dwSnapPicLen > 0 && struFaceMatchAlarm.pSnapPicBuffer  != NULL && struFaceMatchAlarm.byPicTransType == 0)
        {
            qDebug()<<"capture";
            LPCWSTR cFilename = L"D:\\Cloud\\HIKVISION\\SDK\\build-HikvisionCamera2-Desktop_Qt_5_11_2_MSVC2017_64bit-Debug\\debug\\test.jpg";
            HANDLE hFile;
            DWORD dwReturn;

            char chTime[128];
            //sprintf(chTime,"%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",struAbsTime.dwYear,struAbsTime.dwMonth,struAbsTime.dwDay,struAbsTime.dwHour,struAbsTime.dwMinute,struAbsTime.dwSecond);
            //sprintf(cFilename, "FaceSnapPic[%s][%s].jpg",struFaceMatchAlarm.struSnapInfo.struDevInfo.struDevIP.sIpV4, chTime);


            hFile = CreateFileW(cFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile == INVALID_HANDLE_VALUE)
            {
                break;
            }
            qDebug()<<"created";
            WriteFile(hFile, struFaceMatchAlarm.pSnapPicBuffer, struFaceMatchAlarm.dwSnapPicLen, &dwReturn, NULL);  
            qDebug()<<"writed";
            CloseHandle(hFile);
            qDebug()<<"closed";
            hFile = INVALID_HANDLE_VALUE;

            /*FILE *fp;
            void *buffer;
            char *filename = "D:\\Cloud\\HIKVISION\\SDK\\build-HikvisionCamera2-Desktop_Qt_5_11_2_MSVC2017_64bit-Debug\\debug\\test.jpg";

            fp = fopen(filename, "rb");
            fread(buffer, sizeof(struFaceMatchAlarm.dwSnapPicLen), 1, fp);

            lpInputParam1.lpInBuffer = buffer;
            if(NET_DVR_STDXMLConfig(lUserID, &lpInputParam1, &lpOutput1)) {
                qDebug("Anaylze success!");
                qDebug()<<"lpOutBuffer is "<<(LPSTR)lpOutput1.lpOutBuffer<<endl;
            } else {
                qDebug("NET_DVR_STDXMLConfig fail");
            }*/


        }


        //保存黑名单人脸图片
        /*if (struFaceMatchAlarm.struBlackListInfo.dwBlackListPicLen > 0 && struFaceMatchAlarm.struBlackListInfo.pBuffer1 != NULL)
        {
            char cFilename[256] = {0};
            HANDLE hFile;
            DWORD dwReturn;

            char chTime[128];
            sprintf(chTime,"%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",struAbsTime.dwYear,struAbsTime.dwMonth,struAbsTime.dwDay,struAbsTime.dwHour,struAbsTime.dwMinute,struAbsTime.dwSecond);

            sprintf(cFilename, "FaceBlackListPic[%s].jpg", chTime);


            //hFile = CreateFileW(cFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile == INVALID_HANDLE_VALUE)
            {
                break;
            }
            WriteFile(hFile, struFaceMatchAlarm.struBlackListInfo.pBuffer1, struFaceMatchAlarm.struBlackListInfo.dwBlackListPicLen, &dwReturn, NULL);
            CloseHandle(hFile);
            hFile = INVALID_HANDLE_VALUE;
        }
        */

    }
        break;
    default:
        printf("其他报警，报警信息类型: 0x%x\n", lCommand);
        break;
    }

    return TRUE;
}


void MainWindow::clickBtnLogin()
{

    if(!isLogin) {
        qDebug("btnLogin is clicked");

        hMain = (HWND)this->winId();

        //---------------------------------------
        //关闭预览
        if (lRealPlayHandle >= 0) {
            NET_DVR_StopRealPlay(lRealPlayHandle);
            lRealPlayHandle = -1;
        }
        NET_DVR_Logout(lUserID);
        NET_DVR_Cleanup();

        //个人信息
        QString IP = "";
        QString port = "";
        QString username = "";
        QString password = "";

        IP = ui->edIP->text();
        port = ui->edPort->text();
        username = ui->edUsername->text();
        password = ui->edPassword->text();

        if(IP.length()>0 && port.length()>0 && username.length()>0 && password.length()>0) {
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
            // 获取控制台窗口句柄
            LPCWSTR kernelName = L"kernel32";
            HMODULE hKernel32 = GetModuleHandleW(kernelName);
            GetConsoleWindowAPI = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow");

            //---------------------------------------
            // 注册设备

            //登录参数，包括设备地址、登录用户、密码等
            NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
            struLoginInfo.bUseAsynLogin = 0; //同步登录方式
            strcpy(struLoginInfo.sDeviceAddress, IP.toLatin1().data()); //设备IP地址
            int m_Port = port.toInt();
            struLoginInfo.wPort = m_Port; //设备服务端口
            strcpy(struLoginInfo.sUserName, username.toLatin1().data()); //设备登录用户名
            strcpy(struLoginInfo.sPassword, password.toLatin1().data()); //设备登录密码

            //设备信息, 输出参数
            NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

            lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
            qDebug()<<"lUserID is "<<lUserID<<endl;
            if (lUserID < 0)
            {
                printf("Login failed, error code: %d\n", NET_DVR_GetLastError());
                NET_DVR_Cleanup();
                QMessageBox::about(ui->centralWidget, QString::fromLocal8Bit("登录失败"),QString::fromLocal8Bit("您输入的信息有误"));
                return;
            }

            //设置登录状态
            //ui->boxLogin->setTitle("已登录");
            ui->btnLogin->setText(QString::fromLocal8Bit("注销"));
            isLogin = true;

            //---------------------------------------
            //启动预览并设置回调数据流
            HWND hWnd = (HWND)ui->picCamera->winId();
            NET_DVR_PREVIEWINFO struPlayInfo = {0};
            struPlayInfo.hPlayWnd = hWnd;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
            struPlayInfo.lChannel     = 1;       //预览通道号
            struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
            struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
            struPlayInfo.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流

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




        }else {

            QMessageBox::about(ui->centralWidget, QString::fromLocal8Bit("登陆失败"),QString::fromLocal8Bit("请填入完整的信息"));
        }
    }else {
        //---------------------------------------
        //关闭预览
        NET_DVR_StopRealPlay(lRealPlayHandle);
        //注销用户
        NET_DVR_Logout(lUserID);
        //释放SDK资源
        NET_DVR_Cleanup();
        //设置登录状态
        isLogin = false;
        ui->btnLogin->setText(QString::fromLocal8Bit("登录"));

        return;
    }

}



