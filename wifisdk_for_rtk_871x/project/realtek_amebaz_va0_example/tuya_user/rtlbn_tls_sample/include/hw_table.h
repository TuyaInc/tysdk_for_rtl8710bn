/***********************************************************
File: hw_table.h 
Author:徐靖航 JingHang Hu
Date: 2017-11-11
Description:
	硬件结构抽象表，表达通用硬件配置
	描述内容：
	    1，wifi指示[数量(0~1)]
	        驱动方式：高 低驱动
	    2开关 插座控制通道(ctrl_channel)[1~n]
	        继电器(relay)：高 低驱动
	        按钮：高 低驱动或不驱动
	        LED指示灯：高 低驱动或不驱动
***********************************************************/
// ================================= 类型声明定义 =================================
// IO驱动类型： 高 低 IO不存在
#ifndef HW_TABLE_H
#define HW_TABLE_H

//include <com_def.h>
//include <led_indicator.h>
//include <key.h>
//include "wf_sdk_adpt.h"
#include "tuya_gpio.h"
#include "tuya_key.h"
#include "tuya_led.h"
#include "tuya_iot_wifi_api.h"

#define DPID_NOT_EXIST (-1)

typedef enum 
{
	IO_DRIVE_LEVEL_HIGH,		//高电平有效
	IO_DRIVE_LEVEL_LOW,			//低电平有效
	IO_DRIVE_LEVEL_NOT_EXIST	//该IO不存在
}IO_DRIVE_TYPE;

// IO配置抽象
// IO有效电平
// IO引脚号
typedef struct
{
	IO_DRIVE_TYPE type;	// 有效电平类型
	TY_GPIO_PORT_E pin;	// 引脚号
}IO_CONFIG;


// 开关 插座控制通道
// 目前包含
//	    继电器
//		控制按键
//		״ָ̬状态指示灯
typedef struct
{
	IO_CONFIG   relay;			// 继电器
	KEY_USER_DEF_S	    button;			// 控制按键
	IO_CONFIG	led;			// 状态指示灯
	LED_HANDLE	    led_handle;		// 状态指示灯句柄
	INT_T			    dpid;			// 该通道绑定的dpid
	INT_T			    cd_dpid;		// 该通道绑定的倒计时dpid 小于0表示不存在
	INT_T 		    cd_sec;			// 通道倒计时 -1 停止
	//BOOL            is_count_down;
	INT_T             respond_cd_count;
	BOOL_T		    stat;			//通道状态 TRUE - 有效; FALSE - 无效
}CTRL_CHANNEL;

// HW_TABLE结构体类型
// 抽象描述硬件配置
typedef struct
{
	IO_CONFIG	wifi_stat_led;		// wifi状态指示灯
	LED_HANDLE	wifi_stat_led_handle;

	KEY_USER_DEF_S	rst_button;		// 重置按键
	INT_T 		channel_num;
	CTRL_CHANNEL *channels;			// ͨ导通列表 *!* 不要重新指向其它位置
}HW_TABLE;

// ================================ ��������������� ================================

// ================================== ������������ ==================================
// 初始化hw_table
// 注册所需硬件
// 返回注册成功与否
OPERATE_RET init_hw(HW_TABLE *hw_table);
// 控制通道
// hw_table:硬件对象
// channel_no:被控制的通道号
// is_active:通道是否有效 - 有效对应TRUE - 对应该插座 开关通电
void hw_set_channel(HW_TABLE *hw_table, INT_T channel_no, BOOL_T is_active);
// 用dpid控制通道
// hw_table:硬件对象
// dpid:	dpid
// is_active:道是否有效 - 有效对应TRUE - 对应该插座 开关通电
// return: (int) 有动作 通道小标(大于等于0);无动作 -1; 未找到 -2
int hw_set_channel_by_dpid(HW_TABLE *hw_table, INT_T dpid, BOOL_T is_active);
// 切换通道状态 有效 <->无效
// hw_table:    硬件对象
// channel_no:	被控制的通道号
void hw_trig_channel(HW_TABLE *hw_table, INT_T channel_no);
// 切换wifi指示灯状态
// hw_table:硬件对象
// wifi_stat:WiFi状态
void hw_set_wifi_led_stat(HW_TABLE *hw_table, GW_WIFI_NW_STAT_E wifi_stat);
// 倒计时dpid控制通道
// hw_table: 硬件对象
// dpid: 倒计时dpid
// return: (int)有动作 通道小标(大于等于0);无动作 -1
INT_T hw_find_channel_by_cd_dpid(HW_TABLE *hw_table, INT_T dpid);

void sync_led_and_relay_connected(HW_TABLE *hw_table);
void sync_led_and_relay_unconnected(HW_TABLE *hw_table);


// ================================ 方法声明定义结束==============================

// ================================ 全局变量声明定义 ================================
extern HW_TABLE g_hw_table;

#endif
