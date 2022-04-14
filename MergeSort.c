#include <string.h>
#include <malloc.h>
#include "Status.h"
/**
 * @brief 原地归并
 * 
 * @param L 数组对象
 * @param lo 开始的位置(包含)
 * @param mid 中间位置
 * @param hi 数组结束的位置(包含)
 * @return 状态,OK/ERROR
 **/
Status merge(int *list,int lo,int mid,int hi){
    int *leftP=list+lo;// 左数组: lo to mid
    int *rigthP=list+mid+1; //右数组 mid+1 to hi
    int tmp[hi-lo+1];
    int k;
    for(int k=lo;k<=hi)

}