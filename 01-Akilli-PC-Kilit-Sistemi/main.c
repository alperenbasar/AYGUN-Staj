/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f429i_discovery_sdram.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ts.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
CRC_HandleTypeDef hcrc;
DMA2D_HandleTypeDef hdma2d;
I2C_HandleTypeDef hi2c3;
LTDC_HandleTypeDef hltdc;
SPI_HandleTypeDef hspi5;
TIM_HandleTypeDef htim1;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
SDRAM_HandleTypeDef hsdram1;

osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
TS_StateTypeDef TS_State;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_DMA2D_Init(void);
static void MX_FMC_Init(void);
static void MX_I2C3_Init(void);
static void MX_LTDC_Init(void);
static void MX_SPI5_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_UART5_Init(void);
void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */
void DrawMainScreen(void);
void DrawNumpadScreen(void);
void UpdatePinDisplay(char* pin_str);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_DMA2D_Init();
  MX_FMC_Init();
  MX_I2C3_Init();
  MX_LTDC_Init();
  MX_SPI5_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_UART5_Init();

  /* USER CODE BEGIN 2 */
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(0);
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  // 2. Dokunmatiği Başlat (Ekran çözünürlüğü: 240x320)
  BSP_TS_Init(240, 320);

  // "AC" Butonu Çiz
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillRect(0, 0, 240, 160);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"AC", CENTER_MODE);

  // "KILITLE" Butonu Çiz
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_FillRect(0, 160, 240, 160);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(0, 220, (uint8_t *)"KILITLE", CENTER_MODE);
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 4096);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_CRC_Init(void)
{
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_DMA2D_Init(void)
{
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
}

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

static void MX_LTDC_Init(void)
{
  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 9;
  hltdc.Init.VerticalSync = 1;
  hltdc.Init.AccumulatedHBP = 29;
  hltdc.Init.AccumulatedVBP = 3;
  hltdc.Init.AccumulatedActiveW = 269;
  hltdc.Init.AccumulatedActiveH = 323;
  hltdc.Init.TotalWidth = 279;
  hltdc.Init.TotalHeigh = 327;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 240;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 320;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0xD0000000;
  pLayerCfg.ImageWidth = 240;
  pLayerCfg.ImageHeight = 320;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

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

static void MX_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_UART5_Init(void)
{
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 9600;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
}

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

static void MX_FMC_Init(void)
{
  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  hsdram1.Instance = FMC_SDRAM_DEVICE;
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;

  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 3;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler();
  }
}

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

  HAL_GPIO_WritePin(GPIOC, NCS_MEMS_SPI_Pin|CSX_Pin|OTG_FS_PSO_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(ACP_RST_GPIO_Port, ACP_RST_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|WRX_DCX_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOG, LD3_Pin|LD4_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = NCS_MEMS_SPI_Pin|CSX_Pin|OTG_FS_PSO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = B1_Pin|MEMS_INT1_Pin|MEMS_INT2_Pin|TP_INT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = ACP_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ACP_RST_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = OTG_FS_OC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  HAL_GPIO_Init(OTG_FS_OC_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = TE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(TE_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = WRX_DCX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(WRX_DCX_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LD3_Pin|LD4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
void DrawMainScreen(void) {
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    osDelay(20);

    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_FillRect(0, 0, 240, 158);

    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_FillRect(0, 162, 240, 158);

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawHLine(0, 159, 240);
    BSP_LCD_DrawHLine(0, 160, 240);
    BSP_LCD_DrawHLine(0, 161, 240);

    BSP_LCD_SetFont(&Font24);

    BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 65, (uint8_t *)"AC", CENTER_MODE);

    BSP_LCD_SetBackColor(LCD_COLOR_RED);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(0, 225, (uint8_t *)"KILITLE", CENTER_MODE);
}

// Numpad Çizim
void DrawNumpadScreen(void) {
    BSP_LCD_Clear(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DrawHLine(0, 80, 240);
    BSP_LCD_DrawHLine(0, 140, 240);
    BSP_LCD_DrawHLine(0, 200, 240);
    BSP_LCD_DrawHLine(0, 260, 240);
    BSP_LCD_DrawVLine(80, 80, 240);
    BSP_LCD_DrawVLine(160, 80, 240);

    BSP_LCD_SetFont(&Font24);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);

    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(32, 100, (uint8_t *)"1", LEFT_MODE);
    BSP_LCD_DisplayStringAt(112, 100, (uint8_t *)"2", LEFT_MODE);
    BSP_LCD_DisplayStringAt(192, 100, (uint8_t *)"3", LEFT_MODE);

    BSP_LCD_DisplayStringAt(32, 160, (uint8_t *)"4", LEFT_MODE);
    BSP_LCD_DisplayStringAt(112, 160, (uint8_t *)"5", LEFT_MODE);
    BSP_LCD_DisplayStringAt(192, 160, (uint8_t *)"6", LEFT_MODE);

    BSP_LCD_DisplayStringAt(32, 220, (uint8_t *)"7", LEFT_MODE);
    BSP_LCD_DisplayStringAt(112, 220, (uint8_t *)"8", LEFT_MODE);
    BSP_LCD_DisplayStringAt(192, 220, (uint8_t *)"9", LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(32, 280, (uint8_t *)"C", LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DisplayStringAt(112, 280, (uint8_t *)"0", LEFT_MODE);

    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(192, 280, (uint8_t *)"OK", LEFT_MODE);
}

// Şifre Yıldızları
void UpdatePinDisplay(char* pin_str) {
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(0, 0, 240, 79);

    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font24);

    if(strlen(pin_str) == 0) {
        BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"SIFRE:", CENTER_MODE);
    } else {
        char stars[10] = "";
        for(int i=0; i<strlen(pin_str); i++) {
            stars[i] = '*';
        }
        stars[strlen(pin_str)] = '\0';
        BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)stars, CENTER_MODE);
    }
}
/* USER CODE END 4 */

void StartDefaultTask(void const * argument)
{
  MX_USB_HOST_Init();

  /* USER CODE BEGIN 5 */
  // 1. SDRAM'İ BAŞLAT VE KOMUTLARI GÖNDER
  BSP_SDRAM_Init();
  osDelay(50); // SDRAM'in oturması için kısa bir bekleme

  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LCD_BACKGROUND_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_SelectLayer(LCD_BACKGROUND_LAYER);
  BSP_LCD_DisplayOn();

  BSP_LCD_Clear(LCD_COLOR_BLACK);
  osDelay(20);

  BSP_TS_Init(240, 320);

  // BUTONLARI ÇİZ
  // Üst Buton (Yeşil)
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_FillRect(0, 0, 240, 158);

  // Alt Buton (Kırmızı)
  BSP_LCD_SetTextColor(LCD_COLOR_RED);
  BSP_LCD_FillRect(0, 162, 240, 158);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_DrawHLine(0, 159, 240);
  BSP_LCD_DrawHLine(0, 160, 240);
  BSP_LCD_DrawHLine(0, 161, 240);

  BSP_LCD_SetFont(&Font24);

  // "AC" Yazısı
  BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, 65, (uint8_t *)"AC", CENTER_MODE);

  // "KILITLE" Yazısı
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(0, 225, (uint8_t *)"KILITLE", CENTER_MODE);

  // SİSTEM DEĞİŞKENLERİ
  int current_state = 0; // 0 = Ana Ekran (Kilitle/Aç), 1 = Numpad
  char entered_pin[10] = "";
  int pin_index = 0;

  char my_password[] = "2";

  // 6. FREERTOS GÖREV DÖNGÜSÜ
  for(;;)
  {
      BSP_TS_GetState(&TS_State);

      if(TS_State.TouchDetected)
      {
          uint16_t x = TS_State.X;
          uint16_t y = TS_State.Y;

          if (current_state == 0)
          {
              if(y > 160)
              {
                  HAL_UART_Transmit(&huart1, (uint8_t*)"LOCK\n", 5, 100);
                  HAL_UART_Transmit(&huart5, (uint8_t*)"PC Kilitlendi\r\n", 15, 100);

                  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_RESET);
                  HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_SET);

                  osDelay(500);
              }
              else if(y <= 160)
              {
                  current_state = 1;
                  pin_index = 0;
                  memset(entered_pin, 0, sizeof(entered_pin));

                  DrawNumpadScreen();
                  UpdatePinDisplay(entered_pin);
                  osDelay(400);
              }
          }
          else if (current_state == 1)
          {
              if (y > 70) // Sadece rakamların veya butonların olduğu alana basıldıysa
              {
                  char key = 0;

                  if (y > 220 && x > 140) {
                      key = 'E';
                  }
                  else if (y > 220 && x < 85) {
                      key = 'C';
                  }
                  else if (y > 220) {
                      key = '0';
                  }
                  else if (y > 175) {
                      if (x < 85) key = '7';
                      else if (x > 145) key = '9';
                      else key = '8';
                  }
                  else if (y > 125) {
                      if (x < 85) key = '4';
                      else if (x > 145) key = '6';
                      else key = '5';
                  }
                  else if (y > 70) {
                      if (x < 85) key = '1';
                      else if (x > 145) key = '3';
                      else key = '2';
                  }

                  if (key == 'C') // İptal Tuşu (Ana Ekrana Dön)
                  {
                      current_state = 0;
                      DrawMainScreen();
                  }
                  else if (key == 'E') // OK Tuşu (Şifreyi Onayla)
                  {
                      if (strcmp(entered_pin, my_password) == 0) // Şifre DOĞRUYSA
                      {
                          BSP_LCD_Clear(LCD_COLOR_GREEN);
                          BSP_LCD_SetBackColor(LCD_COLOR_GREEN);
                          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                          BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"KILIT ACILDI", CENTER_MODE);

                          // Şifre doğruysa Python'a aç komutu gönderilir
                          HAL_UART_Transmit(&huart1, (uint8_t*)"UNLOCK\n", 7, 100);
                          HAL_UART_Transmit(&huart5, (uint8_t*)"PC Acildi\r\n", 11, 100);

                          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
                          HAL_GPIO_WritePin(GPIOG, LD4_Pin, GPIO_PIN_RESET);
                          HAL_GPIO_WritePin(GPIOG, LD3_Pin, GPIO_PIN_SET);

                          osDelay(1500);

                          current_state = 0;
                          DrawMainScreen();
                      }
                      else // Şifre YANLIŞSA
                      {
                          BSP_LCD_Clear(LCD_COLOR_RED);
                          BSP_LCD_SetBackColor(LCD_COLOR_RED);
                          BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
                          BSP_LCD_DisplayStringAt(0, 150, (uint8_t *)"HATALI SIFRE", CENTER_MODE);

                          osDelay(1200);

                          pin_index = 0;
                          memset(entered_pin, 0, sizeof(entered_pin));
                          DrawNumpadScreen();
                          UpdatePinDisplay(entered_pin);
                      }
                  }
                  else if (key != 0) // Rakamlara basıldıysa
                  {
                      if (pin_index < 8)
                      {
                          entered_pin[pin_index] = key;
                          pin_index++;
                          UpdatePinDisplay(entered_pin);
                      }
                  }
                  osDelay(300); // Tıklama zıplamasını (bounce) engellemek için süre
              }
          }
      }
      osDelay(30);
  }
  /* USER CODE END 5 */
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
