#include <SPI.h>
#include "at86rf2xx.h"

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

void enviaLeituras() {   // Função para  o nó que mede e transmite
  const TAMANHO_LEITURAS = 10;
  uint8_t data[TAMANHO_LEITURAS];

  at86rf2xx.set_state(AT86RF2XX_STATE_PLL_ON);

  at86rf2xx.send(data, TAMANHO_LEITURAS);

  at86rf2xx.set_state(AT86RF2XX_STATE_SLEEP);
}

void at86rf2xx_Lida_evento() {

  /* One less event to handle! */
  at86rf2xx.events--;

  /* If transceiver is sleeping register access is impossible and frames are
   * lost anyway, so return immediately.
   */
  byte state = at86rf2xx.get_status();
  if(state == AT86RF2XX_STATE_SLEEP)
    return;

  /* read (consume) device status */
  byte irq_mask = at86rf2xx.reg_read(AT86RF2XX_REG__IRQ_STATUS);)

  /*  Incoming radio frame! */
  if (irq_mask & AT86RF2XX_IRQ_STATUS_MASK__RX_START)
    Serial.println("[at86rf2xx] EVT - RX_START");

  /*  Done receiving radio frame; call our receive_data function.
   */
  if (irq_mask & AT86RF2XX_IRQ_STATUS_MASK__TRX_END)
  {
    if(state == AT86RF2XX_STATE_RX_AACK_ON || state == AT86RF2XX_STATE_BUSY_RX_AACK) {
      Serial.println("[at86rf2xx] EVT - RX_END");
      at86rf2xx_receive_data();
    }
  }
}

void at86rf2xx_receive_data() {
  /*  print the length of the frame
   *  (including the header)
   */
  size_t pkt_len = at86rf2xx.rx_len();
  Serial.print("Tamanho do pacote de dados: ");
  Serial.print(pkt_len);
  Serial.println(" bytes");

  /*  Print the frame, byte for byte  */
  Serial.println("Frame dump (ASCII):");

  uint8_t data[pkt_len];
  int umidade = 0;
  at86rf2xx.rx_read(data, pkt_len, 0); //lendo o buffer e preenchendo data

  for (int d=0; d<pkt_len; d++) { // lendo data com a mensagem
    Serial.print((char)data[d]);
    umidade += data[d];
  }

  umidade = umidade/pkt_len;
  Serial.println();

  /* How many frames is this so far?  */
  Serial.print("[[Quantas mensagens foram recebidas: ");
  Serial.print(++received);
  Serial.println("]]\n");
}
