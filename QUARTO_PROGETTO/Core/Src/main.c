/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

// Funzioni specifiche richieste dall'esercizio nel PDF
void LCD_STROBE(void);
void LCD_DATA(unsigned char data);
void LCD_CMD(unsigned char cmd);
void LCD_CLEAR(void);
void LCD_PUTS(const char * s);
void LCD_INIT(void);
void LCD_GOTO(unsigned char pos);

int main(void)
{
  /* Reset di tutte le periferiche, inizializza l'interfaccia Flash e il Systick. */
  HAL_Init();

  /* Configura il clock di sistema */
  SystemClock_Config();

  /* Inizializza tutti i GPIO configurati */
  MX_GPIO_Init();

  /* --- INIZIO ESERCIZIO --- */

  LCD_INIT();   // Inizializza il display
  LCD_CLEAR();  // Pulisce lo schermo

  LCD_GOTO(0x00);         // Vai alla prima riga
  LCD_PUTS("Andrei Bodnar"); // Scrivi il tuo nome

  LCD_GOTO(0x40);         // Vai alla seconda riga (indirizzo 0x40 per HD44780)
  LCD_PUTS("STM32 C031");

  /* --- FINE ESERCIZIO --- */

  while (1)
  {
    // Il loop resta vuoto o puoi far lampeggiare un LED qui
  }
}

/* --- IMPLEMENTAZIONE FUNZIONI LCD (Dal Datasheet) --- */

void LCD_STROBE(void) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);   // EN High
    HAL_Delay(1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); // EN Low
    HAL_Delay(1);
}

void LCD_DATA(unsigned char data) {
    // Gestione dei 4 bit di dati sui tuoi pin specifici:
    // DB4 -> PB10, DB5 -> PB4, DB6 -> PB5, DB7 -> PA15
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (data >> 0) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  (data >> 1) & 0x01);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  (data >> 2) & 0x01);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, (data >> 3) & 0x01);
    LCD_STROBE();
}

void LCD_CMD(unsigned char cmd) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET); // RS Low per i comandi
    LCD_DATA(cmd >> 4);   // Invia i 4 bit alti
    LCD_DATA(cmd & 0x0F); // Invia i 4 bit bassi
}

void LCD_CLEAR(void) {
    LCD_CMD(0x01);
    HAL_Delay(2);
}

void LCD_PUTS(const char * s) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); // RS High per i dati
    while(*s) {
        LCD_DATA(*s >> 4);
        LCD_DATA(*s & 0x0F);
        s++;
    }
}

void LCD_GOTO(unsigned char pos) {
    LCD_CMD(0x80 | pos);
}

void LCD_INIT(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); // RW a terra (PB6)
    HAL_Delay(50); // Attesa accensione

    // Sequenza di inizializzazione 4-bit
    LCD_DATA(0x03); HAL_Delay(5);
    LCD_DATA(0x03); HAL_Delay(1);
    LCD_DATA(0x03);
    LCD_DATA(0x02); // Attiva modalità 4-bit

    LCD_CMD(0x28); // 2 Righe, 5x8 dots
    LCD_CMD(0x0C); // Display ON, Cursor OFF
    LCD_CMD(0x06); // Entry mode
    LCD_CLEAR();
}

/* --- CONFIGURAZIONE HARDWARE (Generata da CubeIDE) --- */

void SystemClock_Config(void)
{
  // Configurazione standard generata dal tuo progetto
  // Lascia quella che hai già nel tuo file originale se diversa
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // Configurazione PA10 (EN) e PA15 (DB7)
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Configurazione PB4, PB5, PB6 (RW), PB7 (RS), PB10 (DB4)
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
