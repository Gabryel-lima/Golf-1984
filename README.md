# Golf 1984

Recriação do clássico **Golf** do NES (1984) em **C** utilizando a biblioteca [raylib](https://www.raylib.com/). O objetivo deste projeto é estudar física de trajetória de bola, interface gráfica retrô e mecânicas de golfe (força, ângulo, vento, tacadas e par).

---

## 🎮 Funcionalidades

- **Física da bola**: cálculo de trajetória considerando ângulo, força e vento.  
- **HUD retrô**: exibe informações de `STROKE PLAY`, `CLUB`, vento e distância.  
- **Seleção de taco**: diferentes “clubs” com alcances e características distintas.  
- **Contagem de tacadas**: registro de tacadas por buraco e pontuação comparada ao par.  
- **Campo simplificado**: fairway, bunker e green em visão aérea.

---

## 📋 Pré-requisitos

- **Linguagem C** (compatível com C99 ou superior)  
- **raylib** (versão ≥ 3.0)  
- **gcc** (ou outro compilador compatível)  
- **make** (para usar o Makefile)

> **Termos técnicos**  
> - *raylib*: biblioteca C para desenvolvimento de jogos simples e gráficos 2D/3D.  
> - *C99*: padrão ISO para a linguagem C, com suporte a declarações mistas e novos tipos.  

---

## 🛠️ Instalação e compilação

1. **Clone o repositório**  
   ```bash
   git clone https://github.com/SEU_USUARIO/golf1984.git
   cd golf1984

2. **Instale a raylib**

   * **Ubuntu / Debian**:

     ```bash
     sudo apt update
     sudo apt install libraylib-dev
     ```
   * **macOS** (com Homebrew):

     ```bash
     brew install raylib
     ```
   * **Windows**: seguir instruções oficiais em [https://www.raylib.com/cheatsheet/cheatsheet.html](https://www.raylib.com/cheatsheet/cheatsheet.html)

3. **Compile**

   ```bash
   make
   ```

4. **Execute**

   ```bash
   ./golf1984
   ```

---

## 📂 Estrutura do projeto

```
Golf1984/
├── Makefile           # regras de build
├── .gitignore
├── README.md          # este arquivo
└── src/
    ├── main.c         # entrypoint: janela, loop principal
    ├── game.h         # definições de estruturas e protótipos
    └── game.c         # lógica de atualização e desenho do jogo
```

---

## ⌨️ Controles

* **Seta esquerda / direita**: ajustar ângulo de tiro
* **Seta para cima / baixo**: aumentar / diminuir força
* **Espaço (Space)**: confirmar tacada
* **Esc**: sair do jogo

> **Parâmetros de física**
>
> * *Ângulo*: definido em graus, 0° = horizontal para a direita.
> * *Força*: valor escalar que multiplica a velocidade inicial.
> * *Vento*: desloca a bola em X conforme intensidade e direção.

---

## 🚀 Como contribuir

1. Faça um *fork* deste repositório.
2. Crie uma branch para a sua funcionalidade (`git checkout -b feat/nova-fisica`).
3. Realize as alterações e escreva testes, se aplicável.
4. Abra um *pull request*, descrevendo seu aporte.

---

## 📝 Licença

Este projeto está licenciado sob a [MIT License](LICENSE).

---

```

Sinta-se à vontade para ajustar seções, adicionar imagens de exemplo ou detalhes de implementação. Qualquer dúvida, só avisar!
```
