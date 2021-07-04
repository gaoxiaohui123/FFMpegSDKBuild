/*!
 * \copy
 *     Copyright (c)  2013, Cisco Systems
 *     All rights reserved.
 *
 *     Redistribution and use in source and binary forms, with or without
 *     modification, are permitted provided that the following conditions
 *     are met:
 *
 *        * Redistributions of source code must retain the above copyright
 *          notice, this list of conditions and the following disclaimer.
 *
 *        * Redistributions in binary form must reproduce the above copyright
 *          notice, this list of conditions and the following disclaimer in
 *          the documentation and/or other materials provided with the
 *          distribution.
 *
 *     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *     "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *     FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *     COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *     INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *     BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *     CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *     LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *     ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *     POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "WelsFrameWork.h"
#include "../denoise/denoise.h"
#include "../downsample/downsample.h"
#include "../scrolldetection/ScrollDetection.h"
#include "../scenechangedetection/SceneChangeDetection.h"
#include "../vaacalc/vaacalculation.h"
#include "../backgrounddetection/BackgroundDetection.h"
#include "../adaptivequantization/AdaptiveQuantization.h"
#include "../complexityanalysis/ComplexityAnalysis.h"
#include "../imagerotate/imagerotate.h"
#include "util.h"
#include "inc.h"
#include "../../../encoder/core/inc/sample.h"//added by gxh
#include "../../../encoder/core/inc/wels_func_ptr_def.h"
//#include "sad_common.h" //added by gxh
/* interface API implement */



EResult WelsCreateVpInterface (void** ppCtx, int iVersion) {
  if (iVersion & 0x8000)
    return WelsVP::CreateSpecificVpInterface ((IWelsVP**)ppCtx);
  else if (iVersion & 0x7fff)
    return WelsVP::CreateSpecificVpInterface ((IWelsVPc**)ppCtx);
  else
    return RET_INVALIDPARAM;
}

EResult WelsDestroyVpInterface (void* pCtx, int iVersion) {
  if (iVersion & 0x8000)
    return WelsVP::DestroySpecificVpInterface ((IWelsVP*)pCtx);
  else if (iVersion & 0x7fff)
    return WelsVP::DestroySpecificVpInterface ((IWelsVPc*)pCtx);
  else
    return RET_INVALIDPARAM;
}

WELSVP_NAMESPACE_BEGIN

///////////////////////////////////////////////////////////////////////

EResult CreateSpecificVpInterface (IWelsVP** ppCtx) {
  EResult  eReturn = RET_FAILED;

  CVpFrameWork* pFr = new CVpFrameWork (1, eReturn);
  if (pFr) {
    *ppCtx  = (IWelsVP*)pFr;
    eReturn = RET_SUCCESS;
  }

  return eReturn;
}

EResult DestroySpecificVpInterface (IWelsVP* pCtx) {
  delete pCtx;

  return RET_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

CVpFrameWork::CVpFrameWork (uint32_t uiThreadsNum, EResult& eReturn) {
  int32_t iCoreNum = 1;
  uint32_t uiCPUFlag = WelsCPUFeatureDetect (&iCoreNum);

  for (int32_t i = 0; i < MAX_STRATEGY_NUM; i++) {
    m_pStgChain[i] = CreateStrategy (WelsStaticCast (EMethods, i + 1), uiCPUFlag);
  }

  WelsMutexInit (&m_mutes);

  eReturn = RET_SUCCESS;
}

CVpFrameWork::~CVpFrameWork() {
  for (int32_t i = 0; i < MAX_STRATEGY_NUM; i++) {
    if (m_pStgChain[i]) {
      Uninit (m_pStgChain[i]->m_eMethod);
      delete m_pStgChain[i];
    }
  }

  WelsMutexDestroy (&m_mutes);
}

EResult CVpFrameWork::Init (int32_t iType, void* pCfg) {
  EResult eReturn   = RET_SUCCESS;
  int32_t iCurIdx    = WelsStaticCast (int32_t, WelsVpGetValidMethod (iType)) - 1;

  Uninit (iType);

  WelsMutexLock (&m_mutes);

  IStrategy* pStrategy = m_pStgChain[iCurIdx];
  if (pStrategy)
    eReturn = pStrategy->Init (0, pCfg);

  WelsMutexUnlock (&m_mutes);

  return eReturn;
}

EResult CVpFrameWork::Uninit (int32_t iType) {
  EResult eReturn        = RET_SUCCESS;
  int32_t iCurIdx    = WelsStaticCast (int32_t, WelsVpGetValidMethod (iType)) - 1;

  WelsMutexLock (&m_mutes);

  IStrategy* pStrategy = m_pStgChain[iCurIdx];
  if (pStrategy)
    eReturn = pStrategy->Uninit (0);

  WelsMutexUnlock (&m_mutes);

  return eReturn;
}

EResult CVpFrameWork::Flush (int32_t iType) {
  EResult eReturn        = RET_SUCCESS;

  return eReturn;
}

EResult CVpFrameWork::Process (int32_t iType, SPixMap* pSrcPixMap, SPixMap* pDstPixMap) {
  EResult eReturn        = RET_NOTSUPPORTED;
  EMethods eMethod    = WelsVpGetValidMethod (iType);
  int32_t iCurIdx    = WelsStaticCast (int32_t, eMethod) - 1;
  SPixMap sSrcPic;
  SPixMap sDstPic;
  memset (&sSrcPic, 0, sizeof (sSrcPic)); // confirmed_safe_unsafe_usage
  memset (&sDstPic, 0, sizeof (sDstPic)); // confirmed_safe_unsafe_usage

  if (pSrcPixMap) sSrcPic = *pSrcPixMap;
  if (pDstPixMap) sDstPic = *pDstPixMap;
  if (!CheckValid (eMethod, sSrcPic, sDstPic))
    return RET_INVALIDPARAM;

  WelsMutexLock (&m_mutes);

  IStrategy* pStrategy = m_pStgChain[iCurIdx];
  if (pStrategy)
    eReturn = pStrategy->Process (0, &sSrcPic, &sDstPic);

  WelsMutexUnlock (&m_mutes);

  return eReturn;
}

EResult CVpFrameWork::Get (int32_t iType, void* pParam) {
  EResult eReturn        = RET_SUCCESS;
  int32_t iCurIdx    = WelsStaticCast (int32_t, WelsVpGetValidMethod (iType)) - 1;

  if (!pParam)
    return RET_INVALIDPARAM;

  WelsMutexLock (&m_mutes);

  IStrategy* pStrategy = m_pStgChain[iCurIdx];
  if (pStrategy)
    eReturn = pStrategy->Get (0, pParam);

  WelsMutexUnlock (&m_mutes);

  return eReturn;
}

EResult CVpFrameWork::Set (int32_t iType, void* pParam) {
  EResult eReturn        = RET_SUCCESS;
  int32_t iCurIdx    = WelsStaticCast (int32_t, WelsVpGetValidMethod (iType)) - 1;

  if (!pParam)
    return RET_INVALIDPARAM;

  WelsMutexLock (&m_mutes);

  IStrategy* pStrategy = m_pStgChain[iCurIdx];
  if (pStrategy)
    eReturn = pStrategy->Set (0, pParam);

  WelsMutexUnlock (&m_mutes);

  return eReturn;
}

EResult CVpFrameWork::SpecialFeature (int32_t iType, void* pIn, void* pOut) {
  EResult eReturn        = RET_SUCCESS;

  return eReturn;
}

bool  CVpFrameWork::CheckValid (EMethods eMethod, SPixMap& pSrcPixMap, SPixMap& pDstPixMap) {
  bool eReturn = false;

  if (eMethod == METHOD_NULL)
    goto exit;

  if (eMethod != METHOD_COLORSPACE_CONVERT) {
    if (pSrcPixMap.pPixel[0]) {
      if (pSrcPixMap.eFormat != VIDEO_FORMAT_I420 && pSrcPixMap.eFormat != VIDEO_FORMAT_YV12)
        goto exit;
    }
    if (pSrcPixMap.pPixel[0] && pDstPixMap.pPixel[0]) {
      if (pDstPixMap.eFormat != pSrcPixMap.eFormat)
        goto exit;
    }
  }

  if (pSrcPixMap.pPixel[0]) {
    if (pSrcPixMap.sRect.iRectWidth <= 0 || pSrcPixMap.sRect.iRectWidth > MAX_WIDTH || pSrcPixMap.sRect.iRectHeight <= 0
        || pSrcPixMap.sRect.iRectHeight > MAX_HEIGHT)
      goto exit;
    if (pSrcPixMap.sRect.iRectTop >= pSrcPixMap.sRect.iRectHeight
        || pSrcPixMap.sRect.iRectLeft >= pSrcPixMap.sRect.iRectWidth || pSrcPixMap.sRect.iRectWidth > pSrcPixMap.iStride[0])
      goto exit;
  }
  if (pDstPixMap.pPixel[0]) {
    if (pDstPixMap.sRect.iRectWidth <= 0 || pDstPixMap.sRect.iRectWidth > MAX_WIDTH || pDstPixMap.sRect.iRectHeight <= 0
        || pDstPixMap.sRect.iRectHeight > MAX_HEIGHT)
      goto exit;
    if (pDstPixMap.sRect.iRectTop >= pDstPixMap.sRect.iRectHeight
        || pDstPixMap.sRect.iRectLeft >= pDstPixMap.sRect.iRectWidth || pDstPixMap.sRect.iRectWidth > pDstPixMap.iStride[0])
      goto exit;
  }
  eReturn = true;

exit:
  return eReturn;
}

IStrategy* CVpFrameWork::CreateStrategy (EMethods m_eMethod, int32_t iCpuFlag) {
  IStrategy* pStrategy = NULL;

  switch (m_eMethod) {
  case METHOD_COLORSPACE_CONVERT:
    //not support yet
    break;
  case METHOD_DENOISE:
    pStrategy = WelsDynamicCast (IStrategy*, new CDenoiser (iCpuFlag));
    break;
  case METHOD_SCROLL_DETECTION:
    pStrategy = WelsDynamicCast (IStrategy*, new CScrollDetection (iCpuFlag));
    break;
  case METHOD_SCENE_CHANGE_DETECTION_VIDEO:
  case METHOD_SCENE_CHANGE_DETECTION_SCREEN:
    pStrategy = BuildSceneChangeDetection (m_eMethod, iCpuFlag);
    break;
  case METHOD_DOWNSAMPLE:
    pStrategy = WelsDynamicCast (IStrategy*, new CDownsampling (iCpuFlag));
    break;
  case METHOD_VAA_STATISTICS:
    pStrategy = WelsDynamicCast (IStrategy*, new CVAACalculation (iCpuFlag));
    break;
  case METHOD_BACKGROUND_DETECTION:
    pStrategy = WelsDynamicCast (IStrategy*, new CBackgroundDetection (iCpuFlag));
    break;
  case METHOD_ADAPTIVE_QUANT:
    pStrategy = WelsDynamicCast (IStrategy*, new CAdaptiveQuantization (iCpuFlag));
    break;
  case METHOD_COMPLEXITY_ANALYSIS:
    pStrategy = WelsDynamicCast (IStrategy*, new CComplexityAnalysis (iCpuFlag));
    break;
  case METHOD_COMPLEXITY_ANALYSIS_SCREEN:
    pStrategy = WelsDynamicCast (IStrategy*, new CComplexityAnalysisScreen (iCpuFlag));
    break;
  case METHOD_IMAGE_ROTATE:
    pStrategy = WelsDynamicCast (IStrategy*, new CImageRotating (iCpuFlag));
    break;
  default:
    break;
  }

  return pStrategy;
}



WELSVP_NAMESPACE_END



#define HCSVC_API __attribute__ ((__visibility__("default")))

using namespace WelsVP;

const int32_t g_kiPixMapSizeInBits = sizeof (uint8_t) * 8;

typedef struct
{
    CDenoiser *denoise;
    SPixMap sSrcPixMap;
    int32_t iMethodIdx;
}ImgDenoiseObj;
/*
enum {
BLOCK_16x16    = 0,
BLOCK_16x8     = 1,
BLOCK_8x16     = 2,
BLOCK_8x8      = 3,
BLOCK_4x4      = 4,
BLOCK_8x4      = 5,
BLOCK_4x8      = 6,
BLOCK_SIZE_ALL = 7
};
*/
typedef struct
{
    uint32_t uiCpuFlag;
    WelsEnc::SWelsFuncPtrList sFuncList;
    WelsEnc::PSampleSadSatdCostFunc *pfSampleSad;
    //int32_t(*fun_sad16x16)(uint8_t*, int32_t, uint8_t*, int32_t);
}SADObj;

void *CreateSadHnd(char *handle)
{
    long long *testp = (long long *)handle;
    SADObj *obj = (SADObj *)testp[0];
    if(!obj)
    {
        obj = new SADObj;
        int handle_size = sizeof(long long);
        memcpy(handle, &obj, handle_size);
        int32_t iCoreNum = 0;
        obj->uiCpuFlag = WelsCPUFeatureDetect (&iCoreNum);
        printf("CreateSadHnd: obj->uiCpuFlag= %d \n", obj->uiCpuFlag);
        printf("CreateSadHnd: iCoreNum= %d \n", iCoreNum);
        WelsInitSampleSadFunc (&obj->sFuncList, obj->uiCpuFlag);
        obj->pfSampleSad = obj->sFuncList.sSampleDealingFuncs.pfSampleSad;
        printf("CreateSadHnd: obj= %x \n", obj);
        printf("CreateSadHnd: obj->pfSampleSad= %x \n", obj->pfSampleSad);
    }
    return obj;
}
HCSVC_API
int ISADnxm(char *handle, int n, int m, uint8_t* pSample1, int32_t iStride1, uint8_t* pSample2, int32_t iStride2)
{
    int ret = 0;

    SADObj *obj = (SADObj *)CreateSadHnd(handle);
    //printf("ISADnxm: obj= %x \n", obj);
    WelsEnc::PSampleSadSatdCostFunc *pfSampleSad = obj->pfSampleSad;
    //printf("ISADnxm: pfSampleSad= %x \n", pfSampleSad);
    if(n == 16 && m == 16)
    {
        ret = pfSampleSad[BLOCK_16x16](pSample1, iStride1, pSample2, iStride2);
        //ret = (obj->fun_sad16x16)(pSample1, iStride1, pSample2, iStride2);
    }
    else if(n == 8 && m == 8)
    {
        //printf("ISADnxm: n=%d, m=%d \n", n, m);
        //printf("ISADnxm: pfSampleSad[BLOCK_8x8]= %x \n", pfSampleSad[BLOCK_8x8]);
        ret = pfSampleSad[BLOCK_8x8](pSample1, iStride1, pSample2, iStride2);
        //ret = (obj->fun_sad8x8)(pSample1, iStride1, pSample2, iStride2);
    }
    else if(n == 16 && m == 8)
    {
        ret = pfSampleSad[BLOCK_16x8](pSample1, iStride1, pSample2, iStride2);
        //ret = (obj->fun_sad16x8)(pSample1, iStride1, pSample2, iStride2);
    }
    else if(n == 8 && m == 16)
    {
        ret = pfSampleSad[BLOCK_8x16](pSample1, iStride1, pSample2, iStride2);
        //ret = (obj->fun_sad8x16)(pSample1, iStride1, pSample2, iStride2);
    }
    else if(n == 8 && m == 4)
    {
        ret = pfSampleSad[BLOCK_8x4](pSample1, iStride1, pSample2, iStride2);
        //ret = (obj->fun_sad8x4)(pSample1, iStride1, pSample2, iStride2);
    }
    else if(n == 4 && m == 8)
    {
        ret = pfSampleSad[BLOCK_4x8](pSample1, iStride1, pSample2, iStride2);
        //ret = (obj->fun_sad4x8)(pSample1, iStride1, pSample2, iStride2);
    }
    else if(n == 4 && m == 4)
    {
        ret = pfSampleSad[BLOCK_4x4](pSample1, iStride1, pSample2, iStride2);
        //ret = (obj->fun_sad4x8)(pSample1, iStride1, pSample2, iStride2);
    }
    return ret;
}
HCSVC_API
int ISADHndClose(char *handle)
{
    long long ret = 0;

    if(handle)
    {
        long long *testp = (long long *)handle;
        SADObj *obj = (SADObj *)testp[0];
        delete obj;
        testp[0] = 0;
        printf("ISADHndClose: ok \n");
    }
    return (int)ret;
}

HCSVC_API
int ICreateVideoDenoise(char *handle)
{
    long long ret = 0;

    if(handle)
    {
        ImgDenoiseObj *obj = new ImgDenoiseObj;
        int32_t iCoreNum = 1;
        uint32_t uiCPUFlag = WelsCPUFeatureDetect (&iCoreNum);
        printf("ICreateVideoDenoise: uiCPUFlag= %d \n", uiCPUFlag);
        printf("ICreateVideoDenoise: iCoreNum= %d \n", iCoreNum);
        //CDenoiser *
        obj->denoise = new CDenoiser(uiCPUFlag);
        obj->iMethodIdx = METHOD_DENOISE;
        memset (&obj->sSrcPixMap, 0, sizeof (obj->sSrcPixMap));

        ret = (long long)obj;
        //handle = (void *)obj;
        int handle_size = sizeof(long long);
        printf("ICreateVideoDenoise: handle_size= %d \n", handle_size);
        printf("ICreateVideoDenoise: obj= %x \n", obj);
        memcpy(handle, &ret, handle_size);
        long long *testp = (long long *)handle;
        printf("ICreateVideoDenoise: testp[0]= %x \n", testp[0]);
        //int id = (int)obj;//long long ???
        //obj->Obj_id = id;
        //ret = id;
    }

    return (int)(ret & 0x7FFFFFFF);
}
HCSVC_API
int IVideoDenoiseClose(char *handle)
{
    long long ret = 0;

    if(handle)
    {
        long long *testp = (long long *)handle;
        ImgDenoiseObj *obj = (ImgDenoiseObj *)testp[0];
        //CDenoiser *obj = (CDenoiser *)testp[0];
        //
        delete obj->denoise;
        delete obj;
        testp[0] = 0;
        printf("IVideoDenoiseClose: ok \n");
    }

    return (int)ret;
}
HCSVC_API
int IVideoDenoise(char *handle, unsigned char *data[3], int linesize[3], int width, int height)
{
    int ret = 0;

    if(handle)
    {
        long long *testp = (long long *)handle;
        ImgDenoiseObj *obj = (ImgDenoiseObj *)testp[0];

        //int32_t iMethodIdx = METHOD_DENOISE;
        //SPixMap sSrcPixMap;
        //memset (&sSrcPixMap, 0, sizeof (sSrcPixMap));
        obj->sSrcPixMap.pPixel[0] = data[0];//pSrc->pData[0];
        obj->sSrcPixMap.pPixel[1] = data[1];//pSrc->pData[1];
        obj->sSrcPixMap.pPixel[2] = data[2];//pSrc->pData[2];
        obj->sSrcPixMap.iSizeInBits = g_kiPixMapSizeInBits;
        obj->sSrcPixMap.sRect.iRectWidth = width;//kiWidth;
        obj->sSrcPixMap.sRect.iRectHeight = height;//kiHeight;
        obj->sSrcPixMap.iStride[0] = linesize[0];//pSrc->iLineSize[0];
        obj->sSrcPixMap.iStride[1] = linesize[1];//pSrc->iLineSize[1];
        obj->sSrcPixMap.iStride[2] = linesize[2];//pSrc->iLineSize[2];
        obj->sSrcPixMap.eFormat = VIDEO_FORMAT_I420;

        //printf("IVideoDenoise: width=%d \n", width);
        //printf("IVideoDenoise: height=%d \n", height);
        //printf("IVideoDenoise: linesize[0]=%d \n", linesize[0]);
        //printf("IVideoDenoise: linesize[1]=%d \n", linesize[1]);
        //printf("IVideoDenoise: linesize[2]=%d \n", linesize[2]);
        ret = obj->denoise->Process (obj->iMethodIdx, &obj->sSrcPixMap, NULL);//CDenoiser::Process (int32_t iType, SPixMap* pSrc, SPixMap* dst)
        //printf("IVideoDenoise: ret=%d \n", ret);

    }

    return ret;
}