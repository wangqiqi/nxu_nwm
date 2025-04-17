#include "nxu_nwm_utils.h"

namespace nxu_nwm_utils
{
    /* 构造函数 */
    CNXUNWMUtils::CNXUNWMUtils()
    {
        /* 清理目标信息 */
        __clean_objs();

        /* 初始化模型，加载模型参数 */
        m_PCYOLOV8DetNet = new CYOLOV8_ORT_Detector(NWM_MODEL_DEFAULT);
    }

    /* 用路径构造初始化模型 */
    CNXUNWMUtils::CNXUNWMUtils(const std::string &modelPath)
    {
        /* 清理目标信息 */
        __clean_objs();

        /* 初始化模型，加载模型参数 */
        m_PCYOLOV8DetNet = new CYOLOV8_ORT_Detector(modelPath);
    }

    // 删除目标检测器
    CNXUNWMUtils::~CNXUNWMUtils()
    {
        /* 清理目标信息 */
        __clean_objs();

        delete m_PCYOLOV8DetNet;
        m_PCYOLOV8DetNet = NULL;
    }

    /* 清理目标信息 */
    void CNXUNWMUtils::__clean_objs()
    {
        m_vecObjs.clear();
        std::vector<NXU_DetTarget_S>().swap(m_vecObjs);
    }

    // DONE
    NXU_STATUS_CODE_E CNXUNWMUtils::process(const NXU_IMAGE_S *pstImage)
    {
        int ret;
        // #ifdef SHOW_DEBUG_TIME
        //     double dtStart, dtEnd;
        //     dtStart = __get_current_time_proc();
        // #endif // SHOW_DEBUG_TIME

        // 图像转换
        m_bgrIn = nxu_utils::convert2CvMat(pstImage);

        // #ifdef SHOW_DEBUG_TIME
        //     dtEnd = __get_current_time_proc();
        //     std::cout << "    ++++++  In CNXUNWMUtils----Process Convert Image Time Span ++++++ " << (dtEnd - dtStart) << " ms" << std::endl;
        //     dtStart = __get_current_time_proc();
        // #endif // SHOW_DEBUG_TIME

        // __scaleIm(m_bgrIn, m_TgtSzbgrIn, TGT_IM_SZ, &m_fScale);

        // #ifdef SHOW_DEBUG_TIME
        //     dtEnd = __get_current_time_proc();
        //     std::cout << "    ++++++  In CNXUNWMUtils----Scale Image Time Span ++++++ " << (dtEnd - dtStart) << " ms" << std::endl;
        //     dtStart = __get_current_time_proc();
        // #endif // SHOW_DEBUG_TIME

        // 检测
        m_vecObjs.clear();
        std::vector<NXU_DetTarget_S>().swap(m_vecObjs);
        // ret = m_PCYOLOV8DetNet->process(m_TgtSzbgrIn, m_vecObjs);
        ret = m_PCYOLOV8DetNet->process(m_bgrIn, m_vecObjs);

        if (ret != 0)
        {
            return NXU_FAILURE;
        }

#ifdef SHOW_DEBUG_IMG
        // m_PCYOLOV5DetNet->draw_objects();
#endif

        // #ifdef SHOW_DEBUG_TIME
        //     dtEnd = __get_current_time_proc();
        //     std::cout << "++++++  In CNXUNWMUtils----Network Process Time Span ++++++ " << (dtEnd - dtStart) << " ms" << std::endl;
        //     dtStart = __get_current_time_proc();
        // #endif // SHOW_DEBUG_TIME

        NXU_RECT_S stBB;
        NXU_S32 s32TargetNum = m_vecObjs.size();
        if (s32TargetNum > MAX_NWM_TGT_NUM)
        {
            s32TargetNum = MAX_NWM_TGT_NUM;
        }

#ifdef SHOW_DEBUG_INFO
        std::cout << "Tgt Num: " << s32TargetNum << std::endl;
#endif

        m_stResults.s32TgtNum = s32TargetNum;
        for (int i = 0; i < s32TargetNum; i++)
        {
            const NXU_DetTarget_S &obj = m_vecObjs[i];
            NXU_NWM_TgtItem_S stItem;
            // stBB.stTopLeft.s32X = int(obj.rect.x / m_fScale);
            // stBB.stTopLeft.s32Y = int(obj.rect.y / m_fScale);
            // stBB.stBottomRight.s32X = int((obj.rect.width + obj.rect.x) / m_fScale);
            // stBB.stBottomRight.s32Y = int((obj.rect.height + obj.rect.y) / m_fScale);

            stBB.stTopLeft.s32X = int(obj.rect.x);
            stBB.stTopLeft.s32Y = int(obj.rect.y);
            stBB.stBottomRight.s32X = int((obj.rect.width + obj.rect.x));
            stBB.stBottomRight.s32Y = int((obj.rect.height + obj.rect.y));

            // clip values
            stBB.stTopLeft.s32X = stBB.stTopLeft.s32X > 0 ? stBB.stTopLeft.s32X : 0;
            stBB.stTopLeft.s32Y = stBB.stTopLeft.s32Y > 0 ? stBB.stTopLeft.s32Y : 0;
            stBB.stBottomRight.s32X = stBB.stBottomRight.s32X < (m_bgrIn.cols - 1) ? stBB.stBottomRight.s32X : (m_bgrIn.cols - 1);
            stBB.stBottomRight.s32Y = stBB.stBottomRight.s32Y < (m_bgrIn.rows - 1) ? stBB.stBottomRight.s32Y : (m_bgrIn.rows - 1);

            stItem.stRect = stBB;
            stItem.f32Prob = obj.f32Prob;

            // 类型不知道, 需要进行后处理判断
            stItem.enTgtType = (NXU_NWM_TgtType_E)(obj.s32labelID);
            m_stResults.astTgtItems[i] = stItem;
        }

        __post_process();

        // #ifdef SHOW_DEBUG_TIME
        //     dtEnd = __get_current_time_proc();
        //     std::cout << "    ++++++  In CNXUNWMUtils----Postprocess Time Span ++++++ " << (dtEnd - dtStart) << " ms" << std::endl;
        // #endif // SHOW_DEBUG_TIME

        return NXU_SUCCESS;
    }

    // 后处理，将目标转换为事件
    NXU_STATUS_CODE_E CNXUNWMUtils::__post_process()
    {
        // TODO: 后处理逻辑
        // 将目标分离到不同类型的目标集合里

        return NXU_SUCCESS;
    }

    // 获得结果
    NXU_STATUS_CODE_E CNXUNWMUtils::get_results(NXU_NWM_Result_S *pstResult)
    {
        if (NULL == pstResult)
        {
            return NXU_FAILURE;
        }

        // 将内部结果拷贝出去
        memcpy(pstResult, &m_stResults, sizeof(NXU_NWM_Result_S));

        return NXU_SUCCESS;
    }

    // 将图像转到指定大小
    void CNXUNWMUtils::__scaleIm(cv::Mat &src, cv::Mat &dst, int s32DesignedWH, float *scale)
    {
        int s32SrcW = src.cols;
        int s32SrcH = src.rows;
        int s32NewH, s32NewW;

        // 缩放比例
        float fScale;

        // 将长边转到指定大小
        if (s32SrcW > s32SrcH)
        {
            fScale = (float)s32DesignedWH / s32SrcW;
            s32NewW = s32DesignedWH;
            s32NewH = int(fScale * s32SrcH);
        }
        else
        {
            fScale = (float)s32DesignedWH / s32SrcH;
            s32NewW = int(fScale * s32SrcW);
            s32NewH = s32DesignedWH;
        }

        cv::resize(src, dst, cv::Size(s32NewW, s32NewH));
        *scale = fScale;
    }

}