//
// Created by zhou on 24-3-3.
//

#ifndef BASIC_KITS_DATA_BASICS_H
#define BASIC_KITS_DATA_BASICS_H

#include <cstdint>
#include <cstring>

#define ZZQ_DT_CHANNELS(type_id)      (type_id/8 + 1)
#define ZZQ_DT_BYTES(type_id)         (1 << (type_id%8 /2))
#define ZZQ_IMAGE_BUFF_SIZE(data_info)  \
    (data_info.width * data_info.height * ZZQ_DT_CHANNELS(data_info.type_id) * ZZQ_DT_BYTES(data_info.type_id))

#define ZZQ_DATA_ID_TO_NAME(data_id)    ((char *)&(data_id))

#define ZZQ_DATA_NAME_TO_ID(name, data_id)   \
    do {                                     \
        memcpy(&data_id, name, ZZQ_GET_MIN(sizeof(data_id), strlen(name))); \
    } while (0);


namespace zzq {

    struct DataBasics
    {
        uint64_t id; // 数据的识别ID(由name转换而来)
        char name[8]; // 数据的识别名称
        uint64_t length; // 数据的长度
        uint64_t reserved; //保留字
    };

    struct ImageBasics
    {
        uint64_t id; // 数据的识别ID(由name转换而来)
        char name[8]; // 数据的识别名称
        int32_t width; // 数据的宽度
        int32_t height; // 数据的长度
        int32_t type_id; // 数据的格式: 8UC3/16UC1
        int32_t reserved; //保留字
    };

    struct CamIntrinsic
    {
        float  fx; // 焦距
        float  fy; // 焦距
        float  cx; // 中心点
        float  cy; // 中心点
    };
    struct CamDistortion
    {
        float radial[4]; // 径向畸变
        float tangential[4]; // 切向畸变
    };
    struct CamExtrinsic
    {
        float trans_x; // 平移向量
        float trans_y; // 平移向量
        float trans_z; // 平移向量
        float roll; // 欧拉角
        float pitch; // 欧拉角
        float yaw; // 欧拉角
    };

} // zzq

#endif //BASIC_KITS_DATA_BASICS_H
