/*
 * This is meant to be called after netif initialization (MX_LWIP_Init in lwip.c) when powered up.
 */
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"
#include "task.h"
#include "eth_init_task.h"

extern struct netif gnetif;

void eth_init_task_start(void) {
	const osThreadAttr_t ethInitTask_attributes = {
			.name = "ethInitTask",
			.priority = (osPriority_t) osPriorityLow,
			.stack_size = 512 };
	osThreadNew(EthInitTask, NULL, &ethInitTask_attributes);

}

void EthInitTask(void *argument) {
	HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_SET);
	for (;;) {
		uint32_t phyBSR = 0;
		HAL_ETH_ReadPHYRegister(&heth, PHY_BSR, &phyBSR);
		if ((phyBSR & PHY_LINKED_STATUS) == PHY_LINKED_STATUS
				&& !netif_is_link_up(&gnetif)) {
			// First Ethernet cable reconnect after powered on, bring up netif and start dhcp
			gnetif.flags |= NETIF_FLAG_LINK_UP;
			netif_set_up(&gnetif);
			dhcp_start(&gnetif);
			HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_RESET);
			// No use anymore
			vTaskDelete(NULL);
		}
		osDelay(1);
	}
}
