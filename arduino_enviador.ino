#include <SPI.h>
#include "at86rf2xx.h"
const TAMANHO_LEITURAS = 10;

int received = 0;

int IRQ = 2;
int RESET = 8;
int SLP_TR = 7;
int SEL = 6;

void setup() {
  Serial.begin(115200);
  at86rf2xx.init(SEL, IRQ, SLP_TR, RESET);
  at86rf2xx.set_chan(26); // set channel to 26
}

void loop() {
  if (at86rf2xx.events)
    at86rf2xx_Lida_evento();
  return;
}

void loop() {

    //uint8_t leituras = leSensor();

    enviaLeituras(leituras);

    delay(5000);

  return;
}

void enviaLeituras(uint8_t leituras) {   // Função para  o nó que mede e transmite
  uint8_t data[TAMANHO_LEITURAS];

  at86rf2xx.set_state(AT86RF2XX_STATE_PLL_ON);

  at86rf2xx.send(data, TAMANHO_LEITURAS);

  at86rf2xx.set_state(AT86RF2XX_STATE_SLEEP);
}
