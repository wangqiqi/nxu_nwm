#ifndef __NXU_NWM_API_H__
#define __NXU_NWM_API_H__

#include "nxu_common.h"
#include <stdio.h>

/* 最多目标数 */
#define MAX_NWM_TGT_NUM 64

// /* 最多事件数 */
#define MAX_NWM_EVT_NUM 64

#ifdef __cplusplus
extern "C"
{
#endif

    /* ********************************************************************** */
    /* ..............................  Para Start    ........................ */
    /* ********************************************************************** */
    /* 目标类型 */
    typedef enum __nxu_nwm_tgt_type_e
    {
        NXU_NWM_UNKNOWN = -1,
        NXU_NWM_FACE = 0,
        NXU_NWM_MASK = 1
        // TODO: add your target type
    } NXU_NWM_TgtType_E;

    /* 目标个体 */
    typedef struct __nxu_nwm_tgt_item_s
    {
        NXU_NWM_TgtType_E enTgtType; /* 目标类型 */
        NXU_RECT_S stRect;           /* 目标包围框 */
        NXU_FLOAT f32Prob;           /* 目标概率 */
    } NXU_NWM_TgtItem_S;

    /* 事件类型 */
    typedef enum __nxu_nwm_evt_type_e
    {
        NXU_NWM_EVT_UNKNOWN = -1,
        // TODO: add your event type
    } NXU_NWM_EvtType_E;

    /* 事件个体 */
    typedef struct __nxu_nwm_evt_item_s
    {
        NXU_NWM_EvtType_E enEvtType; /* 事件类型 */
        NXU_RECT_S stRect;           /* 事件位置 */
        NXU_FLOAT f32Prob;           /* 事件概率 */
    } NXU_NWM_EvtItem_S;

    /* 结果 */
    typedef struct __nxu_nwm_results_s
    {
        NXU_S32 s32TgtNum;                              /* 目标数目 */
        NXU_NWM_TgtItem_S astTgtItems[MAX_NWM_TGT_NUM]; /* 目标集合 */

        NXU_S32 s32EvtNum;                              /* 事件数目 */
        NXU_NWM_EvtItem_S astEvtItems[MAX_NWM_EVT_NUM]; /* 事件集合 */
    } NXU_NWM_Result_S;
    /* ********************************************************************** */
    /* ..............................  Para End      ........................ */
    /* ********************************************************************** */

    /* ********************************************************************** */
    /* ..............................  API  Start    ........................ */
    /* ********************************************************************** */
    /**
     * @brief 创建算法句柄，加载算法模型，只用创建一次，就可以循环使用，每一次创建对应一次销毁
     *
     * @param  phNWM            算法句柄
     * @return NXU_EXPORTS
     */
    NXU_EXPORTS NXU_STATUS_CODE_E NXU_NWM_Create(NXU_HANDLE *phNWM);

    /**
     * @brief 带模型路径的算法创建
     *
     * @param  phNWM            算法句柄，从外部接受句柄指针
     * @param  modelPath        模型路径，类似于 models/NWM.weights，指定对应模型路径
     *              最好指定绝对路径，方便找到，这里请注意 windows 和 linux 路径不同
     *              注意：路径中最好不要有空格或者中文（宽字符）
     * new API interface : 2024.4.19
     * @return NXU_EXPORTS
     */
    NXU_EXPORTS NXU_STATUS_CODE_E NXU_NWM_CreateWithModel(NXU_HANDLE *phNWM,
                                                          char *modelPath);

    /**
     * @brief 销毁算法句柄，使用完后记得销毁算法句柄
     *
     * @param  hNWM             算法句柄
     * @return 算法状态
     */
    NXU_EXPORTS NXU_STATUS_CODE_E NXU_NWM_Destroy(NXU_HANDLE hNWM);

    /**
     * @brief 获取算法的版本号
     *
     * @param  pcLibVerStr      算法库版本号
     * @return 算法状态
     */
    NXU_EXPORTS NXU_STATUS_CODE_E NXU_NWM_GetLibVer(NXU_S8 *pcLibVerStr);

    /**
     * @brief 算法处理
     *
     * @param  hNWM             算法句柄
     * @param  pstImage         图像数据
     * @return 算法状态
     */
    NXU_EXPORTS NXU_STATUS_CODE_E NXU_NWM_Process(NXU_HANDLE hNWM,
                                                  const NXU_IMAGE_S *pstImage);

    /**
     * @brief 获取结果
     *
     * @param  hNWM             算法句柄
     * @param  pstNWMResult     结果
     * @return 算法状态
     */
    NXU_EXPORTS NXU_STATUS_CODE_E NXU_NWM_GetResults(NXU_HANDLE hNWM,
                                                     NXU_NWM_Result_S *pstNWMResult);
    /* ********************************************************************** */
    /* ..............................  API  END.............................. */
    /* ********************************************************************** */

#ifdef __cplusplus
}
#endif

#endif // !__NXU_NWM_API_H__
