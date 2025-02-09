# Unidade 4 - Capítulo 6: Projeto de Controle Serial com o RP2040

---

## Aluno: 
- **Carlos Henrique Silva Lopes**

## **Link do vídeo de Ensaio**

## Objetivos

- Compreender o funcionamento e a aplicação de comunicação serial em microcontroladores.
- Aplicar os conhecimentos adquiridos sobre UART e I2C na prática.
- Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812.
- Fixar o estudo do uso de botões de acionamento, interrupções e debounce.
- Desenvolver um projeto funcional que combine hardware e software.

## Descrição do Projeto

Este projeto, feito para o RP2040, possui os seguintes componentes:

- **Matriz 5x5 de LEDs WS2812**, conectada à GPIO 7.
- **LED RGB**, com os pinos conectados às GPIOs (11, 12 e 13).
- **Botão A**, conectado à GPIO 5.
- **Botão B**, conectado à GPIO 6.
- **Display SSD1306**, conectado via I2C (GPIO 14 e GPIO 15).

## Funcionalidades do Projeto

### 1. Modificação da Biblioteca `font.h`
- Adição de caracteres minúsculos.

### 2. Entrada de Caracteres via PC
- Utiliza o **Serial Monitor** do VS Code para digitação.
- Cada letra digitada será exibido no **display SSD1306**.
- Se um número entre 0 e 9 for digitado, um símbolo correspondente será exibido também na **matriz 5x5 WS2812**.

### 3. Interação com o **Botão A**
- Alterna o estado do **LED RGB Verde** (ligado/desligado).
- A operação é registrada de duas formas:
  - Uma mensagem informativa no **display SSD1306**.
  - Um texto descritivo enviado ao **Serial Monitor**.

### 4. Interação com o **Botão B**
- Alterna o estado do **LED RGB Azul** (ligado/desligado).
- A operação é registrada de duas formas:
  - Uma mensagem informativa no **display SSD1306**.
  - Um texto descritivo enviado ao **Serial Monitor**.

## Configuração do Projeto

1. Clone este repositório para sua máquina:

   ```
   git clone https://github.com/CarlosHenriqueSL/Projeto-08-EmbarcaTech.git
   cd Projeto-08-EmbarcaTech
   ```

2. Certifique-se de que o **Pico SDK** está configurado no VS Code.

3. Abra o projeto no **VS Code** e configure os arquivos `CMakeLists.txt` adequados para compilar o código.

4. Execute o projeto no simulador **Wokwi** ou faça o upload para o hardware físico do Raspberry Pi Pico W.

### Principais Arquivos
- **`ProjetoOneShot.c:`** Contém a lógica principal para fazer a leitura do monitor serial, conexão com o display ssd1306, desenho na matriz de LEDs, leitura dos botões
- **`inc/:`** Contém os arquivos com caracteres em hexadecimal, e também arquivos para escrever no ssd1306.
- **`numeros/:`** Contém os arquivos com os números que serão exibidos na matriz de LEDs.
- **`diagram.json:`** Configuração física do projeto no simulador Wokwi.
- **`README.md:`** Documentação detalhada do projeto.


## Testes e Validação

1. Ao digitar uma letra, maiúscula ou minúscula, ela deve aparecer no display ssd1306 e apenas nele.
  
2. Ao digitar um número entre 0 e 9, além de aparecer no display, ele também deve aparecer na matriz de LEDs.

3. Ao acionar o botão A, o LED verde deve alternar de estado, e uma mensagem deve ser exibida no monitor serial e no display.

4. Ao acionar o botão B, o LED azul deve alternar de estado, e uma mensagem deve ser exibida no monitor serial e no display. 

