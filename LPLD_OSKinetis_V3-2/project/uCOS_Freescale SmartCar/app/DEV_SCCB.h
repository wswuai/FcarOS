/**
 * @file DEV_SCCB.h
 * @version 0.1[By LPLD]
 * @date 2013-09-24
 * @brief SCCB�豸��������
 *
 * ���Ľ���:�ɸ���ʵ��Ӳ���޸�
 *
 * ��Ȩ����:�����������µ��Ӽ������޹�˾
 * http://www.lpld.cn
 * mail:support@lpld.cn
 *
 * @par
 * ����������������[LPLD]������ά������������ʹ���߿���Դ���롣
 * �����߿���������ʹ�û��Դ���롣�����μ�����ע��Ӧ���Ա�����
 * ���ø��Ļ�ɾ��ԭ��Ȩ���������������ο����߿��Լ�ע���ΰ�Ȩ�����ߡ�
 * ��Ӧ�����ش�Э��Ļ����ϣ�����Դ���롢���ó��۴��뱾��
 * �������²���������ʹ�ñ��������������κ��¹ʡ��������λ���ز���Ӱ�졣
 * ����������������͡�˵��������ľ���ԭ�����ܡ�ʵ�ַ�����
 * ������������[LPLD]��Ȩ�������߲��ý�������������ҵ��Ʒ��
 */
#ifndef __DEV_SCCB_H__
#define __DEV_SCCB_H__

#include "common.h"
   
#define ADR_OV7670      0x42

/********�û����޸�ֵ ��ʼ***********/
//����SCCB�豸��ַ
#define SCCB_DEV_ADR    ADR_OV7670
//����SCL��SDA������
#define SCCB_SCL        PTC5_O  
#define SCCB_SDA_O      PTC6_O 
#define SCCB_SDA_I      PTC6_I 
//����SDA�������
#define SCCB_SDA_OUT()  DDRC6=1
#define SCCB_SDA_IN()   DDRC6=0
/********�û����޸�ֵ ����***********/

#define SCCB_DELAY()	LPLD_SCCB_Delay(10000)	

void LPLD_SCCB_Init(void);
uint8 LPLD_SCCB_WriteReg(uint16, uint8);
uint8 LPLD_SCCB_ReadReg(uint8, uint8*, uint16);

#endif 
