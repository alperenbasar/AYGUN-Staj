/*
 * CN3303 sarj durum gostergesi - 3 renkli kumulatif dolum animasyonu
 * MCU      : PIC12F1822 (8 pin), dahili osilator 4 MHz
 * Derleyici: MPLAB XC8 v3.x  (MPLAB X IDE 6.x)
 *
 * Pin baglantilari (SOIC-8 / PDIP-8 numaralari):
 *   1  VDD
 *   2  RA5  -> R 470R -> D1 KIRMIZI -> GND
 *   3  RA4  -> R 470R -> D2 SARI    -> GND
 *   4  RA3  -> ICSP pin1 (MCLR/VPP)  (MCLRE=OFF, LVP=OFF, dahili pull-up acik)
 *   5  RA2  -> R 470R -> D3 YESIL   -> GND
 *   6  RA1  -> ICSP pin5 (PGC), baska bir sey bagli degil
 *   7  RA0/AN0 <- R 4.7k <- CN3303 STAT (+ 100k pull-down), ayni zamanda ICSP pin4 (PGD)
 *   8  VSS
 *
 * Davranis:
 *   STAT ~0 V (sarj bitti)          -> uc LED de surekli yanik
 *   STAT 0 V'un uzerinde herhangi bir seviye -> K, K+S, K+S+Y, hepsi sonuk, tekrar (adim 400 ms)
 *   USB (VIN) yok -> PIC'in beslemesi yok -> hicbir sey yanmaz
 *   Durum degisikligi ancak yeni seviye 1 sn boyunca kesintisiz gorulurse uygulanir.
 */

#include <xc.h>
#include <stdint.h>

/* ---------------- Konfigurasyon bitleri ---------------- */
/* CONFIG1 */
#pragma config FOSC    = INTOSC   /* Dahili osilator, CLKIN pini I/O */
#pragma config WDTE    = ON       /* Watchdog acik (kilitlenirse reset atar) */
#pragma config PWRTE   = ON       /* Power-up timer acik */
#pragma config MCLRE   = OFF      /* RA3 dijital giris (MCLR dahili VDD'ye bagli) */
#pragma config CP      = OFF
#pragma config CPD     = OFF
#pragma config BOREN   = ON       /* Brown-out reset acik */
#pragma config CLKOUTEN = OFF
#pragma config IESO    = OFF
#pragma config FCMEN   = OFF
/* CONFIG2 */
#pragma config WRT     = OFF
#pragma config PLLEN   = OFF
#pragma config STVREN  = ON
#pragma config BORV    = LO
#pragma config LVP     = OFF      /* Programlama yuksek voltajla (PICkit 3 VPP) */

#define _XTAL_FREQ 4000000UL

/* ---------------- Kullanici ayarlari ---------------- */
/* STAT ADC ile okunur (10 bit, referans VDD ~4.8-5 V, RA0 onunde 4.7k/100k bolucu).
 * Dolu durumda STAT ~0.014 V (=~3 sayim). Sarjda STAT 0.3 V ile 3.6 V arasinda herhangi
 * bir seviyede olabilir (kirmizi tek, veya kirmizi+mavi birlikte yanarken).
 *   sayim <= ADC_FULL_MAX    -> DOLU
 *   sayim >= ADC_CHARGE_MIN  -> SARJ OLUYOR
 *   arada                    -> onceki karar korunur (histerezis)
 */
#define ADC_FULL_MAX         25     /* ~0.12 V */
#define ADC_CHARGE_MIN       60     /* ~0.28 V */

/* 1 = acilista 300 ms lamba testi (breadboard'da kablolari dogrulamak icin).
 * Nihai urunde 0 yapabilirsin. */
#define LAMP_TEST            1

#define TICK_MS              10     /* ana dongu adimi */
#define FILTER_TICKS         100    /* 100 x 10 ms = 1 sn filtre */
#define STEP_TICKS           40     /* 40 x 10 ms = 400 ms animasyon adimi */
#define LAMP_TEST_TICKS      30     /* 300 ms */

/* ---------------- Pin tanimlari ---------------- */
#define LED_R    LATAbits.LATA5     /* D1 kirmizi */
#define LED_Y    LATAbits.LATA4     /* D2 sari    */
#define LED_G    LATAbits.LATA2     /* D3 yesil   */

enum { ST_UNKNOWN = 0, ST_CHARGING = 1, ST_FULL = 2 };

/* bit0 = kirmizi, bit1 = sari, bit2 = yesil */
static const uint8_t anim[4] = { 0x01, 0x03, 0x07, 0x00 };

static void leds(uint8_t p)
{
    LED_R = (p & 0x01) ? 1 : 0;
    LED_Y = (p & 0x02) ? 1 : 0;
    LED_G = (p & 0x04) ? 1 : 0;
}

static uint16_t read_stat(void)
{
    uint16_t v;
    __delay_us(100);                    /* ornekleme suresi (kaynak empedansi yuksek) */
    ADCON0 |= 0x02;                     /* GO */
    while (ADCON0 & 0x02) {             /* DONE olana kadar bekle */
        ;
    }
    v = (uint16_t)(((uint16_t)ADRESH << 8) | ADRESL);
    return v;
}

static void tick(void)
{
    CLRWDT();
    __delay_ms(TICK_MS);
}

void main(void)
{
    uint8_t state = ST_UNKNOWN;
    uint8_t cand  = ST_UNKNOWN;
    uint8_t cnt   = 0;
    uint8_t step  = 0;
    uint8_t sub   = 0;
    uint8_t raw = ST_CHARGING;          /* belirsizse "sarj oluyor" kabul et */
    uint16_t adc;
    uint8_t i;

    /* Osilator: IRCF=1101 -> 4 MHz, SCS=00 (config bitindeki kaynak) */
    OSCCON = 0x68;

    /* Pinler */
    ANSELA = 0x01;              /* RA0 analog (STAT), digerleri dijital */
    LATA   = 0x00;              /* tum cikislar 0 */
    TRISA  = 0x09;              /* RA0 ve RA3 giris, RA1/RA2/RA4/RA5 cikis */
    OPTION_REGbits.nWPUEN = 0;  /* weak pull-up'lar genel olarak etkin */
    WPUA   = 0x08;              /* sadece RA3 (kullanilmayan MCLR/giris) pull-up'li */

    /* ADC: sag hizali, Fosc/8 (2 us TAD @4MHz), referans VDD, kanal AN0, ADC acik */
    ADCON1 = 0xA0;
    ADCON0 = 0x01;

#if LAMP_TEST
    leds(0x07);
    for (i = 0; i < LAMP_TEST_TICKS; i++) {
        tick();
    }
    leds(0x00);
#endif

    for (;;) {
        /* 1) STAT'i oku, filtrele */
        adc = read_stat();
        if (adc >= ADC_CHARGE_MIN) {
            raw = ST_CHARGING;
        } else if (adc <= ADC_FULL_MAX) {
            raw = ST_FULL;
        }                               /* arada: raw degismez */

        if (raw == state) {
            cand = state;
            cnt  = 0;
        } else {
            if (raw != cand) {
                cand = raw;
                cnt  = 0;
            }
            if (++cnt >= FILTER_TICKS) {
                state = raw;
                cnt   = 0;
                step  = 0;
                sub   = 0;
            }
        }

        /* 2) LED'leri surdur */
        switch (state) {
        case ST_CHARGING:
            leds(anim[step]);
            if (++sub >= STEP_TICKS) {
                sub  = 0;
                step = (uint8_t)((step + 1) & 0x03);
            }
            break;

        case ST_FULL:
            leds(0x07);
            break;

        default:
            leds(0x00);
            break;
        }

        tick();
    }
}