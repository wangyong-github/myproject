#ifndef _USB3102A_DEVICE_
#define _USB3102A_DEVICE_

#include "windows.h"

// #################### 人性化的数据类型定义 #####################
// Humanized data types definition
typedef char				I8;		// 8位有符号整型数据
typedef unsigned char		U8;		// 8位无符号整型数据
typedef short				I16;	// 16位有符号整型数据
typedef unsigned short		U16;	// 16位无符号整型数据
typedef long				I32;	// 32位有符号整型数据
typedef unsigned long		U32;	// 32位无符号整型数据
typedef __int64				I64;	// 64位有符号整型数据
typedef unsigned __int64	U64;	// 64位无符号整型数据
typedef float				F32;	// 32位浮点数据(单精度，同为labVIEW中的SGL)
typedef double				F64;	// 64位浮点数据(双精度，同为labVIEW中的DBL)

// ########################## 设备功能指标 ########################
#define USB3102A_AI_MAX_CHANNELS		16	// 本卡最多支持16路模拟量单端输入通道
#define USB3102A_AO_MAX_CHANNELS		2	// 本卡最多支持2个模拟量输出通道
#define USB3102A_DIO_MAX_PORTS			2	// 本卡最多支持2个数字量端口
#define USB3102A_DIO_PORT0_MAX_LINES	8	// 数字量端口0支持8条线
#define USB3102A_DIO_PORT1_MAX_LINES	8	// 数字量端口1支持8条线

// ############# AI工作参数结构体USB3102A_AI_PARAM描述 #############
typedef struct _USB3102A_AI_CH_PARAM // AI通道参数结构体
{
	U32 nChannel;			// 通道号[0, 15]，分别表示物理通道号AI0－AI15
	U32 nSampleRange;		// 采样范围(Sample Range)档位选择, 具体见下面常量名定义(此参数在单点采样有效，未此注明的参数则在单点采样模式中无效)
	U32 nRefGround;			// 地参考方式

	U32 nReserved0;			// 保留(未用)
	U32 nReserved1;			// 保留(未用)
	U32 nReserved2;			// 保留(未用)
} USB3102A_AI_CH_PARAM, *PUSB3102A_AI_CH_PARAM;

// AI硬件参数结构体USB3102A_AI_CH_PARAM中的nSampleRange参数所使用的采样范围挡位选项
#define USB3102A_AI_SAMPRANGE_N10_P10V		0 // ±10V
#define USB3102A_AI_SAMPRANGE_N5_P5V			1 // ±5V
#define USB3102A_AI_SAMPRANGE_N2_P2V			2 // ±2V
#define USB3102A_AI_SAMPRANGE_N1_P1V			3 // ±1V

// AI硬件通道参数结构体USB3102A_AI_CH_PARAM中的nRefGround参数所使用的地参考选项
#define USB3102A_AI_REFGND_RSE			0 // 接地参考单端(Referenced Single Endpoint)
#define USB3102A_AI_REFGND_NRSE			1 // 非参考单端(Non Referenced Single Endpoint)
#define USB3102A_AI_REFGND_DIFF			2 // 差分(Differential)

typedef struct _USB3102A_AI_PARAM // 跟整个AI有关的硬件工作参数(简称AI工作参数)
{
	// 通道参数
	U32 nSampChanCount;     // 采样通道总数[1, 16](此参数在单点采样有效，未此注明的参数则在单点采样模式中无效)
	USB3102A_AI_CH_PARAM CHParam[16]; // 通道参数配置(此参数在单点采样有效，未此注明的参数则在单点采样模式中无效)
	U32 nSampleSignal;		// 采样信号(Sample Signal), 详见下面常量定义
	U32 nReserved0;		// 采样范围(Sample Range)档位选择, 具体见下面常量名定义(此参数在单点采样有效，未此注明的参数则在单点采样模式中无效)
	U32 nReserved1;			// 保留字段(暂未定义)
	
	// 时钟参数
	U32 nSampleMode;		// 采样模式, 0=单点采样(按需)， 1、硬件定时单点(暂不支持)，2=有限点采样， 3=连续采样
	U32 nSampsPerChan;		// 每个通道采样点数(也是每通道待读取点数),取值范围为单点采样模式时应等于1，有限点或连续采样时为[2, 1024*1024]
	F64 fSampleRate;		// 采样速率(Sample Rate), 单位为sps，取值范围为[1sps, 250000sps],它是每个采样通道的采样速率, 它与nChannelCount的乘积不能大于250000sps
	U32 nClockSource;		// 时钟源选择, =0:内时钟OSCCLK; =1:外时钟CLKIN,由CN2上的Port1.DIO2(CLKIN)复用输入
	U32 bClockOutput;		// 采样时钟输出允许, =0:表示禁止; =1:表示允许,由CN2上的Port1.DIO3(CLKOUT)复用输出
	U32 nReserved2;			// 保留字段(暂未定义)
	U32 nReserved3;			// 保留字段(暂未定义)

	// 开始触发参数
	U32 bDTriggerEn;		// 数字触发允许(Digital Trigger Enable), =FALSE:表示禁止; =TRUE:表示允许,触发信号由CN2上的Port1.DIO1(DTR)复用输入，因此初始化时会将DIO1的方向强制置为输入
	U32 nDTriggerDir;	    // 数字触发方向(Digital Trigger Direction)
	U32 bATriggerEn;		// 模拟量触发允许(Analog Trigger Enable), =TRUE:表示允许, =FALSE:表示禁止
	U32 nATriggerDir;		// 模拟触发方向(Analog Trigger Direction)
	U32 nATrigChannel;		// 触发通道(Analog Trigger Channel)
	F32 fTriggerLevel;		// 触发电平(Trigger Level)
	U32 nTriggerSens;		// 触发灵敏度(Trigger Sensitive for Digital and Analog trigger),单位：微秒(uS)，取值范围[0, 1638]
	U32 nDelaySamps;		// 触发延迟点数, 单位：采样点，取值范围32位有效[0, 4294967295]，0:Post Trigger, >0:Delay Trigger
	U32 nReserved4;			// 保留字段(暂未定义)
	U32 nReserved5;			// 保留字段(暂未定义)

	// 其他方面
	U32 nReserved6;			// 保留字段(暂未定义)
	U32 nReserved7;			// 保留字段(暂未定义)
	U32 nReserved8;			// 保留字段(暂未定义)
	U32 nReserved9;			// 保留字段(暂未定义)
} USB3102A_AI_PARAM, *PUSB3102A_AI_PARAM;

// AI硬件参数结构体USB3102A_AI_PARAM中的nSampleSignal采样信号所使用的选项
#define USB3102A_AI_SAMPSIGNAL_AI			0 // AI通道输入信号
#define USB3102A_AI_SAMPSIGNAL_0V			1 // 0V(AGND)
#define USB3102A_AI_SAMPSIGNAL_4D096V		2 // 4.096V
#define USB3102A_AI_SAMPSIGNAL_N4D096V		3 // -4.096V
#define USB3102A_AI_SAMPSIGNAL_AO0			4 // AO0
#define USB3102A_AI_SAMPSIGNAL_NAO0			5 // -AO0
#define USB3102A_AI_SAMPSIGNAL_AO1			6 // AO1
#define USB3102A_AI_SAMPSIGNAL_NAO1			7 // -AO1

// AI硬件参数结构体USB3102A_AI_PARAM中的nSampleMode采样模式所使用的选项
#define USB3102A_AI_SAMPMODE_ONE_DEMAND		0 // 软件按需单点采样
#define USB3102A_AI_SAMPMODE_FINITE			2 // 有限点采样
#define USB3102A_AI_SAMPMODE_CONTINUOUS		3 // 连续采样

// AI硬件参数结构体USB3102A_AI_PARAM中的nClockSource时钟源所使用的选项
#define USB3102A_AI_CLKSRC_LOCAL			0 // 本地时钟(通常为本地晶振时钟OSCCLK),也叫内部时钟
#define USB3102A_AI_CLKSRC_CLKIN			1 // 外部时钟(由连接器CN2上的DIO2/CLKIN复用输入)

// AI硬件参数结构体USB3102A_AI_PARAM中的nDTriggerDir/nATriggerDir触发方向所使用的选项
#define USB3102A_AI_TRIGDIR_FALLING			0 // 下降沿/低电平
#define USB3102A_AI_TRIGDIR_RISING			1 // 上升沿/高电平
#define USB3102A_AI_TRIGDIR_CHANGE			2 // 变化(即上下边沿/高低电平均有效)

// #################### AI工作状态结构体USB3102A_AI_STATUS描述 #####################
typedef struct _USB3102A_AI_STATUS
{
	U32 bTaskDone;				// 采集任务完成标志, =TRUE:表示采集任务完成, =FALSE:表示采集任务未完成，正在进行中
	U32 bTriggered;				// 触发标志, =TRUE:表示已被触发, =FALSE:表示未被触发(即正等待触发)

	U32 nTaskState;				// 采集任务状态, =1:正常, 其它值表示有异常情况
	U32 nAvailSampsPerChan;		// 每通道有效点数，只有它大于当前指定读数长度时才能调用AI_ReadAnalog()立即读取指定长度的采样数据
	U32 nMaxAvailSampsPerChan;	// 自AI_StartTask()后每通道出现过的最大有效点数，状态值范围[0, nBufSampsPerChan],它是为监测采集软件性能而提供，如果此值越趋近于1，则表示意味着性能越高，越不易出现溢出丢点的可能
	U32 nBufSampsPerChan;		// 每通道缓冲区大小(采样点数)。
	I64 nSampsPerChanAcquired;	// 每通道已采样点数(自开始采集任务(AI_StartTask())之后所采样的点数)，这个只是给用户的统计数据

	U32 nHardOverflowCnt;		// 硬件溢出计数(在不溢出情况下恒等于0)
	U32 nSoftOverflowCnt;		// 软件溢出计数(在不溢出情况下恒等于0)
	U32 nInitTaskCnt;			// 初始化采集任务的次数(即调用AI_InitTask()的次数)
	U32 nReleaseTaskCnt;		// 释放采集任务的次数(即调用AI_ReleaseTask()的次数)
	U32 nStartTaskCnt;			// 开始采集任务的次数(即调用AI_StartTask()的次数)
	U32 nStopTaskCnt;			// 停止采集任务的次数(即调用AI_StopTask()的次数)
	U32 nTransRate;				// 传输速率, 即每秒传输点数(sps)，作为USB及应用软件传输性能的监测信息

	U32 nReserved0;				// 保留字段(暂未定义)
	U32 nReserved1;				// 保留字段(暂未定义)
	U32 nReserved2;				// 保留字段(暂未定义)
	U32 nReserved3;				// 保留字段(暂未定义)
	U32 nReserved4;				// 保留字段(暂未定义)
} USB3102A_AI_STATUS, *PUSB3102A_AI_STATUS;

// #################### AI主要信息结构体(USB3102A_AI_MAIN_INFO) #######################
typedef struct _USB3102A_AI_MAIN_INFO
{
	U32 nChannelCount;		// AI通道数量
	U32 nSampRangeCount;	// AI采样范围挡位数量
	U32 nSampGainCount;		// AI采样增益挡位数量
	U32 nCouplingCount;		// AI耦合挡位数量
	U32 nImpedanceCount;	// AI阻抗的挡位数量
	U32 nDepthOfMemory;		// AI板载存储器深度(点数)
	U32 nSampResolution;	// AI采样分辨率(如=8表示8Bit; =12表示12Bit; =14表示14Bit; =16表示16Bit)
	U32 nSampCodeCount;		// AI采样编码数量(如256, 4096, 16384, 65536)
	U32 nTrigLvlResolution;	// 触发电平分辨率(如=8表示8Bit; =12表示12Bit; =16表示16Bit)
	U32 nTrigLvlCodeCount;	// 触发电平编码数量(如256, 4096)

	U32 nReserved0;			// 保留字段(暂未定义)
	U32 nReserved1;			// 保留字段(暂未定义)
	U32 nReserved2;			// 保留字段(暂未定义)
	U32 nReserved3;			// 保留字段(暂未定义)
} USB3102A_AI_MAIN_INFO, *PUSB3102A_AI_MAIN_INFO;

// #################### AI采样范围信息结构体(USB3102A_AI_VOLT_RANGE_INFO) #######################
typedef struct _USB3102A_AI_VOLT_RANGE_INFO
{
	U32 nSampleRange;		// 当前采样范围挡位号
	U32 nReserved0;			// 保留字段(暂未定义)
	F64 fMaxVolt;			// 采样范围的最大电压值,单位:伏(V)
	F64 fMinVolt;			// 采样范围的最小电压值,单位:伏(V)
	F64 fAmplitude;			// 采样范围幅度,单位:伏(V)
	F64 fHalfOfAmp;			// 采样范围幅度的二分之一,单位:伏(V)
	F64 fCodeWidth;			// 编码宽度,单位:伏(V), 即每个单位码值所分配的电压值
	F64 fOffsetVolt;		// 偏移电压,单位:伏(V),一般用于零偏校准
	F64 fOffsetCode;		// 偏移码值,一般用于零偏校准,它代表的电压值等价于fOffsetVolt
	char strDesc[16];		// 采样范围字符描述,如"±10V", "0-10V"等

	U32 nPolarity;			// 采样范围的极性(0=双极性BiPolar, 1=单极性UniPolar)
	U32 nCodeCount;			// 原码数量
	I32 nMaxCode;			// 原码最大值
	I32 nMinCode;	        // 原码最小值

	U32 nReserved1;			// 保留字段(暂未定义)
	U32 nReserved2;			// 保留字段(暂未定义)
	U32 nReserved3;			// 保留字段(暂未定义)
	U32 nReserved4;			// 保留字段(暂未定义)
} USB3102A_AI_VOLT_RANGE_INFO, *PUSB3102A_AI_VOLT_RANGE_INFO;

// 结构体AI_VOLT_RANGE_INFO的数据成员Polarity所用的采样范围极性选项
#define USB3102A_AI_POLAR_BIPOLAR	0 // 双极性
#define USB3102A_AI_POLAR_UNIPOLAR	1 // 单极性

// #################### AI速率信息结构体(USB3102A_AI_SAMP_RATE_INFO) #######################
typedef struct _USB3102A_AI_SAMP_RATE_INFO
{
	F64 fMaxRate;		// 单通道最大采样速率(sps)，多通道时各通道平分最大采样率
	F64 fMinRate;		// 单通道最小采样速率(sps)，多通道时各通道平分最小采样率
	F64 fTimerBase;		// 时钟基准（即板载晶振频率）单位：Hz
	U32 nDivideMode;	// 分频模式，0=整数分频(INTDIV), 1=DDS分频(DDSDIV)
	U32 nRateType;		// 速率类型,指fMaxRate和fMinRate的类型, =0:表示为所有采样通道的总速率, =1:表示为每个采样通道的速率

	U32 nReserved0;		// 保留字段(暂未定义)
	U32 nReserved1;		// 保留字段(暂未定义)
} USB3102A_AI_SAMP_RATE_INFO, *PUSB3102A_AI_SAMP_RATE_INFO;

// ##################### AO工作参数结构体USB3102A_AO_PARAM描述 ###################
typedef struct _USB3102A_AO_CH_PARAM // AI通道参数结构体
{
	U32 bChannelEn;			// 通道采样允许
	U32 nSampleRange;		// 采样范围，具体定义请参考下面常量定义
	U32 nReserved0;			// 保留(未用)
	U32 nReserved1;			// 保留(未用)
	U32 nReserved2;			// 保留(未用)
	U32 nReserved3;			// 保留(未用)
} USB3102A_AO_CH_PARAM, *PUSB3102A_AO_CH_PARAM;

// AO硬件参数结构体USB3102A_AO_CH_PARAM中的nSampleRange参数所使用的模拟量输入采样范围挡位选项
#define USB3102A_AO_SAMPRANGE_N10_P10V		0 // ±10V

typedef struct _USB3102A_AO_PARAM // 跟整个AO有关的硬件工作参数(简称AO工作参数)
{
	// 通道参数
	USB3102A_AO_CH_PARAM CHParam[2]; // 通道参数（通道使能，采样范围）

	// 时钟参数
	U32 nSampleMode;		// 采样模式, 0=软件定时单点采样(按需)， 1、硬件定时单点采样(暂不支持)，2=有限点采样， 3=连续采样
	U32 nSampsPerChan;		// 每个通道生成点数(也是每通道待写入点数),取值范围为单点采样模式时应等于1，有限点或连续采样时为[2, 1024*1024]
	F64 fSampleRate;		// 采样速率(Sample Rate), 单位为sps，取值范围为[1sps, 100000sps],它是每个采样通道的采样速率
	U32 nClockSource;		// 时钟源选择, =0:内时钟OSCCLK; =1:外时钟CLKIN,由CN2上的Port1.DIO2(CLKIN)复用输入
	U32 bClockOutput;		// 采样时钟输出允许, =0:表示禁止; =1:表示允许,由CN2上的Port1.DIO3(CLKOUT)复用输出
	U32 bRegenModeEn;		// 波形重生成模式允许(在连续采样模式中有效), =TRUE:只是对开始生成任前写入任务中的波形段数据进行循环重复生成，=FALSE:禁止重生成模式，在开始生成任务后，还要不断的往任务中写入新的波形数据
	U32 nReserved0;			// 保留字段(暂未定义)

	// 开始触发参数
	U32 bDTriggerEn;		// 数字触发DTR允许(Digital Trigger Enable), =FALSE:表示禁止; =TRUE:表示允许,触发信号由CN2上的Port1.DIO1(DTR)复用输入，因此初始化时会将DIO1的方向强制置为输入
	U32 nDTriggerDir;	    // 数字触发方向(Digital Trigger Direction)
	U32 nTriggerSens;		// 触发灵敏度(Trigger Sensitive for Digital trigger),单位：微秒(uS)，取值范围[0, 1638]
	U32 nDelaySamps;		// 触发延迟点数, 单位：采样点，取值范围32位有效[0, 4294967295]，0:Post Trigger, >0:Delay Trigger
	U32 nReserved1;			// 保留字段(暂未定义)
	U32 nReserved2;			// 保留字段(暂未定义)
	
	// 其他参数
	U32 nReserved3;			// 保留字段(暂未定义)
	U32 nReserved4;			// 保留字段(暂未定义)
	U32 nReserved5;			// 保留字段(暂未定义)
	U32 nReserved6;			// 保留字段(暂未定义)
} USB3102A_AO_PARAM, *PUSB3102A_AO_PARAM;

// AO硬件参数结构体USB3102A_AO_PARAM中的nSampleMode采样模式所使用的选项
#define USB3102A_AO_SAMPMODE_ONE_DEMAND	0 // 单点采样(按需)
#define USB3102A_AO_SAMPMODE_ONE_HWTIMED	1 // 单点采样(硬件定时,Hardware Timed, 本设备暂时不支持)
#define USB3102A_AO_SAMPMODE_FINITE		2 // 有限点采样
#define USB3102A_AO_SAMPMODE_CONTINUOUS	3 // 连续采样

// AO硬件参数结构体USB3102A_AO_PARAM中的nClockSource时钟源所使用的选项
#define USB3102A_AO_CLKSRC_LOCAL			0 // 本地时钟(通常为本地晶振时钟OSCCLK),也叫内部时钟
#define USB3102A_AO_CLKSRC_CLKIN			1 // 外部时钟(由连接器CN2上的DIO2/CLKIN复用输入)

// AO硬件参数结构体USB3102A_AO_PARAM中的nDTriggerDir触发方向所使用的选项
#define USB3102A_AO_TRIGDIR_FALLING		0 // 下降沿/低电平
#define USB3102A_AO_TRIGDIR_RISING		1 // 上升沿/高电平
#define USB3102A_AO_TRIGDIR_CHANGE		2 // 变化(即上下边沿/高低电平均有效)

// #################### AO工作状态结构体USB3102A_AO_STATUS描述 #####################
typedef struct _USB3102A_AO_STATUS
{
	U32 bTaskDone;				// 生成任务完成标志, =TRUE:表示生成任务完成, =FALSE:表示生成任务未完成，正在进行中
	U32 bTriggered;				// 触发标志, =TRUE:表示已被触发, =FALSE:表示未被触发(即正等待触发)

	U32 nTaskState;				// 生成任务状态, =1:正常, 其它值表示有异常情况
	U32 nAvailSampsPerChan;		// 可写点数，最好等它大于参数nWriteSampsPerChan时才能调用AO_WriteAnalog()写入采样数据
	U32 nMaxAvailSampsPerChan;	// 自AO_StartTask()后出现过的最大可写点数，状态值范围[0, nBufSampsPerChan],它是为监测采集软件性能而提供，如果此值越趋近于1，则表示意味着性能越高，越不易出现溢出丢点的可能
	U32 nBufSampsPerChan;		// 每通道缓冲区大小(采样点数)。
	U64 nSampsPerChanAcquired;	// 每通道已采样点数(自开始生成任务(AO_StartTask())之后所采样的点数)，这个只是给用户的统计数据

	U32 nHardUnderflowCnt;		// 硬件缓冲下溢次数(在不下溢情况下恒等于0)
	U32 nSoftUnderflowCnt;		// 软件缓冲下溢次数(在不下溢情况下恒等于0)
	U32 nInitTaskCnt;			// 初始化生成任务的次数(即调用AO_InitTask()的次数)
	U32 nReleaseTaskCnt;		// 释放生成任务的次数(即调用AO_ReleaseTask()的次数)
	U32 nStartTaskCnt;			// 开始生成任务的次数(即调用AO_StartTask()的次数)
	U32 nStopTaskCnt;			// 停止生成任务的次数(即调用AO_StopTask()的次数)
	U32 nTransRate;				// 传输速率, 即每秒传输点数(sps)，作为USB及应用软件传输性能的监测信息

	U32 nReserved0;				// 保留字段(暂未定义)
	U32 nReserved1;				// 保留字段(暂未定义)
	U32 nReserved2;				// 保留字段(暂未定义)
	U32 nReserved3;				// 保留字段(暂未定义)
	U32 nReserved4;				// 保留字段(暂未定义)
} USB3102A_AO_STATUS, *PUSB3102A_AO_STATUS;

// #################### AO主要信息结构体(USB3102A_AO_MAIN_INFO) #######################
typedef struct _USB3102A_AO_MAIN_INFO
{
	U32 nChannelCount;		// AO通道数量
	U32 nSampRangeCount;	// AO采样范围挡位数量
	U32 nSampGainCount;		// AO增益挡位数量
	U32 nCouplingCount;		// AO耦合挡位数量
	U32 nImpedanceCount;	// AO阻抗的挡位数量
	U32 nDepthOfMemory;		// AO板载存储器深度(点数)
	U32 nSampResolution;	// AO采样分辨率(如=8表示8Bit; =12表示12Bit; =14表示14Bit; =16表示16Bit)
	U32 nSampCodeCount;		// AO采样编码数量(如256, 4096, 16384, 65536)
	U32 nTrigLvlResolution;	// 触发电平分辨率(如=8表示8Bit; =12表示12Bit; =16表示16Bit)
	U32 nTrigLvlCodeCount;	// 触发电平编码数量(如256, 4096)

	U32 nReserved0;			// 保留字段(暂未定义)
	U32 nReserved1;			// 保留字段(暂未定义)
	U32 nReserved2;			// 保留字段(暂未定义)
	U32 nReserved3;			// 保留字段(暂未定义)
} USB3102A_AO_MAIN_INFO, *PUSB3102A_AO_MAIN_INFO;

// #################### AO采样范围信息结构体(USB3102A_AO_VOLT_RANGE_INFO) #######################
typedef struct _USB3102A_AO_VOLT_RANGE_INFO
{
	U32 nSampleRange;		// 当前采样范围挡位号
	U32 nReserved0;			// 保留字段(暂未定义)
	F64 fMaxVolt;			// 采样范围的最大电压值,单位:伏(V)
	F64 fMinVolt;			// 采样范围的最小电压值,单位:伏(V)
	F64 fAmplitude;			// 采样范围幅度,单位:伏(V)
	F64 fHalfOfAmp;			// 采样范围幅度的二分之一,单位:伏(V)
	F64 fCodeWidth;			// 编码宽度,单位:伏(V), 即每个单位码值所分配的电压值
	F64 fOffsetVolt;		// 偏移电压,单位:伏(V),一般用于零偏校准
	F64 fOffsetCode;		// 偏移码值,一般用于零偏校准,它代表的电压值等价于fOffsetVolt
	char strDesc[16];		// 采样范围字符描述,如"±10V", "0-10V"等

	U32 nPolarity;			// 采样范围的极性(0=双极性BiPolar, 1=单极性UniPolar)
	U32 nCodeCount;			// 原码数量
	I32 nMaxCode;			// 原码最大值
	I32 nMinCode;	        // 原码最小值

	U32 nReserved1;			// 保留字段(暂未定义)
	U32 nReserved2;			// 保留字段(暂未定义)
	U32 nReserved3;			// 保留字段(暂未定义)
	U32 nReserved4;			// 保留字段(暂未定义)
} USB3102A_AO_VOLT_RANGE_INFO, *PUSB3102A_AO_VOLT_RANGE_INFO;

// 结构体AO_VOLT_RANGE_INFO的数据成员Polarity所用的采样范围极性选项
#define USB3102A_AO_POLAR_BIPOLAR	0 // 双极性
#define USB3102A_AO_POLAR_UNIPOLAR	1 // 单极性

// #################### AO速率信息结构体(USB3102A_AO_SAMP_RATE_INFO) #######################
typedef struct _USB3102A_AO_SAMP_RATE_INFO
{
	F64 fMaxRate;		// 单通道最大采样速率(sps)，多通道时各通道平分最大采样率
	F64 fMinRate;		// 单通道最小采样速率(sps)，多通道时各通道平分最小采样率
	F64 fTimerBase;		// 时钟基准（即板载晶振频率）单位：Hz
	U32 nDivideMode;	// 分频模式，0=整数分频(INTDIV), 1=DDS分频(DDSDIV)

	U32 nRateType;		// 速率类型,指fMaxRate和fMinRate的类型, =0:表示为所有采样通道的总速率, =1:表示为每个采样通道的速率
	U32 nReserved1;		// 保留字段(暂未定义)
	U32 nReserved2;		// 保留字段(暂未定义)
} USB3102A_AO_SAMP_RATE_INFO, *PUSB3102A_AO_SAMP_RATE_INFO;

// ###################### CTR工作参数结构体(USB3102A_CTR_PARAM) #######################
typedef struct _USB3102A_CTR_PARAM // 跟整个CTR有关的硬件工作参数(简称CTR工作参数)
{
	U32 nPulseDir;		// 脉冲方向(0=下降沿,1=上升沿, 2=双边沿),脉冲源由Port1.DIO0(CTRSRC)复用输入
	U32 bInitReset;		// 是否在初始化时复位计数器至0，=TRUE:初始化时清零计数器, =FALSE:初始化是不清
	U32 bFullReset;		// 是否在溢出时自动复位计数器至0，=TRUE:溢出时自动复位至0, =FALSE:在溢出时锁住当前计数不变

	U32 nReserved0;		// 保留字段(暂未定义)
	U32 nReserved1;		// 保留字段(暂未定义)
	U32 nReserved2;		// 保留字段(暂未定义)
} USB3102A_CTR_PARAM, *PUSB3102A_CTR_PARAM;

// CTR硬件参数结构体USB3102A_CTR_PARAM中的nPulseDir脉冲方向所使用的选项
#define USB3102A_CTR_PULSEDIR_FALLING	0 // 下边沿
#define USB3102A_CTR_PULSEDIR_RISING		1 // 上边沿
#define USB3102A_CTR_PULSEDIR_CHANGE		2 // 变化(上下边沿均有效)

// #################### DIO主要信息结构体(USB3102A_DIO_PARAM) #######################
typedef struct _USB3102A_DIO_PARAM // 跟整个DIO有关的硬件工作参数(简称DIO工作参数)
{
	U8 bOutputEn[8];	// 输出方向允许(Output Enable),bOutputEn[n]=0:输出禁止(即为输入)，=1:输出允许(即输入无效)。(默认方向为输入)
						// 但与其复用的CTRSRC,DTR,CLKIN,CLKOUT有效时，则与其相应的DIO方向控制自动失效
	
	U32 nReserved0;		// 保留字段(暂未定义)
	U32 nReserved1;		// 保留字段(暂未定义)
	U32 nReserved2;		// 保留字段(暂未定义)
} USB3102A_DIO_PARAM, *PUSB3102A_DIO_PARAM;

// ################################ 函数错误信息 ################################
#define ERROR_NO_AVAILABLE_SAMPS	(0xE0000000+1)	// 无有效点数
#define ERROR_TASK_FAIL				(0xE0000000+2)	// 任务失败
#define ERROR_RATE_OVER_RANGE		(0xE0000000+3)	// 采样速率超限

// ################################ 设备驱动接口申明 ################################
#ifndef _USB3102A_DRIVER_
#define DEVLIB __declspec(dllimport)
#else
#define DEVLIB __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	// ################################ DEV设备对象管理函数 ################################
	HANDLE DEVLIB WINAPI USB3102A_DEV_Create(					// 创建设备对象句柄(hDevice), 成功返回实际句柄,失败则返回INVALID_HANDLE_VALUE(-1),可调用GetLastError()分析错误原因
										U32 nDeviceIdx,			// 设备序号(逻辑序号或物理序号, 具体使用哪种序号由参数bUsePhysIdx决定)
										BOOL bUsePhysIdx);		// 是否使用物理序号, =TRUE:使用物理序号, =FALSE:使用逻辑序号										

	U32 DEVLIB WINAPI USB3102A_DEV_GetCount(void);				// 取得该设备总台数,成功返回值>0, 失败返回0(可调用GetLastError()分析错误原因)

	BOOL DEVLIB WINAPI USB3102A_DEV_GetCurrentIdx(				// 获得指定设备中的逻辑序号和物理序号
										HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
										U32* pLgcIdx,			// 返回的逻辑序号
										U32* pPhysIdx);			// 返回的物理序号

	BOOL DEVLIB WINAPI USB3102A_DEV_GetSpeed(					// 读取设备连接的USB端口速度, 
										HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
										U32* pSpeed);			// USB接口速度，=1:USB1.0, =2:USB2.0, =3:USB3.0


	BOOL DEVLIB WINAPI USB3102A_DEV_Release(HANDLE hDevice);		// 释放设备对象(关键函数)

	// ################################ AI模拟量输入实现函数 ################################
	BOOL DEVLIB WINAPI USB3102A_AI_InitTask(					// 初始化采集任务(Initialize Task)
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									PUSB3102A_AI_PARAM pAIParam, // AI工作参数, 它仅在此函数中决定硬件初始状态和各工作模式,可以事先由AI_VerifyParam()进行参数校验
									HANDLE* pSampEvent);	// 返回采样事件对象句柄,当设备中出现可读数据段时会触发此事件，参数=NULL,表示不需要此事件句柄

    BOOL DEVLIB WINAPI USB3102A_AI_StartTask(				// 开始采集任务
									HANDLE hDevice);		// 设备对象句柄,它由DEV_Create()函数创建

    BOOL DEVLIB WINAPI USB3102A_AI_SendSoftTrig(				// 发送软件触发事件(Send Software Trigger),软件触发也叫强制触发
									HANDLE hDevice);		// 设备对象句柄,它由DEV_Create()函数创建

	BOOL DEVLIB WINAPI USB3102A_AI_GetStatus(HANDLE hDevice, PUSB3102A_AI_STATUS pAIStatus);   // 取得AI各种状态

	BOOL DEVLIB WINAPI USB3102A_AI_WaitUntilTaskDone(		// 采集任务结束前等待,返回TRUE表示采集任务结束
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									F64 fTimeout);			// 用于等待的时间，单位：秒(S)

	BOOL DEVLIB WINAPI USB3102A_AI_ReadAnalog(				// 从采集任务中读取采样数据(电压数据序列)(Read analog data from the task)
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									F64 fAnlgArray[],		// 模拟数据数组(电压数组),用于返回采样的电压数据，取值区间由各通道采样时的采样范围决定(单位:V)
									U32 nReadSampsPerChan,	// 每通道请求读取的点数(单位：点)
									U32* pSampsPerChanRead,	// 返回每通道实际读取的点数(单位：点), =NULL,表示无须返回
									U32* pAvailSampsPerChan,// 任务中还存在的可读点数, =NULL,表示无须返回
									F64 fTimeout);			// 超时时间，单位：秒(S)

	BOOL DEVLIB WINAPI USB3102A_AI_ReadBinary(				// 从采集任务中读取采样数据(原码数据序列)(Read binary data from the task)
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									I16 nBinArray[],		// 二进制数据数组（原码数组）,用于返回采样的原码数据，取值区间为[-32768, 32767]
									U32 nReadSampsPerChan,	// 每通道请求读取的点数(单位：点)
									U32* pSampsPerChanRead,	// 返回每通道实际读取的点数(单位：点), =NULL,表示无须返回
									U32* pAvailSampsPerChan,// 任务中还存在的可读点数, =NULL,表示无须返回
									F64 fTimeout);			// 超时时间，单位：秒(S)

	BOOL DEVLIB WINAPI USB3102A_AI_StopTask(HANDLE hDevice);	// 停止AI采集任务

	BOOL DEVLIB WINAPI USB3102A_AI_ReleaseTask(HANDLE hDevice); // 释放采集任务

	// ========================= AI辅助操作函数 =========================
	BOOL DEVLIB WINAPI USB3102A_AI_GetMainInfo(				// 获得AI的主要信息
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									PUSB3102A_AI_MAIN_INFO pMainInfo); // 获得AI主要信息

	BOOL DEVLIB WINAPI USB3102A_AI_GetVoltRangeInfo(				// 获得采样范围的各种信息(幅度、分辨率、极性)
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									U32 nChannel,			// AI物理通道号(由于本设备所有通道共享一个采样范围选择,故恒等于0)
									U32 nSampleRange,		// 采样范围选择[0, 0]
									PUSB3102A_AI_VOLT_RANGE_INFO pRangeInfo); // 采样范围信息

	BOOL DEVLIB WINAPI USB3102A_AI_GetRateInfo(				// 获得采样速率信息
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									PUSB3102A_AI_SAMP_RATE_INFO pRateInfo); // 采样速率信息

	BOOL DEVLIB WINAPI USB3102A_AI_ScaleBinToVolt(			// 根据指定采样范围将采样原码数据量化成电压数据,返回实际量化的点数
									PUSB3102A_AI_VOLT_RANGE_INFO pRangeInfo, // 当前转换数据需要的采样范围信息
									PVOID pGainInfo,		// 当前转换数据需要的采样增益信息(若于=NULL，表示不使用增益)
									F64 fVoltArray[],		// 电压数据数组,用于返回量化后的电压数据,单位V
									I16 nBinArray[],		// 二进制原码数组,用于传入量化前的原码数据，取值区间为[-32768, 32767], (单指某个通道的连续数据)
									U32 nScaleSamps,		// 请求量化的采样点数
									U32* pSampsScaled);		// 返回实际量化的采样点数, =NULL,表示无须返回

	BOOL DEVLIB WINAPI USB3102A_AI_ScaleVoltToBin(			// 根据指定采样范围将电压数据量化成二进制原码数据,返回实际量化的点数
									PUSB3102A_AI_VOLT_RANGE_INFO pRangeInfo, // 当前转换数据需要的采样范围信息
									PVOID pGainInfo,		// 当前转换数据需要的采样增益信息(若于=NULL，表示不使用增益)
									I16 nBinArray[],		// 原码缓冲区,用于返回量化后的原码数据，取值区间为[-32768, 32767], (单指某个通道的连续数据)
									F64 fVoltArray[],		// 电压缓冲区,用于传入量化前的电压数据,单位V
									U32 nScaleSamps,		// 请求量化的采样点数
									U32* pSampsScaled);		// 返回实际量化的采样点数, =NULL,表示无须返回

	BOOL DEVLIB WINAPI USB3102A_AI_VerifyParam(				// 校验AI工作参数(Verify Parameter),建议在初始化AI参数前调用此函数校验各参数合法性
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									PUSB3102A_AI_PARAM pAIParam);// 待校验的AI工作参数

	BOOL DEVLIB WINAPI USB3102A_AI_LoadParam(HANDLE hDevice, PUSB3102A_AI_PARAM pAIParam); // 从USB3102A.ini中加载AI参数
	BOOL DEVLIB WINAPI USB3102A_AI_SaveParam(HANDLE hDevice, PUSB3102A_AI_PARAM pAIParam); // 保存AI参数至USB3102A.ini
	BOOL DEVLIB WINAPI USB3102A_AI_ResetParam(HANDLE hDevice, PUSB3102A_AI_PARAM pAIParam); // 将当前AI参数复位至出厂值

	// ################################ AO模拟量输出实现函数 ################################
	BOOL DEVLIB WINAPI USB3102A_AO_InitTask(					// 初始化生成任务(Initialize Task)
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									PUSB3102A_AO_PARAM pAOParam, // AO工作参数, 它仅在此函数中决定硬件初始状态和各工作模式,可以事先由AO_VerifyParam()进行参数校验
									HANDLE* pSampEvent);	// 返回采样事件对象句柄,当设备中出现可读数据段时会触发此事件，参数=NULL,表示不需要此事件句柄

    BOOL DEVLIB WINAPI USB3102A_AO_StartTask(				// 开始生成任务 
									HANDLE hDevice);		// 设备对象句柄,它由DEV_Create()函数创建

    BOOL DEVLIB WINAPI USB3102A_AO_SendSoftTrig(				// 发送软件触发事件(Send Software Trigger),软件触发也叫强制触发
									HANDLE hDevice);		// 设备对象句柄,它由DEV_Create()函数创建

	BOOL DEVLIB WINAPI USB3102A_AO_GetStatus(HANDLE hDevice, PUSB3102A_AO_STATUS pAOStatus);   // 取得AO各种状态

	BOOL DEVLIB WINAPI USB3102A_AO_WaitUntilTaskDone(		// 生成任务结束前等待,返回TRUE表示生成任务结束
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									F64 fTimeout);			// 超时时间，单位：秒(S)

	BOOL DEVLIB WINAPI USB3102A_AO_WriteAnalog(				// 向生成任务中写入AO输出的模拟量电压数据(Write analog data to the task)
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									F64 fAnlgArray[],		// AO0、AO1电压混合缓冲区,存放AO的模拟(电压)数据，取值区间由各通道采样时的采样范围决定(单位:V)
									U32 nWriteSampsPerChan,	// 每通道请求写入的点数(单位：点)
									U32* pSampsPerChanWritten,// 返回每通道实际写入的点数(单位：点), =NULL,表示无须返回
									U32* pAvailSampsPerChan,// 返回当前可写入的采样点数, =NULL,表示无须返回
									F64 fTimeout);			// 超时时间，单位：秒(S)

	BOOL DEVLIB WINAPI USB3102A_AO_WriteBinary(				// 向生成任务中写入AO输出的二进制原码数据(Write binary data to the task)
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									I16 nBinArray[],		// AO0、AO1原码混合缓冲区,用于返回采样的原码数据，取值区间为[-32768, 32767]
									U32 nWriteSampsPerChan,	// 每通道请求写入的点数(单位：点), =NULL,表示无须返回
									U32* pSampsPerChanWritten,// 返回每通道实际写入的点数(单位：点), =NULL,表示无须返回
									U32* pAvailSampsPerChan,// 返回当前可写入的采样点数, =NULL,表示无须返回
									F64 fTimeout);			// 超时时间，单位：秒(S)

	BOOL DEVLIB WINAPI USB3102A_AO_ReadbackAnalog(			// 回读所有AO通道的当前生成的模拟量数据(电压数据序列)(Read back analog data from the task)
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									F64 fAnlgArray[2]);		// fAnlgArray[0]=AO0电压数据,fAnlgArray[1]=AO1电压数据, 取值区间由相应通道的采样范围决定

	BOOL DEVLIB WINAPI USB3102A_AO_ReadbackBinary(			// 回读所有AO通道的当前生成的模拟量数据(二进制原码数据序列)(Read back binary data from the task)
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									I16 nBinArray[2]);		// nBinArray[0]=AO0原码数据,nBinArray[1]=AO1原码数据, 取值区间[-32768, 32767]

	BOOL DEVLIB WINAPI USB3102A_AO_StopTask(HANDLE hDevice);	// 停止生成任务

	BOOL DEVLIB WINAPI USB3102A_AO_ReleaseTask(HANDLE hDevice);// 释放生成任务

	// ========================= AO辅助操作函数 =========================
	BOOL DEVLIB WINAPI USB3102A_AO_GetMainInfo(				// 获得AO的主要信息
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									PUSB3102A_AO_MAIN_INFO pMainInfo); // 获得AO主要信息

	BOOL DEVLIB WINAPI USB3102A_AO_GetVoltRangeInfo(				// 获得采样范围的各种信息(幅度、分辨率、极性)
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									U32 nChannel,			// AO物理通道号
									U32 nSampleRange,		// 采样范围选择[0, 0]
									PUSB3102A_AO_VOLT_RANGE_INFO pRangeInfo); // 采样范围信息

	BOOL DEVLIB WINAPI USB3102A_AO_GetRateInfo(				// 获得采样速率信息
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									PUSB3102A_AO_SAMP_RATE_INFO pRateInfo); // 采样速率信息

	BOOL DEVLIB WINAPI USB3102A_AO_ScaleBinToVolt(			// 根据指定采样范围将采样原码数据量化成电压数据,返回实际量化的点数
									PUSB3102A_AO_VOLT_RANGE_INFO pRangeInfo, // 当前转换数据需要的采样范围信息
									F64 fVoltArray[],		// 电压缓冲区,用于返回量化后的电压数据,单位V
									I16 nBinArray[],		// 二进制原码缓冲区,用于传入量化前的原码数据，取值区间为[-32768, 32767], (单指某个通道的连续数据)
									U32 nScaleSamps,		// 请求量化的采样点数
									U32* pSampsScaled);		// 返回实际量化的采样点数

	BOOL DEVLIB WINAPI USB3102A_AO_ScaleVoltToBin(			// 根据指定采样范围将电压数据量化成采样原码数据,返回实际量化的点数
									PUSB3102A_AO_VOLT_RANGE_INFO pRangeInfo, // 当前转换数据需要的采样范围信息
									I16 nBinArray[],		// 原码缓冲区,用于返回量化后的原码数据，取值区间为[-32768, 32767], (单指某个通道的连续数据)
									F64 fVoltArray[],		// 电压缓冲区,用于传入量化前的电压数据,单位V
									U32 nScaleSamps,		// 请求量化的采样点数
									U32* pSampsScaled);		// 返回实际量化的采样点数

	BOOL DEVLIB WINAPI USB3102A_AO_VerifyParam(				// 校验AO工作参数(Verify Parameter),建议在初始化生成任务前调用此函数校验各参数合法性
									HANDLE hDevice,			// 设备对象句柄,它由DEV_Create()函数创建
									PUSB3102A_AO_PARAM pAOParam);// 待校验的AO工作参数

	BOOL DEVLIB WINAPI USB3102A_AO_LoadParam(HANDLE hDevice, PUSB3102A_AO_PARAM pAOParam); // 从USB3102A.ini中加载AO参数
	BOOL DEVLIB WINAPI USB3102A_AO_SaveParam(HANDLE hDevice, PUSB3102A_AO_PARAM pAOParam); // 保存AO参数至USB3102A.ini
	BOOL DEVLIB WINAPI USB3102A_AO_ResetParam(HANDLE hDevice, PUSB3102A_AO_PARAM pAOParam); // 将当前AO参数复位至出厂值

	// ############################ CTR计数器函数 ############################
	BOOL DEVLIB WINAPI USB3102A_CTR_InitTask(			// 初始采集任务
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nChannel,		// 通道号(本设备仅有1个通道,恒等于0)
									PUSB3102A_CTR_PARAM pCTRParam); // 工作参数

	BOOL DEVLIB WINAPI USB3102A_CTR_StartTask(			// 开始采集任务
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nChannel);		// 通道号(本设备仅有1个通道,恒等于0)

    BOOL DEVLIB WINAPI USB3102A_CTR_ReadCounter(			// 读取计数器的当前计数值
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nChannel,		// 通道号(本设备仅有1个通道,恒等于0)
									U32* pCountVal);	// 计数值

	BOOL DEVLIB WINAPI USB3102A_CTR_StopTask(			// 停止(或暂停)采集任务
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nChannel);		// 通道号(本设备仅有1个通道,恒等于0)

	BOOL DEVLIB WINAPI USB3102A_CTR_ReleaseTask(			// 释放采集任务
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nChannel);		// 通道号(本设备仅有1个通道,恒等于0)

	// ############################ DI、DO数字量输入输出实现函数 ############################
	BOOL DEVLIB WINAPI USB3102A_DIO_InitTask(			// 初始化DIO任务
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nPort,			// 端口号, 取值范围为[0, 1]
									PUSB3102A_DIO_PARAM pDIOParam); // 工作参数

	BOOL DEVLIB WINAPI USB3102A_DIO_GetParam(			// 获取DIO硬件参数
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nPort,			// 端口号, 取值范围为[0, 1]
									PUSB3102A_DIO_PARAM pDIOParam);	// 返回输出工作参数值

	BOOL DEVLIB WINAPI USB3102A_DIO_ReadPort(			// 读数字量端口值(Read Digital Port Value)
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nPort,			// 端口号, 取值范围为[0, 1]
									U32* pPortData);	// 返回的端口数据, 有效位Bit[7:0]

    BOOL DEVLIB WINAPI USB3102A_DIO_WritePort(			// 写数字量端口值(Write Digital Port Value)
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nPort,			// 端口号, 取值范围为[0, 1]
									U32 nPortData);		// 端口数据, 有效位Bit[7:0]

	BOOL DEVLIB WINAPI USB3102A_DIO_ReadLines(			// 读数字量端口值(Read Digital Port Value)
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nPort,			// 端口号, 取值范围为[0, 1]
									U32 bLineDataArray[8]);// 线数据缓冲区, 同时返回端口中各线的状态值bLineDataArray[n]=0:表示关(或低)状态, =1表示开(或高)状态

    BOOL DEVLIB WINAPI USB3102A_DIO_WriteLines(			// 写数字量端口值(Write Digital Port Value)
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nPort,			// 端口号, 取值范围为[0, 1]
									U32 bLineDataArray[8]); // 线数据缓冲区, 端口中各线的状态值bLineDataArray[n]=0:表示关(或低)状态, =1表示开(或高)状态

    BOOL DEVLIB WINAPI USB3102A_DIO_ReadLine(			// 读线值
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nPort,			// 端口号, 取值范围为[0, 1]
									U32 nLine,			// 线号,取值范围Port0时[0, 7]， Port1时[0, 7]
									U32* pLineData);	// 线数据, 取值0或1 

    BOOL DEVLIB WINAPI USB3102A_DIO_WriteLine(			// 写线值
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nPort,			// 端口号, 取值范围为[0, 1]
									U32 nLine,			// 线号,取值范围Port0时[0, 7]， Port1时[0, 7]
									U32 bLineData);		// 线值,取值0或1

	BOOL DEVLIB WINAPI USB3102A_DIO_ReleaseTask(			// 释放DIO任务
									HANDLE hDevice,		// 设备对象句柄,它由DEV_Create()函数创建
									U32 nPort);			// 端口号, 取值范围为[0, 1]

#ifdef __cplusplus
}
#endif

#ifndef _CVIDEF_H
	// 自动包含驱动函数导入库
	#ifndef _USB3102A_DRIVER_
		#ifndef LOAD_USB3102A_LIB // 如果没有加载LIB库
			#define LOAD_USB3102A_LIB
			#pragma comment(lib, "USB3102A.lib")
			#pragma message("Welcome to use ART products!")
			#pragma message("Linked with USB3102A.lib successfully!")
			#pragma message("Linked with USB3102A.dll successfully!")
		#endif // LOAD_USB3102A_LIB
	#endif // _USB3102A_DRIVER_
#endif // _CVIDEF_H

#endif // _USB3102A_DEVICE_