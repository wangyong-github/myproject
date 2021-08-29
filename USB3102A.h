#ifndef _USB3102A_DEVICE_
#define _USB3102A_DEVICE_

#include "windows.h"

// #################### ���Ի����������Ͷ��� #####################
// Humanized data types definition
typedef char				I8;		// 8λ�з�����������
typedef unsigned char		U8;		// 8λ�޷�����������
typedef short				I16;	// 16λ�з�����������
typedef unsigned short		U16;	// 16λ�޷�����������
typedef long				I32;	// 32λ�з�����������
typedef unsigned long		U32;	// 32λ�޷�����������
typedef __int64				I64;	// 64λ�з�����������
typedef unsigned __int64	U64;	// 64λ�޷�����������
typedef float				F32;	// 32λ��������(�����ȣ�ͬΪlabVIEW�е�SGL)
typedef double				F64;	// 64λ��������(˫���ȣ�ͬΪlabVIEW�е�DBL)

// ########################## �豸����ָ�� ########################
#define USB3102A_AI_MAX_CHANNELS		16	// �������֧��16·ģ������������ͨ��
#define USB3102A_AO_MAX_CHANNELS		2	// �������֧��2��ģ�������ͨ��
#define USB3102A_DIO_MAX_PORTS			2	// �������֧��2���������˿�
#define USB3102A_DIO_PORT0_MAX_LINES	8	// �������˿�0֧��8����
#define USB3102A_DIO_PORT1_MAX_LINES	8	// �������˿�1֧��8����

// ############# AI���������ṹ��USB3102A_AI_PARAM���� #############
typedef struct _USB3102A_AI_CH_PARAM // AIͨ�������ṹ��
{
	U32 nChannel;			// ͨ����[0, 15]���ֱ��ʾ����ͨ����AI0��AI15
	U32 nSampleRange;		// ������Χ(Sample Range)��λѡ��, ��������泣��������(�˲����ڵ��������Ч��δ��ע���Ĳ������ڵ������ģʽ����Ч)
	U32 nRefGround;			// �زο���ʽ

	U32 nReserved0;			// ����(δ��)
	U32 nReserved1;			// ����(δ��)
	U32 nReserved2;			// ����(δ��)
} USB3102A_AI_CH_PARAM, *PUSB3102A_AI_CH_PARAM;

// AIӲ�������ṹ��USB3102A_AI_CH_PARAM�е�nSampleRange������ʹ�õĲ�����Χ��λѡ��
#define USB3102A_AI_SAMPRANGE_N10_P10V		0 // ��10V
#define USB3102A_AI_SAMPRANGE_N5_P5V			1 // ��5V
#define USB3102A_AI_SAMPRANGE_N2_P2V			2 // ��2V
#define USB3102A_AI_SAMPRANGE_N1_P1V			3 // ��1V

// AIӲ��ͨ�������ṹ��USB3102A_AI_CH_PARAM�е�nRefGround������ʹ�õĵزο�ѡ��
#define USB3102A_AI_REFGND_RSE			0 // �ӵزο�����(Referenced Single Endpoint)
#define USB3102A_AI_REFGND_NRSE			1 // �ǲο�����(Non Referenced Single Endpoint)
#define USB3102A_AI_REFGND_DIFF			2 // ���(Differential)

typedef struct _USB3102A_AI_PARAM // ������AI�йص�Ӳ����������(���AI��������)
{
	// ͨ������
	U32 nSampChanCount;     // ����ͨ������[1, 16](�˲����ڵ��������Ч��δ��ע���Ĳ������ڵ������ģʽ����Ч)
	USB3102A_AI_CH_PARAM CHParam[16]; // ͨ����������(�˲����ڵ��������Ч��δ��ע���Ĳ������ڵ������ģʽ����Ч)
	U32 nSampleSignal;		// �����ź�(Sample Signal), ������泣������
	U32 nReserved0;		// ������Χ(Sample Range)��λѡ��, ��������泣��������(�˲����ڵ��������Ч��δ��ע���Ĳ������ڵ������ģʽ����Ч)
	U32 nReserved1;			// �����ֶ�(��δ����)
	
	// ʱ�Ӳ���
	U32 nSampleMode;		// ����ģʽ, 0=�������(����)�� 1��Ӳ����ʱ����(�ݲ�֧��)��2=���޵������ 3=��������
	U32 nSampsPerChan;		// ÿ��ͨ����������(Ҳ��ÿͨ������ȡ����),ȡֵ��ΧΪ�������ģʽʱӦ����1�����޵����������ʱΪ[2, 1024*1024]
	F64 fSampleRate;		// ��������(Sample Rate), ��λΪsps��ȡֵ��ΧΪ[1sps, 250000sps],����ÿ������ͨ���Ĳ�������, ����nChannelCount�ĳ˻����ܴ���250000sps
	U32 nClockSource;		// ʱ��Դѡ��, =0:��ʱ��OSCCLK; =1:��ʱ��CLKIN,��CN2�ϵ�Port1.DIO2(CLKIN)��������
	U32 bClockOutput;		// ����ʱ���������, =0:��ʾ��ֹ; =1:��ʾ����,��CN2�ϵ�Port1.DIO3(CLKOUT)�������
	U32 nReserved2;			// �����ֶ�(��δ����)
	U32 nReserved3;			// �����ֶ�(��δ����)

	// ��ʼ��������
	U32 bDTriggerEn;		// ���ִ�������(Digital Trigger Enable), =FALSE:��ʾ��ֹ; =TRUE:��ʾ����,�����ź���CN2�ϵ�Port1.DIO1(DTR)�������룬��˳�ʼ��ʱ�ὫDIO1�ķ���ǿ����Ϊ����
	U32 nDTriggerDir;	    // ���ִ�������(Digital Trigger Direction)
	U32 bATriggerEn;		// ģ������������(Analog Trigger Enable), =TRUE:��ʾ����, =FALSE:��ʾ��ֹ
	U32 nATriggerDir;		// ģ�ⴥ������(Analog Trigger Direction)
	U32 nATrigChannel;		// ����ͨ��(Analog Trigger Channel)
	F32 fTriggerLevel;		// ������ƽ(Trigger Level)
	U32 nTriggerSens;		// ����������(Trigger Sensitive for Digital and Analog trigger),��λ��΢��(uS)��ȡֵ��Χ[0, 1638]
	U32 nDelaySamps;		// �����ӳٵ���, ��λ�������㣬ȡֵ��Χ32λ��Ч[0, 4294967295]��0:Post Trigger, >0:Delay Trigger
	U32 nReserved4;			// �����ֶ�(��δ����)
	U32 nReserved5;			// �����ֶ�(��δ����)

	// ��������
	U32 nReserved6;			// �����ֶ�(��δ����)
	U32 nReserved7;			// �����ֶ�(��δ����)
	U32 nReserved8;			// �����ֶ�(��δ����)
	U32 nReserved9;			// �����ֶ�(��δ����)
} USB3102A_AI_PARAM, *PUSB3102A_AI_PARAM;

// AIӲ�������ṹ��USB3102A_AI_PARAM�е�nSampleSignal�����ź���ʹ�õ�ѡ��
#define USB3102A_AI_SAMPSIGNAL_AI			0 // AIͨ�������ź�
#define USB3102A_AI_SAMPSIGNAL_0V			1 // 0V(AGND)
#define USB3102A_AI_SAMPSIGNAL_4D096V		2 // 4.096V
#define USB3102A_AI_SAMPSIGNAL_N4D096V		3 // -4.096V
#define USB3102A_AI_SAMPSIGNAL_AO0			4 // AO0
#define USB3102A_AI_SAMPSIGNAL_NAO0			5 // -AO0
#define USB3102A_AI_SAMPSIGNAL_AO1			6 // AO1
#define USB3102A_AI_SAMPSIGNAL_NAO1			7 // -AO1

// AIӲ�������ṹ��USB3102A_AI_PARAM�е�nSampleMode����ģʽ��ʹ�õ�ѡ��
#define USB3102A_AI_SAMPMODE_ONE_DEMAND		0 // ������赥�����
#define USB3102A_AI_SAMPMODE_FINITE			2 // ���޵����
#define USB3102A_AI_SAMPMODE_CONTINUOUS		3 // ��������

// AIӲ�������ṹ��USB3102A_AI_PARAM�е�nClockSourceʱ��Դ��ʹ�õ�ѡ��
#define USB3102A_AI_CLKSRC_LOCAL			0 // ����ʱ��(ͨ��Ϊ���ؾ���ʱ��OSCCLK),Ҳ���ڲ�ʱ��
#define USB3102A_AI_CLKSRC_CLKIN			1 // �ⲿʱ��(��������CN2�ϵ�DIO2/CLKIN��������)

// AIӲ�������ṹ��USB3102A_AI_PARAM�е�nDTriggerDir/nATriggerDir����������ʹ�õ�ѡ��
#define USB3102A_AI_TRIGDIR_FALLING			0 // �½���/�͵�ƽ
#define USB3102A_AI_TRIGDIR_RISING			1 // ������/�ߵ�ƽ
#define USB3102A_AI_TRIGDIR_CHANGE			2 // �仯(�����±���/�ߵ͵�ƽ����Ч)

// #################### AI����״̬�ṹ��USB3102A_AI_STATUS���� #####################
typedef struct _USB3102A_AI_STATUS
{
	U32 bTaskDone;				// �ɼ�������ɱ�־, =TRUE:��ʾ�ɼ��������, =FALSE:��ʾ�ɼ�����δ��ɣ����ڽ�����
	U32 bTriggered;				// ������־, =TRUE:��ʾ�ѱ�����, =FALSE:��ʾδ������(�����ȴ�����)

	U32 nTaskState;				// �ɼ�����״̬, =1:����, ����ֵ��ʾ���쳣���
	U32 nAvailSampsPerChan;		// ÿͨ����Ч������ֻ�������ڵ�ǰָ����������ʱ���ܵ���AI_ReadAnalog()������ȡָ�����ȵĲ�������
	U32 nMaxAvailSampsPerChan;	// ��AI_StartTask()��ÿͨ�����ֹ��������Ч������״ֵ̬��Χ[0, nBufSampsPerChan],����Ϊ���ɼ�������ܶ��ṩ�������ֵԽ������1�����ʾ��ζ������Խ�ߣ�Խ���׳����������Ŀ���
	U32 nBufSampsPerChan;		// ÿͨ����������С(��������)��
	I64 nSampsPerChanAcquired;	// ÿͨ���Ѳ�������(�Կ�ʼ�ɼ�����(AI_StartTask())֮���������ĵ���)�����ֻ�Ǹ��û���ͳ������

	U32 nHardOverflowCnt;		// Ӳ���������(�ڲ��������º����0)
	U32 nSoftOverflowCnt;		// ����������(�ڲ��������º����0)
	U32 nInitTaskCnt;			// ��ʼ���ɼ�����Ĵ���(������AI_InitTask()�Ĵ���)
	U32 nReleaseTaskCnt;		// �ͷŲɼ�����Ĵ���(������AI_ReleaseTask()�Ĵ���)
	U32 nStartTaskCnt;			// ��ʼ�ɼ�����Ĵ���(������AI_StartTask()�Ĵ���)
	U32 nStopTaskCnt;			// ֹͣ�ɼ�����Ĵ���(������AI_StopTask()�Ĵ���)
	U32 nTransRate;				// ��������, ��ÿ�봫�����(sps)����ΪUSB��Ӧ������������ܵļ����Ϣ

	U32 nReserved0;				// �����ֶ�(��δ����)
	U32 nReserved1;				// �����ֶ�(��δ����)
	U32 nReserved2;				// �����ֶ�(��δ����)
	U32 nReserved3;				// �����ֶ�(��δ����)
	U32 nReserved4;				// �����ֶ�(��δ����)
} USB3102A_AI_STATUS, *PUSB3102A_AI_STATUS;

// #################### AI��Ҫ��Ϣ�ṹ��(USB3102A_AI_MAIN_INFO) #######################
typedef struct _USB3102A_AI_MAIN_INFO
{
	U32 nChannelCount;		// AIͨ������
	U32 nSampRangeCount;	// AI������Χ��λ����
	U32 nSampGainCount;		// AI�������浲λ����
	U32 nCouplingCount;		// AI��ϵ�λ����
	U32 nImpedanceCount;	// AI�迹�ĵ�λ����
	U32 nDepthOfMemory;		// AI���ش洢�����(����)
	U32 nSampResolution;	// AI�����ֱ���(��=8��ʾ8Bit; =12��ʾ12Bit; =14��ʾ14Bit; =16��ʾ16Bit)
	U32 nSampCodeCount;		// AI������������(��256, 4096, 16384, 65536)
	U32 nTrigLvlResolution;	// ������ƽ�ֱ���(��=8��ʾ8Bit; =12��ʾ12Bit; =16��ʾ16Bit)
	U32 nTrigLvlCodeCount;	// ������ƽ��������(��256, 4096)

	U32 nReserved0;			// �����ֶ�(��δ����)
	U32 nReserved1;			// �����ֶ�(��δ����)
	U32 nReserved2;			// �����ֶ�(��δ����)
	U32 nReserved3;			// �����ֶ�(��δ����)
} USB3102A_AI_MAIN_INFO, *PUSB3102A_AI_MAIN_INFO;

// #################### AI������Χ��Ϣ�ṹ��(USB3102A_AI_VOLT_RANGE_INFO) #######################
typedef struct _USB3102A_AI_VOLT_RANGE_INFO
{
	U32 nSampleRange;		// ��ǰ������Χ��λ��
	U32 nReserved0;			// �����ֶ�(��δ����)
	F64 fMaxVolt;			// ������Χ������ѹֵ,��λ:��(V)
	F64 fMinVolt;			// ������Χ����С��ѹֵ,��λ:��(V)
	F64 fAmplitude;			// ������Χ����,��λ:��(V)
	F64 fHalfOfAmp;			// ������Χ���ȵĶ���֮һ,��λ:��(V)
	F64 fCodeWidth;			// ������,��λ:��(V), ��ÿ����λ��ֵ������ĵ�ѹֵ
	F64 fOffsetVolt;		// ƫ�Ƶ�ѹ,��λ:��(V),һ��������ƫУ׼
	F64 fOffsetCode;		// ƫ����ֵ,һ��������ƫУ׼,������ĵ�ѹֵ�ȼ���fOffsetVolt
	char strDesc[16];		// ������Χ�ַ�����,��"��10V", "0-10V"��

	U32 nPolarity;			// ������Χ�ļ���(0=˫����BiPolar, 1=������UniPolar)
	U32 nCodeCount;			// ԭ������
	I32 nMaxCode;			// ԭ�����ֵ
	I32 nMinCode;	        // ԭ����Сֵ

	U32 nReserved1;			// �����ֶ�(��δ����)
	U32 nReserved2;			// �����ֶ�(��δ����)
	U32 nReserved3;			// �����ֶ�(��δ����)
	U32 nReserved4;			// �����ֶ�(��δ����)
} USB3102A_AI_VOLT_RANGE_INFO, *PUSB3102A_AI_VOLT_RANGE_INFO;

// �ṹ��AI_VOLT_RANGE_INFO�����ݳ�ԱPolarity���õĲ�����Χ����ѡ��
#define USB3102A_AI_POLAR_BIPOLAR	0 // ˫����
#define USB3102A_AI_POLAR_UNIPOLAR	1 // ������

// #################### AI������Ϣ�ṹ��(USB3102A_AI_SAMP_RATE_INFO) #######################
typedef struct _USB3102A_AI_SAMP_RATE_INFO
{
	F64 fMaxRate;		// ��ͨ������������(sps)����ͨ��ʱ��ͨ��ƽ����������
	F64 fMinRate;		// ��ͨ����С��������(sps)����ͨ��ʱ��ͨ��ƽ����С������
	F64 fTimerBase;		// ʱ�ӻ�׼�������ؾ���Ƶ�ʣ���λ��Hz
	U32 nDivideMode;	// ��Ƶģʽ��0=������Ƶ(INTDIV), 1=DDS��Ƶ(DDSDIV)
	U32 nRateType;		// ��������,ָfMaxRate��fMinRate������, =0:��ʾΪ���в���ͨ����������, =1:��ʾΪÿ������ͨ��������

	U32 nReserved0;		// �����ֶ�(��δ����)
	U32 nReserved1;		// �����ֶ�(��δ����)
} USB3102A_AI_SAMP_RATE_INFO, *PUSB3102A_AI_SAMP_RATE_INFO;

// ##################### AO���������ṹ��USB3102A_AO_PARAM���� ###################
typedef struct _USB3102A_AO_CH_PARAM // AIͨ�������ṹ��
{
	U32 bChannelEn;			// ͨ����������
	U32 nSampleRange;		// ������Χ�����嶨����ο����泣������
	U32 nReserved0;			// ����(δ��)
	U32 nReserved1;			// ����(δ��)
	U32 nReserved2;			// ����(δ��)
	U32 nReserved3;			// ����(δ��)
} USB3102A_AO_CH_PARAM, *PUSB3102A_AO_CH_PARAM;

// AOӲ�������ṹ��USB3102A_AO_CH_PARAM�е�nSampleRange������ʹ�õ�ģ�������������Χ��λѡ��
#define USB3102A_AO_SAMPRANGE_N10_P10V		0 // ��10V

typedef struct _USB3102A_AO_PARAM // ������AO�йص�Ӳ����������(���AO��������)
{
	// ͨ������
	USB3102A_AO_CH_PARAM CHParam[2]; // ͨ��������ͨ��ʹ�ܣ�������Χ��

	// ʱ�Ӳ���
	U32 nSampleMode;		// ����ģʽ, 0=�����ʱ�������(����)�� 1��Ӳ����ʱ�������(�ݲ�֧��)��2=���޵������ 3=��������
	U32 nSampsPerChan;		// ÿ��ͨ�����ɵ���(Ҳ��ÿͨ����д�����),ȡֵ��ΧΪ�������ģʽʱӦ����1�����޵����������ʱΪ[2, 1024*1024]
	F64 fSampleRate;		// ��������(Sample Rate), ��λΪsps��ȡֵ��ΧΪ[1sps, 100000sps],����ÿ������ͨ���Ĳ�������
	U32 nClockSource;		// ʱ��Դѡ��, =0:��ʱ��OSCCLK; =1:��ʱ��CLKIN,��CN2�ϵ�Port1.DIO2(CLKIN)��������
	U32 bClockOutput;		// ����ʱ���������, =0:��ʾ��ֹ; =1:��ʾ����,��CN2�ϵ�Port1.DIO3(CLKOUT)�������
	U32 bRegenModeEn;		// ����������ģʽ����(����������ģʽ����Ч), =TRUE:ֻ�ǶԿ�ʼ������ǰд�������еĲ��ζ����ݽ���ѭ���ظ����ɣ�=FALSE:��ֹ������ģʽ���ڿ�ʼ��������󣬻�Ҫ���ϵ���������д���µĲ�������
	U32 nReserved0;			// �����ֶ�(��δ����)

	// ��ʼ��������
	U32 bDTriggerEn;		// ���ִ���DTR����(Digital Trigger Enable), =FALSE:��ʾ��ֹ; =TRUE:��ʾ����,�����ź���CN2�ϵ�Port1.DIO1(DTR)�������룬��˳�ʼ��ʱ�ὫDIO1�ķ���ǿ����Ϊ����
	U32 nDTriggerDir;	    // ���ִ�������(Digital Trigger Direction)
	U32 nTriggerSens;		// ����������(Trigger Sensitive for Digital trigger),��λ��΢��(uS)��ȡֵ��Χ[0, 1638]
	U32 nDelaySamps;		// �����ӳٵ���, ��λ�������㣬ȡֵ��Χ32λ��Ч[0, 4294967295]��0:Post Trigger, >0:Delay Trigger
	U32 nReserved1;			// �����ֶ�(��δ����)
	U32 nReserved2;			// �����ֶ�(��δ����)
	
	// ��������
	U32 nReserved3;			// �����ֶ�(��δ����)
	U32 nReserved4;			// �����ֶ�(��δ����)
	U32 nReserved5;			// �����ֶ�(��δ����)
	U32 nReserved6;			// �����ֶ�(��δ����)
} USB3102A_AO_PARAM, *PUSB3102A_AO_PARAM;

// AOӲ�������ṹ��USB3102A_AO_PARAM�е�nSampleMode����ģʽ��ʹ�õ�ѡ��
#define USB3102A_AO_SAMPMODE_ONE_DEMAND	0 // �������(����)
#define USB3102A_AO_SAMPMODE_ONE_HWTIMED	1 // �������(Ӳ����ʱ,Hardware Timed, ���豸��ʱ��֧��)
#define USB3102A_AO_SAMPMODE_FINITE		2 // ���޵����
#define USB3102A_AO_SAMPMODE_CONTINUOUS	3 // ��������

// AOӲ�������ṹ��USB3102A_AO_PARAM�е�nClockSourceʱ��Դ��ʹ�õ�ѡ��
#define USB3102A_AO_CLKSRC_LOCAL			0 // ����ʱ��(ͨ��Ϊ���ؾ���ʱ��OSCCLK),Ҳ���ڲ�ʱ��
#define USB3102A_AO_CLKSRC_CLKIN			1 // �ⲿʱ��(��������CN2�ϵ�DIO2/CLKIN��������)

// AOӲ�������ṹ��USB3102A_AO_PARAM�е�nDTriggerDir����������ʹ�õ�ѡ��
#define USB3102A_AO_TRIGDIR_FALLING		0 // �½���/�͵�ƽ
#define USB3102A_AO_TRIGDIR_RISING		1 // ������/�ߵ�ƽ
#define USB3102A_AO_TRIGDIR_CHANGE		2 // �仯(�����±���/�ߵ͵�ƽ����Ч)

// #################### AO����״̬�ṹ��USB3102A_AO_STATUS���� #####################
typedef struct _USB3102A_AO_STATUS
{
	U32 bTaskDone;				// ����������ɱ�־, =TRUE:��ʾ�����������, =FALSE:��ʾ��������δ��ɣ����ڽ�����
	U32 bTriggered;				// ������־, =TRUE:��ʾ�ѱ�����, =FALSE:��ʾδ������(�����ȴ�����)

	U32 nTaskState;				// ��������״̬, =1:����, ����ֵ��ʾ���쳣���
	U32 nAvailSampsPerChan;		// ��д��������õ������ڲ���nWriteSampsPerChanʱ���ܵ���AO_WriteAnalog()д���������
	U32 nMaxAvailSampsPerChan;	// ��AO_StartTask()����ֹ�������д������״ֵ̬��Χ[0, nBufSampsPerChan],����Ϊ���ɼ�������ܶ��ṩ�������ֵԽ������1�����ʾ��ζ������Խ�ߣ�Խ���׳����������Ŀ���
	U32 nBufSampsPerChan;		// ÿͨ����������С(��������)��
	U64 nSampsPerChanAcquired;	// ÿͨ���Ѳ�������(�Կ�ʼ��������(AO_StartTask())֮���������ĵ���)�����ֻ�Ǹ��û���ͳ������

	U32 nHardUnderflowCnt;		// Ӳ�������������(�ڲ���������º����0)
	U32 nSoftUnderflowCnt;		// ��������������(�ڲ���������º����0)
	U32 nInitTaskCnt;			// ��ʼ����������Ĵ���(������AO_InitTask()�Ĵ���)
	U32 nReleaseTaskCnt;		// �ͷ���������Ĵ���(������AO_ReleaseTask()�Ĵ���)
	U32 nStartTaskCnt;			// ��ʼ��������Ĵ���(������AO_StartTask()�Ĵ���)
	U32 nStopTaskCnt;			// ֹͣ��������Ĵ���(������AO_StopTask()�Ĵ���)
	U32 nTransRate;				// ��������, ��ÿ�봫�����(sps)����ΪUSB��Ӧ������������ܵļ����Ϣ

	U32 nReserved0;				// �����ֶ�(��δ����)
	U32 nReserved1;				// �����ֶ�(��δ����)
	U32 nReserved2;				// �����ֶ�(��δ����)
	U32 nReserved3;				// �����ֶ�(��δ����)
	U32 nReserved4;				// �����ֶ�(��δ����)
} USB3102A_AO_STATUS, *PUSB3102A_AO_STATUS;

// #################### AO��Ҫ��Ϣ�ṹ��(USB3102A_AO_MAIN_INFO) #######################
typedef struct _USB3102A_AO_MAIN_INFO
{
	U32 nChannelCount;		// AOͨ������
	U32 nSampRangeCount;	// AO������Χ��λ����
	U32 nSampGainCount;		// AO���浲λ����
	U32 nCouplingCount;		// AO��ϵ�λ����
	U32 nImpedanceCount;	// AO�迹�ĵ�λ����
	U32 nDepthOfMemory;		// AO���ش洢�����(����)
	U32 nSampResolution;	// AO�����ֱ���(��=8��ʾ8Bit; =12��ʾ12Bit; =14��ʾ14Bit; =16��ʾ16Bit)
	U32 nSampCodeCount;		// AO������������(��256, 4096, 16384, 65536)
	U32 nTrigLvlResolution;	// ������ƽ�ֱ���(��=8��ʾ8Bit; =12��ʾ12Bit; =16��ʾ16Bit)
	U32 nTrigLvlCodeCount;	// ������ƽ��������(��256, 4096)

	U32 nReserved0;			// �����ֶ�(��δ����)
	U32 nReserved1;			// �����ֶ�(��δ����)
	U32 nReserved2;			// �����ֶ�(��δ����)
	U32 nReserved3;			// �����ֶ�(��δ����)
} USB3102A_AO_MAIN_INFO, *PUSB3102A_AO_MAIN_INFO;

// #################### AO������Χ��Ϣ�ṹ��(USB3102A_AO_VOLT_RANGE_INFO) #######################
typedef struct _USB3102A_AO_VOLT_RANGE_INFO
{
	U32 nSampleRange;		// ��ǰ������Χ��λ��
	U32 nReserved0;			// �����ֶ�(��δ����)
	F64 fMaxVolt;			// ������Χ������ѹֵ,��λ:��(V)
	F64 fMinVolt;			// ������Χ����С��ѹֵ,��λ:��(V)
	F64 fAmplitude;			// ������Χ����,��λ:��(V)
	F64 fHalfOfAmp;			// ������Χ���ȵĶ���֮һ,��λ:��(V)
	F64 fCodeWidth;			// ������,��λ:��(V), ��ÿ����λ��ֵ������ĵ�ѹֵ
	F64 fOffsetVolt;		// ƫ�Ƶ�ѹ,��λ:��(V),һ��������ƫУ׼
	F64 fOffsetCode;		// ƫ����ֵ,һ��������ƫУ׼,������ĵ�ѹֵ�ȼ���fOffsetVolt
	char strDesc[16];		// ������Χ�ַ�����,��"��10V", "0-10V"��

	U32 nPolarity;			// ������Χ�ļ���(0=˫����BiPolar, 1=������UniPolar)
	U32 nCodeCount;			// ԭ������
	I32 nMaxCode;			// ԭ�����ֵ
	I32 nMinCode;	        // ԭ����Сֵ

	U32 nReserved1;			// �����ֶ�(��δ����)
	U32 nReserved2;			// �����ֶ�(��δ����)
	U32 nReserved3;			// �����ֶ�(��δ����)
	U32 nReserved4;			// �����ֶ�(��δ����)
} USB3102A_AO_VOLT_RANGE_INFO, *PUSB3102A_AO_VOLT_RANGE_INFO;

// �ṹ��AO_VOLT_RANGE_INFO�����ݳ�ԱPolarity���õĲ�����Χ����ѡ��
#define USB3102A_AO_POLAR_BIPOLAR	0 // ˫����
#define USB3102A_AO_POLAR_UNIPOLAR	1 // ������

// #################### AO������Ϣ�ṹ��(USB3102A_AO_SAMP_RATE_INFO) #######################
typedef struct _USB3102A_AO_SAMP_RATE_INFO
{
	F64 fMaxRate;		// ��ͨ������������(sps)����ͨ��ʱ��ͨ��ƽ����������
	F64 fMinRate;		// ��ͨ����С��������(sps)����ͨ��ʱ��ͨ��ƽ����С������
	F64 fTimerBase;		// ʱ�ӻ�׼�������ؾ���Ƶ�ʣ���λ��Hz
	U32 nDivideMode;	// ��Ƶģʽ��0=������Ƶ(INTDIV), 1=DDS��Ƶ(DDSDIV)

	U32 nRateType;		// ��������,ָfMaxRate��fMinRate������, =0:��ʾΪ���в���ͨ����������, =1:��ʾΪÿ������ͨ��������
	U32 nReserved1;		// �����ֶ�(��δ����)
	U32 nReserved2;		// �����ֶ�(��δ����)
} USB3102A_AO_SAMP_RATE_INFO, *PUSB3102A_AO_SAMP_RATE_INFO;

// ###################### CTR���������ṹ��(USB3102A_CTR_PARAM) #######################
typedef struct _USB3102A_CTR_PARAM // ������CTR�йص�Ӳ����������(���CTR��������)
{
	U32 nPulseDir;		// ���巽��(0=�½���,1=������, 2=˫����),����Դ��Port1.DIO0(CTRSRC)��������
	U32 bInitReset;		// �Ƿ��ڳ�ʼ��ʱ��λ��������0��=TRUE:��ʼ��ʱ���������, =FALSE:��ʼ���ǲ���
	U32 bFullReset;		// �Ƿ������ʱ�Զ���λ��������0��=TRUE:���ʱ�Զ���λ��0, =FALSE:�����ʱ��ס��ǰ��������

	U32 nReserved0;		// �����ֶ�(��δ����)
	U32 nReserved1;		// �����ֶ�(��δ����)
	U32 nReserved2;		// �����ֶ�(��δ����)
} USB3102A_CTR_PARAM, *PUSB3102A_CTR_PARAM;

// CTRӲ�������ṹ��USB3102A_CTR_PARAM�е�nPulseDir���巽����ʹ�õ�ѡ��
#define USB3102A_CTR_PULSEDIR_FALLING	0 // �±���
#define USB3102A_CTR_PULSEDIR_RISING		1 // �ϱ���
#define USB3102A_CTR_PULSEDIR_CHANGE		2 // �仯(���±��ؾ���Ч)

// #################### DIO��Ҫ��Ϣ�ṹ��(USB3102A_DIO_PARAM) #######################
typedef struct _USB3102A_DIO_PARAM // ������DIO�йص�Ӳ����������(���DIO��������)
{
	U8 bOutputEn[8];	// �����������(Output Enable),bOutputEn[n]=0:�����ֹ(��Ϊ����)��=1:�������(��������Ч)��(Ĭ�Ϸ���Ϊ����)
						// �����临�õ�CTRSRC,DTR,CLKIN,CLKOUT��Чʱ����������Ӧ��DIO��������Զ�ʧЧ
	
	U32 nReserved0;		// �����ֶ�(��δ����)
	U32 nReserved1;		// �����ֶ�(��δ����)
	U32 nReserved2;		// �����ֶ�(��δ����)
} USB3102A_DIO_PARAM, *PUSB3102A_DIO_PARAM;

// ################################ ����������Ϣ ################################
#define ERROR_NO_AVAILABLE_SAMPS	(0xE0000000+1)	// ����Ч����
#define ERROR_TASK_FAIL				(0xE0000000+2)	// ����ʧ��
#define ERROR_RATE_OVER_RANGE		(0xE0000000+3)	// �������ʳ���

// ################################ �豸�����ӿ����� ################################
#ifndef _USB3102A_DRIVER_
#define DEVLIB __declspec(dllimport)
#else
#define DEVLIB __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	// ################################ DEV�豸��������� ################################
	HANDLE DEVLIB WINAPI USB3102A_DEV_Create(					// �����豸������(hDevice), �ɹ�����ʵ�ʾ��,ʧ���򷵻�INVALID_HANDLE_VALUE(-1),�ɵ���GetLastError()��������ԭ��
										U32 nDeviceIdx,			// �豸���(�߼���Ż��������, ����ʹ����������ɲ���bUsePhysIdx����)
										BOOL bUsePhysIdx);		// �Ƿ�ʹ���������, =TRUE:ʹ���������, =FALSE:ʹ���߼����										

	U32 DEVLIB WINAPI USB3102A_DEV_GetCount(void);				// ȡ�ø��豸��̨��,�ɹ�����ֵ>0, ʧ�ܷ���0(�ɵ���GetLastError()��������ԭ��)

	BOOL DEVLIB WINAPI USB3102A_DEV_GetCurrentIdx(				// ���ָ���豸�е��߼���ź��������
										HANDLE hDevice,			// �豸������,����DEV_Create()��������
										U32* pLgcIdx,			// ���ص��߼����
										U32* pPhysIdx);			// ���ص��������

	BOOL DEVLIB WINAPI USB3102A_DEV_GetSpeed(					// ��ȡ�豸���ӵ�USB�˿��ٶ�, 
										HANDLE hDevice,			// �豸������,����DEV_Create()��������
										U32* pSpeed);			// USB�ӿ��ٶȣ�=1:USB1.0, =2:USB2.0, =3:USB3.0


	BOOL DEVLIB WINAPI USB3102A_DEV_Release(HANDLE hDevice);		// �ͷ��豸����(�ؼ�����)

	// ################################ AIģ��������ʵ�ֺ��� ################################
	BOOL DEVLIB WINAPI USB3102A_AI_InitTask(					// ��ʼ���ɼ�����(Initialize Task)
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									PUSB3102A_AI_PARAM pAIParam, // AI��������, �����ڴ˺����о���Ӳ����ʼ״̬�͸�����ģʽ,����������AI_VerifyParam()���в���У��
									HANDLE* pSampEvent);	// ���ز����¼�������,���豸�г��ֿɶ����ݶ�ʱ�ᴥ�����¼�������=NULL,��ʾ����Ҫ���¼����

    BOOL DEVLIB WINAPI USB3102A_AI_StartTask(				// ��ʼ�ɼ�����
									HANDLE hDevice);		// �豸������,����DEV_Create()��������

    BOOL DEVLIB WINAPI USB3102A_AI_SendSoftTrig(				// ������������¼�(Send Software Trigger),�������Ҳ��ǿ�ƴ���
									HANDLE hDevice);		// �豸������,����DEV_Create()��������

	BOOL DEVLIB WINAPI USB3102A_AI_GetStatus(HANDLE hDevice, PUSB3102A_AI_STATUS pAIStatus);   // ȡ��AI����״̬

	BOOL DEVLIB WINAPI USB3102A_AI_WaitUntilTaskDone(		// �ɼ��������ǰ�ȴ�,����TRUE��ʾ�ɼ��������
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									F64 fTimeout);			// ���ڵȴ���ʱ�䣬��λ����(S)

	BOOL DEVLIB WINAPI USB3102A_AI_ReadAnalog(				// �Ӳɼ������ж�ȡ��������(��ѹ��������)(Read analog data from the task)
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									F64 fAnlgArray[],		// ģ����������(��ѹ����),���ڷ��ز����ĵ�ѹ���ݣ�ȡֵ�����ɸ�ͨ������ʱ�Ĳ�����Χ����(��λ:V)
									U32 nReadSampsPerChan,	// ÿͨ�������ȡ�ĵ���(��λ����)
									U32* pSampsPerChanRead,	// ����ÿͨ��ʵ�ʶ�ȡ�ĵ���(��λ����), =NULL,��ʾ���뷵��
									U32* pAvailSampsPerChan,// �����л����ڵĿɶ�����, =NULL,��ʾ���뷵��
									F64 fTimeout);			// ��ʱʱ�䣬��λ����(S)

	BOOL DEVLIB WINAPI USB3102A_AI_ReadBinary(				// �Ӳɼ������ж�ȡ��������(ԭ����������)(Read binary data from the task)
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									I16 nBinArray[],		// �������������飨ԭ�����飩,���ڷ��ز�����ԭ�����ݣ�ȡֵ����Ϊ[-32768, 32767]
									U32 nReadSampsPerChan,	// ÿͨ�������ȡ�ĵ���(��λ����)
									U32* pSampsPerChanRead,	// ����ÿͨ��ʵ�ʶ�ȡ�ĵ���(��λ����), =NULL,��ʾ���뷵��
									U32* pAvailSampsPerChan,// �����л����ڵĿɶ�����, =NULL,��ʾ���뷵��
									F64 fTimeout);			// ��ʱʱ�䣬��λ����(S)

	BOOL DEVLIB WINAPI USB3102A_AI_StopTask(HANDLE hDevice);	// ֹͣAI�ɼ�����

	BOOL DEVLIB WINAPI USB3102A_AI_ReleaseTask(HANDLE hDevice); // �ͷŲɼ�����

	// ========================= AI������������ =========================
	BOOL DEVLIB WINAPI USB3102A_AI_GetMainInfo(				// ���AI����Ҫ��Ϣ
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									PUSB3102A_AI_MAIN_INFO pMainInfo); // ���AI��Ҫ��Ϣ

	BOOL DEVLIB WINAPI USB3102A_AI_GetVoltRangeInfo(				// ��ò�����Χ�ĸ�����Ϣ(���ȡ��ֱ��ʡ�����)
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									U32 nChannel,			// AI����ͨ����(���ڱ��豸����ͨ������һ��������Χѡ��,�ʺ����0)
									U32 nSampleRange,		// ������Χѡ��[0, 0]
									PUSB3102A_AI_VOLT_RANGE_INFO pRangeInfo); // ������Χ��Ϣ

	BOOL DEVLIB WINAPI USB3102A_AI_GetRateInfo(				// ��ò���������Ϣ
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									PUSB3102A_AI_SAMP_RATE_INFO pRateInfo); // ����������Ϣ

	BOOL DEVLIB WINAPI USB3102A_AI_ScaleBinToVolt(			// ����ָ��������Χ������ԭ�����������ɵ�ѹ����,����ʵ�������ĵ���
									PUSB3102A_AI_VOLT_RANGE_INFO pRangeInfo, // ��ǰת��������Ҫ�Ĳ�����Χ��Ϣ
									PVOID pGainInfo,		// ��ǰת��������Ҫ�Ĳ���������Ϣ(����=NULL����ʾ��ʹ������)
									F64 fVoltArray[],		// ��ѹ��������,���ڷ���������ĵ�ѹ����,��λV
									I16 nBinArray[],		// ������ԭ������,���ڴ�������ǰ��ԭ�����ݣ�ȡֵ����Ϊ[-32768, 32767], (��ָĳ��ͨ������������)
									U32 nScaleSamps,		// ���������Ĳ�������
									U32* pSampsScaled);		// ����ʵ�������Ĳ�������, =NULL,��ʾ���뷵��

	BOOL DEVLIB WINAPI USB3102A_AI_ScaleVoltToBin(			// ����ָ��������Χ����ѹ���������ɶ�����ԭ������,����ʵ�������ĵ���
									PUSB3102A_AI_VOLT_RANGE_INFO pRangeInfo, // ��ǰת��������Ҫ�Ĳ�����Χ��Ϣ
									PVOID pGainInfo,		// ��ǰת��������Ҫ�Ĳ���������Ϣ(����=NULL����ʾ��ʹ������)
									I16 nBinArray[],		// ԭ�뻺����,���ڷ����������ԭ�����ݣ�ȡֵ����Ϊ[-32768, 32767], (��ָĳ��ͨ������������)
									F64 fVoltArray[],		// ��ѹ������,���ڴ�������ǰ�ĵ�ѹ����,��λV
									U32 nScaleSamps,		// ���������Ĳ�������
									U32* pSampsScaled);		// ����ʵ�������Ĳ�������, =NULL,��ʾ���뷵��

	BOOL DEVLIB WINAPI USB3102A_AI_VerifyParam(				// У��AI��������(Verify Parameter),�����ڳ�ʼ��AI����ǰ���ô˺���У��������Ϸ���
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									PUSB3102A_AI_PARAM pAIParam);// ��У���AI��������

	BOOL DEVLIB WINAPI USB3102A_AI_LoadParam(HANDLE hDevice, PUSB3102A_AI_PARAM pAIParam); // ��USB3102A.ini�м���AI����
	BOOL DEVLIB WINAPI USB3102A_AI_SaveParam(HANDLE hDevice, PUSB3102A_AI_PARAM pAIParam); // ����AI������USB3102A.ini
	BOOL DEVLIB WINAPI USB3102A_AI_ResetParam(HANDLE hDevice, PUSB3102A_AI_PARAM pAIParam); // ����ǰAI������λ������ֵ

	// ################################ AOģ�������ʵ�ֺ��� ################################
	BOOL DEVLIB WINAPI USB3102A_AO_InitTask(					// ��ʼ����������(Initialize Task)
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									PUSB3102A_AO_PARAM pAOParam, // AO��������, �����ڴ˺����о���Ӳ����ʼ״̬�͸�����ģʽ,����������AO_VerifyParam()���в���У��
									HANDLE* pSampEvent);	// ���ز����¼�������,���豸�г��ֿɶ����ݶ�ʱ�ᴥ�����¼�������=NULL,��ʾ����Ҫ���¼����

    BOOL DEVLIB WINAPI USB3102A_AO_StartTask(				// ��ʼ�������� 
									HANDLE hDevice);		// �豸������,����DEV_Create()��������

    BOOL DEVLIB WINAPI USB3102A_AO_SendSoftTrig(				// ������������¼�(Send Software Trigger),�������Ҳ��ǿ�ƴ���
									HANDLE hDevice);		// �豸������,����DEV_Create()��������

	BOOL DEVLIB WINAPI USB3102A_AO_GetStatus(HANDLE hDevice, PUSB3102A_AO_STATUS pAOStatus);   // ȡ��AO����״̬

	BOOL DEVLIB WINAPI USB3102A_AO_WaitUntilTaskDone(		// �����������ǰ�ȴ�,����TRUE��ʾ�����������
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									F64 fTimeout);			// ��ʱʱ�䣬��λ����(S)

	BOOL DEVLIB WINAPI USB3102A_AO_WriteAnalog(				// ������������д��AO�����ģ������ѹ����(Write analog data to the task)
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									F64 fAnlgArray[],		// AO0��AO1��ѹ��ϻ�����,���AO��ģ��(��ѹ)���ݣ�ȡֵ�����ɸ�ͨ������ʱ�Ĳ�����Χ����(��λ:V)
									U32 nWriteSampsPerChan,	// ÿͨ������д��ĵ���(��λ����)
									U32* pSampsPerChanWritten,// ����ÿͨ��ʵ��д��ĵ���(��λ����), =NULL,��ʾ���뷵��
									U32* pAvailSampsPerChan,// ���ص�ǰ��д��Ĳ�������, =NULL,��ʾ���뷵��
									F64 fTimeout);			// ��ʱʱ�䣬��λ����(S)

	BOOL DEVLIB WINAPI USB3102A_AO_WriteBinary(				// ������������д��AO����Ķ�����ԭ������(Write binary data to the task)
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									I16 nBinArray[],		// AO0��AO1ԭ���ϻ�����,���ڷ��ز�����ԭ�����ݣ�ȡֵ����Ϊ[-32768, 32767]
									U32 nWriteSampsPerChan,	// ÿͨ������д��ĵ���(��λ����), =NULL,��ʾ���뷵��
									U32* pSampsPerChanWritten,// ����ÿͨ��ʵ��д��ĵ���(��λ����), =NULL,��ʾ���뷵��
									U32* pAvailSampsPerChan,// ���ص�ǰ��д��Ĳ�������, =NULL,��ʾ���뷵��
									F64 fTimeout);			// ��ʱʱ�䣬��λ����(S)

	BOOL DEVLIB WINAPI USB3102A_AO_ReadbackAnalog(			// �ض�����AOͨ���ĵ�ǰ���ɵ�ģ��������(��ѹ��������)(Read back analog data from the task)
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									F64 fAnlgArray[2]);		// fAnlgArray[0]=AO0��ѹ����,fAnlgArray[1]=AO1��ѹ����, ȡֵ��������Ӧͨ���Ĳ�����Χ����

	BOOL DEVLIB WINAPI USB3102A_AO_ReadbackBinary(			// �ض�����AOͨ���ĵ�ǰ���ɵ�ģ��������(������ԭ����������)(Read back binary data from the task)
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									I16 nBinArray[2]);		// nBinArray[0]=AO0ԭ������,nBinArray[1]=AO1ԭ������, ȡֵ����[-32768, 32767]

	BOOL DEVLIB WINAPI USB3102A_AO_StopTask(HANDLE hDevice);	// ֹͣ��������

	BOOL DEVLIB WINAPI USB3102A_AO_ReleaseTask(HANDLE hDevice);// �ͷ���������

	// ========================= AO������������ =========================
	BOOL DEVLIB WINAPI USB3102A_AO_GetMainInfo(				// ���AO����Ҫ��Ϣ
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									PUSB3102A_AO_MAIN_INFO pMainInfo); // ���AO��Ҫ��Ϣ

	BOOL DEVLIB WINAPI USB3102A_AO_GetVoltRangeInfo(				// ��ò�����Χ�ĸ�����Ϣ(���ȡ��ֱ��ʡ�����)
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									U32 nChannel,			// AO����ͨ����
									U32 nSampleRange,		// ������Χѡ��[0, 0]
									PUSB3102A_AO_VOLT_RANGE_INFO pRangeInfo); // ������Χ��Ϣ

	BOOL DEVLIB WINAPI USB3102A_AO_GetRateInfo(				// ��ò���������Ϣ
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									PUSB3102A_AO_SAMP_RATE_INFO pRateInfo); // ����������Ϣ

	BOOL DEVLIB WINAPI USB3102A_AO_ScaleBinToVolt(			// ����ָ��������Χ������ԭ�����������ɵ�ѹ����,����ʵ�������ĵ���
									PUSB3102A_AO_VOLT_RANGE_INFO pRangeInfo, // ��ǰת��������Ҫ�Ĳ�����Χ��Ϣ
									F64 fVoltArray[],		// ��ѹ������,���ڷ���������ĵ�ѹ����,��λV
									I16 nBinArray[],		// ������ԭ�뻺����,���ڴ�������ǰ��ԭ�����ݣ�ȡֵ����Ϊ[-32768, 32767], (��ָĳ��ͨ������������)
									U32 nScaleSamps,		// ���������Ĳ�������
									U32* pSampsScaled);		// ����ʵ�������Ĳ�������

	BOOL DEVLIB WINAPI USB3102A_AO_ScaleVoltToBin(			// ����ָ��������Χ����ѹ���������ɲ���ԭ������,����ʵ�������ĵ���
									PUSB3102A_AO_VOLT_RANGE_INFO pRangeInfo, // ��ǰת��������Ҫ�Ĳ�����Χ��Ϣ
									I16 nBinArray[],		// ԭ�뻺����,���ڷ����������ԭ�����ݣ�ȡֵ����Ϊ[-32768, 32767], (��ָĳ��ͨ������������)
									F64 fVoltArray[],		// ��ѹ������,���ڴ�������ǰ�ĵ�ѹ����,��λV
									U32 nScaleSamps,		// ���������Ĳ�������
									U32* pSampsScaled);		// ����ʵ�������Ĳ�������

	BOOL DEVLIB WINAPI USB3102A_AO_VerifyParam(				// У��AO��������(Verify Parameter),�����ڳ�ʼ����������ǰ���ô˺���У��������Ϸ���
									HANDLE hDevice,			// �豸������,����DEV_Create()��������
									PUSB3102A_AO_PARAM pAOParam);// ��У���AO��������

	BOOL DEVLIB WINAPI USB3102A_AO_LoadParam(HANDLE hDevice, PUSB3102A_AO_PARAM pAOParam); // ��USB3102A.ini�м���AO����
	BOOL DEVLIB WINAPI USB3102A_AO_SaveParam(HANDLE hDevice, PUSB3102A_AO_PARAM pAOParam); // ����AO������USB3102A.ini
	BOOL DEVLIB WINAPI USB3102A_AO_ResetParam(HANDLE hDevice, PUSB3102A_AO_PARAM pAOParam); // ����ǰAO������λ������ֵ

	// ############################ CTR���������� ############################
	BOOL DEVLIB WINAPI USB3102A_CTR_InitTask(			// ��ʼ�ɼ�����
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nChannel,		// ͨ����(���豸����1��ͨ��,�����0)
									PUSB3102A_CTR_PARAM pCTRParam); // ��������

	BOOL DEVLIB WINAPI USB3102A_CTR_StartTask(			// ��ʼ�ɼ�����
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nChannel);		// ͨ����(���豸����1��ͨ��,�����0)

    BOOL DEVLIB WINAPI USB3102A_CTR_ReadCounter(			// ��ȡ�������ĵ�ǰ����ֵ
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nChannel,		// ͨ����(���豸����1��ͨ��,�����0)
									U32* pCountVal);	// ����ֵ

	BOOL DEVLIB WINAPI USB3102A_CTR_StopTask(			// ֹͣ(����ͣ)�ɼ�����
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nChannel);		// ͨ����(���豸����1��ͨ��,�����0)

	BOOL DEVLIB WINAPI USB3102A_CTR_ReleaseTask(			// �ͷŲɼ�����
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nChannel);		// ͨ����(���豸����1��ͨ��,�����0)

	// ############################ DI��DO�������������ʵ�ֺ��� ############################
	BOOL DEVLIB WINAPI USB3102A_DIO_InitTask(			// ��ʼ��DIO����
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nPort,			// �˿ں�, ȡֵ��ΧΪ[0, 1]
									PUSB3102A_DIO_PARAM pDIOParam); // ��������

	BOOL DEVLIB WINAPI USB3102A_DIO_GetParam(			// ��ȡDIOӲ������
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nPort,			// �˿ں�, ȡֵ��ΧΪ[0, 1]
									PUSB3102A_DIO_PARAM pDIOParam);	// ���������������ֵ

	BOOL DEVLIB WINAPI USB3102A_DIO_ReadPort(			// ���������˿�ֵ(Read Digital Port Value)
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nPort,			// �˿ں�, ȡֵ��ΧΪ[0, 1]
									U32* pPortData);	// ���صĶ˿�����, ��ЧλBit[7:0]

    BOOL DEVLIB WINAPI USB3102A_DIO_WritePort(			// д�������˿�ֵ(Write Digital Port Value)
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nPort,			// �˿ں�, ȡֵ��ΧΪ[0, 1]
									U32 nPortData);		// �˿�����, ��ЧλBit[7:0]

	BOOL DEVLIB WINAPI USB3102A_DIO_ReadLines(			// ���������˿�ֵ(Read Digital Port Value)
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nPort,			// �˿ں�, ȡֵ��ΧΪ[0, 1]
									U32 bLineDataArray[8]);// �����ݻ�����, ͬʱ���ض˿��и��ߵ�״ֵ̬bLineDataArray[n]=0:��ʾ��(���)״̬, =1��ʾ��(���)״̬

    BOOL DEVLIB WINAPI USB3102A_DIO_WriteLines(			// д�������˿�ֵ(Write Digital Port Value)
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nPort,			// �˿ں�, ȡֵ��ΧΪ[0, 1]
									U32 bLineDataArray[8]); // �����ݻ�����, �˿��и��ߵ�״ֵ̬bLineDataArray[n]=0:��ʾ��(���)״̬, =1��ʾ��(���)״̬

    BOOL DEVLIB WINAPI USB3102A_DIO_ReadLine(			// ����ֵ
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nPort,			// �˿ں�, ȡֵ��ΧΪ[0, 1]
									U32 nLine,			// �ߺ�,ȡֵ��ΧPort0ʱ[0, 7]�� Port1ʱ[0, 7]
									U32* pLineData);	// ������, ȡֵ0��1 

    BOOL DEVLIB WINAPI USB3102A_DIO_WriteLine(			// д��ֵ
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nPort,			// �˿ں�, ȡֵ��ΧΪ[0, 1]
									U32 nLine,			// �ߺ�,ȡֵ��ΧPort0ʱ[0, 7]�� Port1ʱ[0, 7]
									U32 bLineData);		// ��ֵ,ȡֵ0��1

	BOOL DEVLIB WINAPI USB3102A_DIO_ReleaseTask(			// �ͷ�DIO����
									HANDLE hDevice,		// �豸������,����DEV_Create()��������
									U32 nPort);			// �˿ں�, ȡֵ��ΧΪ[0, 1]

#ifdef __cplusplus
}
#endif

#ifndef _CVIDEF_H
	// �Զ������������������
	#ifndef _USB3102A_DRIVER_
		#ifndef LOAD_USB3102A_LIB // ���û�м���LIB��
			#define LOAD_USB3102A_LIB
			#pragma comment(lib, "USB3102A.lib")
			#pragma message("Welcome to use ART products!")
			#pragma message("Linked with USB3102A.lib successfully!")
			#pragma message("Linked with USB3102A.dll successfully!")
		#endif // LOAD_USB3102A_LIB
	#endif // _USB3102A_DRIVER_
#endif // _CVIDEF_H

#endif // _USB3102A_DEVICE_