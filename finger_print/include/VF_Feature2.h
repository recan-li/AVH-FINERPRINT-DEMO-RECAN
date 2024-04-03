#ifndef	__VFFEATURE_H__
#define __VFFEATURE_H__

#include "VF_Type.h"
#include "VF_Global.h"

#if (CFG_FEATURE_V2)

//
//	EncodeFeature: 对指纹特征进行压缩编码
//
sint32	EncodeFeature(FEATUREPTR lpFeature, uint8 *lpData)
{
/////////////////////////////////////////////////////////////////////
//	lpFeature: [in] 要编码的指纹特征指针
//  lpData: [out] 压缩编码后的指纹特征缓冲区指针
/////////////////////////////////////////////////////////////////////
	sint32	i;
	sint32	totalLen = 0;
	sint32	len = 0;
	sint32	x, y, o, a1, a2, a3;
	uint8	v, t, mn;
	sint32  boSize = 0;

	if(lpData == NULL || lpFeature == NULL)
		return 0;
	
	// 特征点数目
	mn = (uint8)lpFeature->MinutiaNum;
	// 特征总长度
	totalLen = 4 + 7*mn;
	// 第一个字节为特征总长度
	lpData[0] = (uint8)(totalLen/256);
	lpData[1] = (uint8)(totalLen - lpData[0]*256);
	// 第二个字节为特征点数目
	lpData[2] = mn;

	len = 3;
	
	// 对每个特征点进行压缩
	for(i = 0; i < mn; i++)
	{
		x = lpFeature->MinutiaArr[i].x;		// 9位表示
		y = lpFeature->MinutiaArr[i].y;		// 9位表示
		o = lpFeature->MinutiaArr[i].Direction; // 9位表示
		a1 = lpFeature->MinutiaArr[i].Triangle[0]; // 9位表示
		a2 = lpFeature->MinutiaArr[i].Triangle[1]; // 9位表示
		a3 = lpFeature->MinutiaArr[i].Triangle[2]; // 9位表示
		t = (uint8)lpFeature->MinutiaArr[i].Type;	// 2位表示

		lpData[len] = (uint8)(x >> 1);
		lpData[len+1] = (uint8)((x & 0x1) << 7) + (uint8)(y >> 2);
		lpData[len+2] = (uint8)((y & 0x3) << 6) + (uint8)(o >> 3);
		lpData[len+3] = (uint8)((o & 0x7) << 5) + (uint8)(a1 >> 4);
		lpData[len+4] = (uint8)((a1 & 0xF) << 4) + (uint8)(a2 >> 5);
		lpData[len+5] = (uint8)((a2 & 0x1F) << 3) + (uint8)(a3 >> 6);
		lpData[len+6] = (uint8)((a3 & 0x3F) << 2) + (uint8)(t && 0x3);

		len += 7;
	}
	
	// 计算校验位
	v = 0;
	for(i = 0; i < totalLen-1; i++)
	{
		v = v ^ lpData[i];
	}
	// 最后一个字节放校验字节
	lpData[len] = v;


	return totalLen;
}

//
//	DecodeFeature: 对指纹特征进行解码
//
sint32	DecodeFeature(uint8 *lpData, FEATUREPTR lpFeature)
{
/////////////////////////////////////////////////////////////////////
//  lpData: [in] 压缩编码的指纹特征缓冲区指针
//	lpFeature: [out] 解码后的指纹特征指针
/////////////////////////////////////////////////////////////////////
	sint32	i;
	sint32	totalLen = 0;
	sint32	len = 0;
	uint8	v;
	sint32	boSize = 0;
	if(lpData == NULL || lpFeature == NULL)
		return 1;

	memset((void *)lpFeature, 0, sizeof(FEATURE));
	// 第一个字节为特征总长
	totalLen = (sint32)lpData[0]*256 + lpData[1];
	// 计算校验位
	v = 0;
	for(i = 0; i < totalLen; i++)
	{
		v = v ^ lpData[i];
	}
	if(v != 0)	// 校验失败则返回错误
		return 2;

	// 第二个字节为指纹特征点数目
	lpFeature->MinutiaNum = (uint8)lpData[2]; 
	
	len = 3;

	// 解码
	for(i = 0; i < lpFeature->MinutiaNum; i++)
	{
		lpFeature->MinutiaArr[i].x = (sint32)(((sint32)lpData[len])<<1) + 
								   (sint32)(((sint32)(lpData[len+1] & 0x80))>>7);
		lpFeature->MinutiaArr[i].y = (sint32)(((sint32)(lpData[len+1] & 0x7F))<<2) + 
			                       (sint32)(((sint32)(lpData[len+2] & 0xC0))>>6);
		lpFeature->MinutiaArr[i].Direction = (sint32)(((sint32)(lpData[len+2] & 0xF))<<5) + 
			                       (sint32)(((sint32)(lpData[len+3] & 0xF8))>>3);
		lpFeature->MinutiaArr[i].Type = (uint8)(lpData[len+3] & 0x7);

		len += 7;
	}

	return 0;
}

#endif

#endif
