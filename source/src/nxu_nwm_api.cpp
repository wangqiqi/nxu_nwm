#include "nxu_nwm_api.h"
#include "nxu_nwm_gconfig.h"
#include "nxu_nwm_utils.h"

using namespace nxu_nwm_utils;

// 算法句柄结构体
typedef struct _nxu_nwm_handle_s
{
    CNXUNWMUtils *__nwmobj;
} NXU_NWM_Handle_S;

/**
 * @brief 创建算法句柄，加载算法模型，只用创建一次，就可以循环使用，每一次创建对应一次销毁
 *
 * @param  phNWM            算法句柄
 * @return NXU_EXPORTS
 */
NXU_STATUS_CODE_E NXU_NWM_Create(NXU_HANDLE *phNWM)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;

    // 创建nwm实例
    NXU_NWM_Handle_S *hNWM = new NXU_NWM_Handle_S;
    if (NULL == hNWM)
    {
        return NXU_FAILURE;
    }

    hNWM->__nwmobj = new CNXUNWMUtils();
    if (NULL == hNWM->__nwmobj)
    {
        return NXU_FAILURE;
    }

    // 返回句柄
    *phNWM = (NXU_HANDLE)hNWM;

    return s32Ret;
}


NXU_STATUS_CODE_E NXU_NWM_CreateWithModel(NXU_HANDLE* phNWM,
    char* modelPath)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;

    // 创建nwm实例
    NXU_NWM_Handle_S* hNWM = new NXU_NWM_Handle_S;
    if (NULL == hNWM)
    {
        return NXU_FAILURE;
    }

    hNWM->__nwmobj = new CNXUNWMUtils(modelPath);
    if (NULL == hNWM->__nwmobj)
    {
        return NXU_FAILURE;
    }

    // 返回句柄
    *phNWM = (NXU_HANDLE)hNWM;

    return s32Ret;
}


/**
 * @brief 销毁算法句柄，使用完后记得销毁算法句柄
 *
 * @param  hNWM             算法句柄
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_NWM_Destroy(NXU_HANDLE hNWM)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;
    NXU_NWM_Handle_S *pHandle = (NXU_NWM_Handle_S *)hNWM;
    if (NULL == pHandle)
    {
        return NXU_FAILURE;
    }

    if (NULL == pHandle->__nwmobj)
    {
        return NXU_FAILURE;
    }
    else
    {
        delete pHandle->__nwmobj;
        pHandle->__nwmobj = NULL;
    }

    delete pHandle;
    pHandle = NULL;

    return s32Ret;
}

/**
 * @brief 获取算法的版本号
 *
 * @param  pcLibVerStr      算法库版本号
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_NWM_GetLibVer(NXU_S8 *pcLibVerStr)
{

    // 获取版本号
    std::cout << "\n=============== LIB INFO =====================\n";
    std::cout << "PROJECT_NAME: " << NWM_PROJECT_NAME << std::endl;
    std::cout << "LIB_VERSION: " << NWM_VERSION << std::endl;
    std::cout << "LIB_COMMIT_MD5: " << NWM_COMMIT << std::endl;
    std::cout << "BUILD_DATETIME: " << NWM_DATETIME << std::endl;
    std::cout<< "==============================================\n";

    sprintf((char*) pcLibVerStr, "%s-%s-%s-%s", NWM_PROJECT_NAME, 
            NWM_VERSION, NWM_COMMIT, NWM_DATETIME);
            
    return NXU_SUCCESS;
}

/**
 * @brief 算法处理
 *
 * @param  hNWM             算法句柄
 * @param  pstImage         图像数据
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_NWM_Process(NXU_HANDLE hNWM, const NXU_IMAGE_S *pstImage)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;

    NXU_NWM_Handle_S *pHandle = (NXU_NWM_Handle_S *)hNWM;
    if (NULL == pHandle || NULL == pstImage || NULL == pstImage->pData)
    {
        return NXU_FAILURE;
    }

#ifdef SHOW_DEBUG_TIME
    double dtStart, dtEnd;
    dtStart = __get_current_time_proc();
#endif // SHOW_DEBUG_TIME

    // 处理图像
    s32Ret = (NXU_STATUS_CODE_E)(pHandle->__nwmobj->process(pstImage));
    if (s32Ret != NXU_SUCCESS)
    {
        return s32Ret;
    }

#ifdef SHOW_DEBUG_TIME
    dtEnd = __get_current_time_proc();
    std::cout << "++++++  In NWM API: Process Time Span ++++++ " << (dtEnd - dtStart) << " ms" << std::endl;
    dtStart = __get_current_time_proc();
#endif // SHOW_DEBUG_TIME

    return s32Ret;
}

/**
 * @brief 获取结果
 *
 * @param  hNWM             算法句柄
 * @param  pstNWMResult     NWM结果
 * @return 算法状态
 */
NXU_STATUS_CODE_E NXU_NWM_GetResults(NXU_HANDLE hNWM, NXU_NWM_Result_S *pstNWMResult)
{
    NXU_STATUS_CODE_E s32Ret = NXU_SUCCESS;
    NXU_NWM_Handle_S *pHandle = (NXU_NWM_Handle_S *)hNWM;
    if (NULL == pHandle || NULL == pstNWMResult)
    {
        return NXU_FAILURE;
    }

    // 获取结果，通过参数返回出来
    s32Ret = (NXU_STATUS_CODE_E)(pHandle->__nwmobj->get_results(pstNWMResult));

    return s32Ret;
}
