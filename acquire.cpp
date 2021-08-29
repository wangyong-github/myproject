#include "acquire.h"
#include "QDebug"
#include "QFile"



acquire::acquire(QObject *parent) : QThread(parent)
{

}



void acquire::run()
{
    USB3102A_AI_PARAM AIParam;
    U32 nReadSampsPerChan = 100,
        nSampsPerChanRead = 0;              // 每个AI通道读取的点数    // 每个通道实际读取的点数
    F64 fTimeout = 60.0; // 10秒钟超时
    U32 nAvailSampsPerChan = 0;             // 读操作完成时每通道还可读而未读的数据点数
    HANDLE hDevice = INVALID_HANDLE_VALUE;       // 采集的总数据量(点数)  500x2   nReadSampsPerChan * AIParam.nSampChanCount


    // 第一步 创建设备对象
    hDevice = USB3102A_DEV_Create(0, 0);

    memset(&AIParam, 0, sizeof(USB3102A_AI_PARAM));          // 结构体清零

    // 通道参数
    AIParam.nSampChanCount		= 4;                           // AI采集4个通道
    for(int nChannel=0; nChannel<USB3102A_AI_MAX_CHANNELS; nChannel++)             // 通道结构体设置
    {
        AIParam.CHParam[nChannel].nChannel		= nChannel;
        AIParam.CHParam[nChannel].nSampleRange	= USB3102A_AI_SAMPRANGE_N10_P10V;             // 采样范围+-5V
        AIParam.CHParam[nChannel].nRefGround	= USB3102A_AI_REFGND_RSE;                    // 单端输入，有参考地
        AIParam.CHParam[nChannel].nReserved0	= 0;
        AIParam.CHParam[nChannel].nReserved1	= 0;
        AIParam.CHParam[nChannel].nReserved2	= 0;
    }

    AIParam.nSampleSignal			= USB3102A_AI_SAMPSIGNAL_AI;

    // 时钟参数
    AIParam.nSampleMode				= USB3102A_AI_SAMPMODE_CONTINUOUS;   // 连续采样
    AIParam.nSampsPerChan			= 100;                              // AI每通道待读取点数       // 连续采样模式下该参数对应着触发hSampEvent条件的点数 大于该点数时触发采样事件
    AIParam.fSampleRate				= 8000;                           // 采样率
    AIParam.nClockSource			= USB3102A_AI_CLKSRC_LOCAL;
    AIParam.bClockOutput			= FALSE;

    // 开始触发参数
    AIParam.bDTriggerEn				= TRUE;
    AIParam.nDTriggerDir			= USB3102A_AI_TRIGDIR_FALLING;
    AIParam.bATriggerEn				= TRUE;
    AIParam.nATriggerDir			= USB3102A_AI_TRIGDIR_FALLING;
    AIParam.nATrigChannel			= 0;
    AIParam.fTriggerLevel			= 0;
    AIParam.nTriggerSens			= 5;
    AIParam.nDelaySamps				= 0;


    // 其他参数
    AIParam.nReserved1				= 0;
    AIParam.nReserved2				= 0;
    AIParam.nReserved3				= 0;

    if(!USB3102A_AI_VerifyParam(hDevice, &AIParam))     {   }


    // 第二步 初始化AI采集任务
    if(!USB3102A_AI_InitTask(hDevice, &AIParam, NULL))   {   }


    // 第三步 开始AI采集任务
    if(!USB3102A_AI_StartTask(hDevice))       {   }


    // 第四步 发送软件触发事件(硬件外触发时不需要)
    if(!USB3102A_AI_SendSoftTrig(hDevice))    {   }



    /////////////////////////////////// 初始化保存路径
    QFile file(filepath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setRealNumberPrecision(4);


    // 第五步 循环读取AI数据
     while(flag)
  {

      if(!USB3102A_AI_ReadAnalog(hDevice, fAnlgArray, nReadSampsPerChan, &nSampsPerChanRead, &nAvailSampsPerChan, fTimeout))    {    }   /////// 数据结果

      if (saveflag)
     {

            for(U32 nIndex=0; nIndex<100; nIndex=nIndex+10)          //这里设置为缓存数组的一半，即采样点个数
            {
            for(U32 nChannel=0; nChannel<AIParam.nSampChanCount; nChannel++){
            out <<"AI"<<nChannel<<"="<< fAnlgArray[nChannel+nIndex*AIParam.nSampChanCount]<<"(V)"<<"  ";
            }
            out<<"\n";  }
     }

 }



    // 第六步 停止AI采集任务
    if(!USB3102A_AI_StopTask(hDevice))        {       }

    // 第七步 释放AI采集任务
    if(!USB3102A_AI_ReleaseTask(hDevice))     {       }

    // 第八步 释放设备对象
    if(!USB3102A_DEV_Release(hDevice))        {       }


  emit acquireDone();
  file.close();
}




