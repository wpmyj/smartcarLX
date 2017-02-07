/*
 *     COPYRIGHT NOTICE
 *     Copyright 刘忠威
 *     All rights reserved.
 * @file      PC_link.c
 * @author    Liuzw
 * @version   V1.0
 * @date      2016.02.17
 * @brief     上位机通信协议
 */
#include  "PC_link.h"

/*!
 *  @brief      上位机解包协议
 *  @param      *Buf    缓冲地址
 *  @author     Liuzw
 */
void analysisdata(unsigned char *Buf)
{
    if(Buf[0] == 0xaa && Buf[1] == 0xaa) //帧头符合
    {
        
    }
}
