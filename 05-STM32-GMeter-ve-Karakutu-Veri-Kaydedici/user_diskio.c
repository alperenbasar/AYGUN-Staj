/* USER CODE BEGIN Header */
 /* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */
#include <string.h>
#include "ff_gen_drv.h"
#include "user_diskio.h"
#include "main.h"

// SPI4 Tanimi
extern SPI_HandleTypeDef hspi4;

#define SD_CS_LOW()  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET)
#define SD_CS_HIGH() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET)

/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
static uint8_t CardType = 0; // 1: SDSC (2GB Bayt Adresleme), 2: SDHC

static uint8_t SPI_RxByte(void) {
    uint8_t dummy = 0xFF, data = 0xFF;
    HAL_SPI_TransmitReceive(&hspi4, &dummy, &data, 1, 200);
    return data;
}

static void SPI_TxByte(uint8_t data) {
    uint8_t dummy_rx;
    HAL_SPI_TransmitReceive(&hspi4, &data, &dummy_rx, 1, 200);
}

static uint8_t SD_ReadyWait(void) {
    uint32_t timeout = HAL_GetTick() + 500;
    while (SPI_RxByte() != 0xFF) {
        if (HAL_GetTick() >= timeout) return 0xFF;
    }
    return 0x00;
}

static uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg) {
    uint8_t response, retry = 0;
    uint8_t crc = 0xFF;

    if (cmd == 0) crc = 0x95;
    else if (cmd == 8) crc = 0x87;

    SD_CS_LOW();

    if (cmd != 0 && cmd != 12) {
        if (SD_ReadyWait() != 0x00) {
            SD_CS_HIGH();
            SPI_TxByte(0xFF);
            return 0xFF;
        }
    }

    SPI_TxByte(cmd | 0x40);
    SPI_TxByte((uint8_t)(arg >> 24));
    SPI_TxByte((uint8_t)(arg >> 16));
    SPI_TxByte((uint8_t)(arg >> 8));
    SPI_TxByte((uint8_t)arg);
    SPI_TxByte(crc);

    do {
        response = SPI_RxByte();
        retry++;
    } while ((response & 0x80) && retry < 250);

    return response;
}
/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

DSTATUS USER_initialize (
	BYTE pdrv
)
{
  /* USER CODE BEGIN INIT */
    if (pdrv != 0) return STA_NOINIT;

    uint8_t ocr[4];

    SD_CS_HIGH();
    HAL_Delay(150);

    hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    HAL_SPI_Init(&hspi4);

    for (int i = 0; i < 30; i++) {
        SPI_TxByte(0xFF);
    }

    uint8_t res0 = 0xFF;
    for (int retry = 0; retry < 100; retry++) {
        res0 = SD_SendCommand(0, 0);
        SD_CS_HIGH();
        SPI_TxByte(0xFF);
        if (res0 == 0x01 || res0 == 0x00) break;
        HAL_Delay(5);
    }

    if (res0 != 0x01 && res0 != 0x00) {
        return STA_NOINIT;
    }

    if (SD_SendCommand(8, 0x1AA) == 0x01) {
        for (int i = 0; i < 4; i++) ocr[i] = SPI_RxByte();
        SD_CS_HIGH();
        SPI_TxByte(0xFF);

        uint32_t timeout = HAL_GetTick() + 2500;
        while (HAL_GetTick() < timeout) {
            SD_SendCommand(55, 0);
            SD_CS_HIGH();
            SPI_TxByte(0xFF);

            if (SD_SendCommand(41, 0x40000000) == 0x00) break;
            SD_CS_HIGH();
            SPI_TxByte(0xFF);
            HAL_Delay(10);
        }
        if (HAL_GetTick() >= timeout) return STA_NOINIT;

        if (SD_SendCommand(58, 0) == 0x00) {
            for (int i = 0; i < 4; i++) ocr[i] = SPI_RxByte();
            CardType = (ocr[0] & 0x40) ? 2 : 1;
        }
        SD_CS_HIGH();
        SPI_TxByte(0xFF);
    } else {
        SD_CS_HIGH();
        SPI_TxByte(0xFF);
        CardType = 1;

        uint32_t timeout = HAL_GetTick() + 2500;
        while (HAL_GetTick() < timeout) {
            SD_SendCommand(55, 0);
            SD_CS_HIGH();
            SPI_TxByte(0xFF);

            if (SD_SendCommand(41, 0) == 0x00) break;
            SD_CS_HIGH();
            SPI_TxByte(0xFF);
            HAL_Delay(10);
        }
        if (HAL_GetTick() >= timeout) return STA_NOINIT;
    }

    SD_SendCommand(16, 512);
    SD_CS_HIGH();
    SPI_TxByte(0xFF);

    hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
    HAL_SPI_Init(&hspi4);

    Stat &= ~STA_NOINIT;
    return Stat;
  /* USER CODE END INIT */
}

DSTATUS USER_status (
	BYTE pdrv
)
{
  /* USER CODE BEGIN STATUS */
    if (pdrv != 0) return STA_NOINIT;
    return Stat;
  /* USER CODE END STATUS */
}

DRESULT USER_read (
	BYTE pdrv,
	BYTE *buff,
	DWORD sector,
	UINT count
)
{
  /* USER CODE BEGIN READ */
    if (pdrv != 0 || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    uint32_t addr = (CardType == 1) ? (sector * 512) : sector;

    while (count--) {
        if (SD_SendCommand(17, addr) != 0x00) {
            SD_CS_HIGH();
            SPI_TxByte(0xFF);
            return RES_ERROR;
        }

        uint32_t timeout = HAL_GetTick() + 300;
        while (SPI_RxByte() != 0xFE) {
            if (HAL_GetTick() >= timeout) {
                SD_CS_HIGH();
                SPI_TxByte(0xFF);
                return RES_ERROR;
            }
        }

        for (int i = 0; i < 512; i++) *buff++ = SPI_RxByte();
        SPI_RxByte();
        SPI_RxByte();

        SD_CS_HIGH();
        SPI_TxByte(0xFF);

        addr += (CardType == 1) ? 512 : 1;
    }
    return RES_OK;
  /* USER CODE END READ */
}

#if _USE_WRITE == 1
DRESULT USER_write (
	BYTE pdrv,
	const BYTE *buff,
	DWORD sector,
	UINT count
)
{
  /* USER CODE BEGIN WRITE */
    if (pdrv != 0 || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    uint32_t addr = (CardType == 1) ? (sector * 512) : sector;

    while (count--) {
        if (SD_SendCommand(24, addr) != 0x00) {
            SD_CS_HIGH();
            SPI_TxByte(0xFF);
            return RES_ERROR;
        }

        SPI_TxByte(0xFF);
        SPI_TxByte(0xFE);

        for (int i = 0; i < 512; i++) SPI_TxByte(*buff++);

        SPI_TxByte(0xFF);
        SPI_TxByte(0xFF);

        uint8_t resp = SPI_RxByte();
        if ((resp & 0x1F) != 0x05) {
            SD_CS_HIGH();
            SPI_TxByte(0xFF);
            return RES_ERROR;
        }

        uint32_t timeout = HAL_GetTick() + 1500;
        while (SPI_RxByte() != 0xFF) {
            if (HAL_GetTick() >= timeout) {
                SD_CS_HIGH();
                SPI_TxByte(0xFF);
                return RES_ERROR;
            }
        }

        SD_CS_HIGH();
        SPI_TxByte(0xFF);

        addr += (CardType == 1) ? 512 : 1;
    }
    return RES_OK;
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

#if _USE_IOCTL == 1
DRESULT USER_ioctl (
	BYTE pdrv,
	BYTE cmd,
	void *buff
)
{
  /* USER CODE BEGIN IOCTL */
    if (pdrv != 0) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    DRESULT res = RES_ERROR;
    switch (cmd) {
        case CTRL_SYNC:
            SD_CS_LOW();
            if (SD_ReadyWait() == 0x00) res = RES_OK;
            SD_CS_HIGH();
            SPI_TxByte(0xFF);
            break;

        case GET_SECTOR_SIZE:
            *(WORD*)buff = 512;
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE:
            *(DWORD*)buff = 1;
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT:
            *(DWORD*)buff = 3900000;
            res = RES_OK;
            break;

        default:
            res = RES_OK;
            break;
    }
    return res;
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */
