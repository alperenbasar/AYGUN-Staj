/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_sdram.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MPU6050_ADDR (0x68 << 1)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DMA2D_HandleTypeDef hdma2d;
I2C_HandleTypeDef hi2c3;
LTDC_HandleTypeDef hltdc;
SPI_HandleTypeDef hspi5;
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t mpu_data[6];
int16_t raw_ax, raw_ay, raw_az;
float ax, ay, az;
float prev_ax = 0.0f, prev_ay = 0.0f, prev_az = 1.0f;

uint8_t alarm_state = 0;       // 0: Guvenli, 1: ALARM
uint32_t last_led_toggle = 0;

TS_StateTypeDef TS_State;
char entered_pass[5] = "";
const char correct_pass[] = "2";   // Şifreniz
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI5_Init(void);
/* USER CODE BEGIN PFP */
void MPU6050_Init(void);
void I2C3_Recover(void);
void Draw_Idle_Screen(void);
void Draw_Numpad_Screen(void);
char Get_Touched_Key(uint16_t tx, uint16_t ty);
uint8_t Test_MicroSD_Catalex(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SD_CS_LOW()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
#define SD_CS_HIGH()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)

// SPI üzerinden 1 byte gönderip 1 byte okuma
uint8_t SPI_Transfer(uint8_t data)
{
    uint8_t rx_data = 0xFF;
    HAL_SPI_TransmitReceive(&hspi5, &data, &rx_data, 1, 100);
    return rx_data;
}

// MicroSD kartı SPI moduna geçirme testi (CMD0)
uint8_t Test_MicroSD_Catalex(void)
{
    uint8_t response = 0xFF;

    // Kartı uyandırmak için CS High durumundayken en az 80 dummy clock
    SD_CS_HIGH();
    for (int i = 0; i < 15; i++)
    {
        SPI_Transfer(0xFF);
    }

    // CMD0 Gönder: {0x40, 0x00, 0x00, 0x00, 0x00, 0x95}
    SD_CS_LOW();
    SPI_Transfer(0x40);
    SPI_Transfer(0x00);
    SPI_Transfer(0x00);
    SPI_Transfer(0x00);
    SPI_Transfer(0x00);
    SPI_Transfer(0x95); // CRC

    // Karttan R1 yanıtı bekle (Geçerli cevap: 0x01)
    for (int i = 0; i < 200; i++)
    {
        response = SPI_Transfer(0xFF);
        if (response != 0xFF)
        {
            break;
        }
    }

    SD_CS_HIGH();
    SPI_Transfer(0xFF);

    return response;
}
/* USER CODE END 0 */

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize configured peripherals */
  MX_GPIO_Init();
  MX_I2C3_Init();
  MX_USART1_UART_Init();
  MX_SPI5_Init();

  /* USER CODE BEGIN 2 */
  // LED pinlerini hızlıca aktif et (PG13 Yeşil, PG14 Kırmızı)
    __HAL_RCC_GPIOG_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_Led = {0};
    GPIO_Led.Pin = GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_Led.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Led.Pull = GPIO_NOPULL;
    GPIO_Led.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_Led);

    // Başlangıçta ikisini de söndür
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET);

    HAL_Delay(300); // Voltaj otursun

    // MicroSD Testini Çalıştır
    uint8_t status = Test_MicroSD_Catalex();

    if (status == 0x01)
    {
        // --- MODÜL VE KART SAĞLAM ---
        // Yeşil LED sürekli yanacak!
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);
    }
    else
    {
        // --- HATA VAR / MODÜL VEYA KART CEVAP VERMEDİ ---
        // Kırmızı LED yanıp sönecek (Flaşör yapacak)
        while (1)
        {
            HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
            HAL_Delay(200);
        }
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      // 1. MPU6050 İvme Okuma
      HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c3, MPU6050_ADDR, 0x3B, 1, mpu_data, 6, 20);

      if (status != HAL_OK)
      {
          I2C3_Recover();
          HAL_Delay(20);
          continue;
      }

      raw_ax = (int16_t)(mpu_data[0] << 8 | mpu_data[1]);
      raw_ay = (int16_t)(mpu_data[2] << 8 | mpu_data[3]);
      raw_az = (int16_t)(mpu_data[4] << 8 | mpu_data[5]);

      ax = raw_ax / 16384.0f;
      ay = raw_ay / 16384.0f;
      az = raw_az / 16384.0f;

      float diff_x = fabsf(ax - prev_ax);
      float diff_y = fabsf(ay - prev_ay);
      float diff_z = fabsf(az - prev_az);
      float total_delta = diff_x + diff_y + diff_z;

      prev_ax = (prev_ax * 0.8f) + (ax * 0.2f);
      prev_ay = (prev_ay * 0.8f) + (ay * 0.2f);
      prev_az = (prev_az * 0.8f) + (az * 0.2f);

      // 2. Alarm Tetikleme
      if (!alarm_state && total_delta > 0.55f)
      {
          alarm_state = 1;
          entered_pass[0] = '\0';
          Draw_Numpad_Screen();
          HAL_UART_Transmit(&huart1, (uint8_t*)"ALARM\r\n", 7, 100);
      }

      // 3. Alarm Esnasında Numpad / Dokunmatik Yönetimi
      if (alarm_state)
      {
          // Harici LED Flaşör
          if (HAL_GetTick() - last_led_toggle > 120)
          {
              HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_11);
              last_led_toggle = HAL_GetTick();
          }

          // Dokunmatik Panel Kontrolü
          BSP_TS_GetState(&TS_State);
                        if (TS_State.TouchDetected)
                        {
                            char key = Get_Touched_Key(TS_State.X, TS_State.Y);

                            if (key == 'C') // Sil
                            {
                                entered_pass[0] = '\0';
                                Draw_Numpad_Screen();
                                HAL_Delay(200);
                            }
                            else if (key == 'E') // Enter
                            {
                                if (strcmp(entered_pass, correct_pass) == 0)
                                {
                                    alarm_state = 0;
                                    entered_pass[0] = '\0';
                                    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
                                    HAL_UART_Transmit(&huart1, (uint8_t*)"DISARMED\r\n", 10, 100);
                                    Draw_Idle_Screen();
                                }
                                else
                                {
                                    // Şifre yanlışsa sıfırla
                                    entered_pass[0] = '\0';
                                    Draw_Numpad_Screen();
                                }
                                HAL_Delay(250);
                            }
                            else if (key >= '0' && key <= '9') // Sadece rakamlar
                            {
                                if (strlen(entered_pass) < 4)
                                {
                                    int len = strlen(entered_pass);
                                    entered_pass[len] = key;
                                    entered_pass[len + 1] = '\0';
                                    Draw_Numpad_Screen();
                                }
                                HAL_Delay(200);
                            }
                        }
      }
      else
      {
          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
      }

      HAL_Delay(20);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C3 Initialization Function
  */
static void MX_I2C3_Init(void)
{
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI5 Initialization Function
  */
static void MX_SPI5_Init(void)
{
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
void MPU6050_Init(void)
{
    uint8_t wake = 0x00;
    HAL_I2C_Mem_Write(&hi2c3, MPU6050_ADDR, 0x6B, 1, &wake, 1, 100);
}

void I2C3_Recover(void)
{
    __HAL_RCC_I2C3_FORCE_RESET();
    HAL_Delay(2);
    __HAL_RCC_I2C3_RELEASE_RESET();
    HAL_I2C_Init(&hi2c3);
    MPU6050_Init();
}

// 1. BEKLEME EKRANI (Güvenli Durum)
void Draw_Idle_Screen(void)
{
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 100, (uint8_t *)"SISTEM GUVENLI", CENTER_MODE);

    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(0, 140, (uint8_t *)"Hareket Sensoru", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, 160, (uint8_t *)"Aktif Olarak Izliyor", CENTER_MODE);
}

// 2. ALARM EKRANI VE NUMPAD ÇİZİMİ
void Draw_Numpad_Screen(void)
{
    BSP_LCD_Clear(LCD_COLOR_RED);
    BSP_LCD_SetBackColor(LCD_COLOR_RED);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(0, 15, (uint8_t *)"! ALARM AKTIF !", CENTER_MODE);

    // Şifre Giriş Kutusu
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(20, 45, 200, 35);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DrawRect(20, 45, 200, 35);

    // Girilen karakterler
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_SetFont(&Font20);
    if (strlen(entered_pass) == 0)
    {
        BSP_LCD_DisplayStringAt(0, 53, (uint8_t *)"SIFRE GIR", CENTER_MODE);
    }
    else
    {
        char mask_buf[10] = "";
        for (int i = 0; i < strlen(entered_pass); i++) mask_buf[i] = '*';
        mask_buf[strlen(entered_pass)] = '\0';
        BSP_LCD_DisplayStringAt(0, 53, (uint8_t *)mask_buf, CENTER_MODE);
    }

    // 3x4 Tuş Takımı
    char keys[4][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'C', '0', 'E'}
    };

    BSP_LCD_SetFont(&Font20);

    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            uint16_t x = 15 + col * 72;
            uint16_t y = 90 + row * 52;

            uint32_t btn_color;
            if (keys[row][col] == 'C')
            {
                btn_color = LCD_COLOR_DARKRED;
            }
            else if (keys[row][col] == 'E')
            {
                btn_color = LCD_COLOR_DARKGREEN;
            }
            else
            {
                btn_color = LCD_COLOR_BLUE;
            }

            // Buton zemin dolgusu
            BSP_LCD_SetTextColor(btn_color);
            BSP_LCD_FillRect(x, y, 66, 46);

            // Buton dış çerçevesi
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
            BSP_LCD_DrawRect(x, y, 66, 46);

            // Buton üzerindeki beyaz karakter
            BSP_LCD_SetBackColor(btn_color);
            BSP_LCD_SetTextColor(LCD_COLOR_WHITE);

            char key_str[2] = {keys[row][col], '\0'};
            BSP_LCD_DisplayStringAt(x + 25, y + 14, (uint8_t *)key_str, LEFT_MODE);
        }
    }
}

// 3. DOKUNULAN TUŞUN ANALİZİ
char Get_Touched_Key(uint16_t raw_x, uint16_t raw_y)
{
    // STMPE811 ham koordinatlarını LCD dikey (240x320) koordinatlarına çevir:
    // DISC1 kartında donanımsal X/Y dönüşümü:
    uint16_t tx = raw_x;
    uint16_t ty = raw_y;

    // Tuş yerleşim haritası (Ekrandaki çizimle birebir aynı)
    char keys[4][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'C', '0', 'E'}
    };

    // Ekrandaki buton pikselleri:
    // x = 15, 87, 159 (genişlik 66 piksel)
    // y = 90, 142, 194, 246 (yükseklik 46 piksel)

    // Kolon belirleme (X Ekseni: 0..240)
    int col = -1;
    if (tx >= 10 && tx < 83)        col = 0;  // 1. Sütun (1, 4, 7, C)
    else if (tx >= 83 && tx < 155)  col = 1;  // 2. Sütun (2, 5, 8, 0)
    else if (tx >= 155 && tx <= 235) col = 2; // 3. Sütun (3, 6, 9, E)

    // Satır belirleme (Y Ekseni: 0..320)
    int row = -1;
    if (ty >= 85 && ty < 138)       row = 0;  // 1. Satır (1, 2, 3)
    else if (ty >= 138 && ty < 190) row = 1;  // 2. Satır (4, 5, 6)
    else if (ty >= 190 && ty < 242) row = 2;  // 3. Satır (7, 8, 9)
    else if (ty >= 242 && ty <= 315) row = 3; // 4. Satır (C, 0, E) -> E BURADA!

    // EĞER X ve Y TERS İSE (Kart revizyonuna göre swap durumu):
    if (col == -1 || row == -1)
    {
        // Swap ve Invert dönüşümü
        uint16_t sw_x = raw_y;
        uint16_t sw_y = (raw_x <= 320) ? (320 - raw_x) : raw_x;

        if (sw_x >= 10 && sw_x < 83)        col = 0;
        else if (sw_x >= 83 && sw_x < 155)  col = 1;
        else if (sw_x >= 155 && sw_x <= 235) col = 2;

        if (sw_y >= 85 && sw_y < 138)       row = 0;
        else if (sw_y >= 138 && sw_y < 190) row = 1;
        else if (sw_y >= 190 && sw_y < 242) row = 2;
        else if (sw_y >= 242 && sw_y <= 315) row = 3;
    }

    if (row >= 0 && row < 4 && col >= 0 && col < 3)
    {
        return keys[row][col];
    }

    return 0;
}
/* USER CODE END 4 */

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
