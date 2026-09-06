# TODO — Golf 1984 (recriação em C + raylib)

Referência de mecânica: [Golf (NES, 1984)](https://pt.wikipedia.org/wiki/Golf_(jogo_eletr%C3%B4nico)).
O objetivo é reproduzir a jogabilidade original (visão de cima, swing meter de 3 toques,
seleção de tacos, 18 buracos, obstáculos), não apenas "um jogo de golfe genérico estilo retrô".

Convenção: cada fase deixa o jogo compilando e jogável (`make run`) antes de avançar para a próxima.

---

## Fase 0 — Corrigir a base atual (bloqueante)

O estado atual (`src/main.c`) tem um bug que impede jogar: o ângulo do tiro usa
`course.windAngle` em vez de um ângulo controlado pelo jogador. Antes de adicionar
qualquer feature nova, arrumar isso.

- [ ] Mover as `struct Player`, `Ball`, `Course` e os protótipos de função para `src/game.h`
      (hoje está vazio; `main.c` concentra tudo, o que não bate com o README).
- [ ] Mover `InitGame`, `UpdateGame`, `DrawGame` para `src/game.c`; `main.c` deve conter
      apenas o loop (`InitWindow`/`while`/`CloseWindow`).
- [ ] Adicionar `float aimAngle` em `Player`, controlado por `KEY_LEFT`/`KEY_RIGHT`
      (o README já promete isso, o código não implementa).
- [ ] Corrigir o disparo em `UpdateGame` para usar `player.aimAngle`, não `course.windAngle`.
- [ ] Separar claramente "força do vento como constante somada à velocidade" (atual) de
      "força do vento como deslocamento acumulado" — hoje o vento é aplicado como aceleração
      contínua em todo frame de voo, o que é razoável, mas precisa de comentário/decisão
      explícita, não um valor mágico herdado do protótipo.
- [ ] Testar manualmente: mirar, carregar, soltar, ver a bola seguir o ângulo escolhido
      (não o vento).

## Fase 1 — Mecânica de tacada fiel ao original (swing meter)

O NES Golf não usa "segurar para carregar". Usa um medidor de 3 toques:
1º toque inicia a barra subindo: 2º toque trava a **potência** (quanto mais alto, mais longe);
depois a barra desce por uma faixa de precisão e o 3º toque define **hook/slice**
(acertar o centro = tiro reto; errar = curva lateral proporcional ao erro).

- [ ] Implementar `SwingMeter` como máquina de estados: `IDLE → RISING → POWER_LOCKED → ACCURACY_WINDOW → RESOLVED`.
- [ ] Trocar `KEY_SPACE` hold por 3 toques discretos (`IsKeyPressed`, não `IsKeyDown`).
- [ ] Calcular potência a partir do ponto de trava (0.0–1.0), não de tempo de charge.
- [ ] Calcular desvio lateral (hook/slice) a partir do erro no 3º toque; aplicar como
      componente perpendicular à direção da mira.
- [ ] Desenhar a barra do swing meter na HUD (retângulo vertical ou horizontal com marcador).
- [ ] Ajustar `MAX_SHOT_POWER` por taco (ver Fase 2) em vez de constante global única.

## Fase 2 — Tacos (clubs)

- [ ] Definir tabela de tacos com alcance/trajetória distintos: Driver (1W), 3W, 5W,
      ferros 3i–9i, Sand Wedge, Putter.
- [ ] `KEY_UP`/`KEY_DOWN` (ou `TAB`) para trocar de taco antes da tacada.
- [ ] Cada taco define: distância máxima, altura de trajetória (arco), e se pode ser
      usado dentro de bunker (só wedge/sand) ou no green (só putter).
- [ ] Regra: dentro do green, forçar troca automática para Putter (como no original).
- [ ] HUD já mostra `CLUB %dW` — expandir formatação para irons/putter (`3i`, `PT`, etc.).

## Fase 3 — Campo e terrenos

Hoje existe só fundo `DARKGREEN` uniforme. O original distingue terrenos que afetam
a física e a pontuação.

- [ ] Modelar o campo como dados (não hardcoded): posição do tee, fairway, green, hole,
      bunkers, água, rough, out-of-bounds — via polígonos simples ou grade de tiles.
- [ ] Cada terreno define atrito de rolagem diferente (fairway > rough > bunker em atrito).
- [ ] Água e OB: bola que cai neles não continua o movimento — aplica penalidade de
      1 tacada e reposiciona no último ponto válido (regra de golfe real).
- [ ] Bunker: reduzir distância máxima do taco usado ali (dificulta o próximo tiro).
- [ ] Renderizar os terrenos com cores/formas distintas (mesmo que simples, sem sprites).
- [ ] Câmera: no NES original a visão alterna entre "vista ampla do buraco" (mira) e
      "seguir a bola" durante o voo, aproximando perto do green. Decidir se replicamos
      isso ou fixamos câmera única — registrar a decisão aqui antes de implementar.

## Fase 4 — Loop de jogo (18 buracos, scorecard)

- [ ] Estrutura `Hole { int par; Vector2 teePos; Vector2 holePos; /* geometria dos terrenos */ }`.
- [ ] Array de 18 buracos (dados fixos, não proceduralmente gerados — como no original).
- [ ] Contagem de tacadas por buraco; ao cair no buraco, mostrar resultado
      (Eagle/Birdie/Par/Bogey/Double Bogey) comparando com o par.
- [ ] Transição entre buracos (tela de resumo → próximo tee).
- [ ] Scorecard final ao completar os 18 buracos (soma total vs. par total).
- [ ] Persistência simples do progresso (arquivo local), já que senha estilo NES não
      faz sentido fora do hardware original — decisão a confirmar com o usuário se quiser
      fidelidade histórica em vez de conveniência moderna.

## Fase 5 — HUD e menus estilo NES

- [ ] Tela de título (Stroke Play / Match Play — ou só Stroke Play se Match Play for
      escopo futuro).
- [ ] HUD final: `STROKE PLAY`, taco atual, tacadas no buraco, par, vento (força + direção
      com seta), distância até o buraco.
- [ ] Indicador de vento como seta/flâmula (hoje é só texto numérico).
- [ ] Paleta de cores e fonte pixelada para reforçar identidade visual retrô (raylib
      permite carregar fonte bitmap custom).

## Fase 6 — Áudio (opcional, retro bleep-bloop)

- [ ] SFX de tacada, bola caindo no buraco, splash na água.
- [ ] Música de fundo em loop, estilo chiptune (opcional).

## Fase 7 — Extensão 2D/3D (o "3D" mencionado no pedido)

O jogo original é 2D top-down. Se a intenção é ir além da fidelidade histórica:

- [ ] Avaliar câmera 3D em perspectiva (raylib tem `Camera3D` pronta) como modo alternativo,
      mantendo a física de projétil já existente adaptada para `Vector3`.
- [ ] Isso é um **item de escopo a confirmar com o usuário** antes de investir tempo:
      historicamente fiel (2D puro) vs. remake com câmera 3D. Não assumir qual é o objetivo.

## Fase 8 — Qualidade

- [ ] Adicionar testes manuais documentados (não há framework de teste em C aqui — decidir
      se vale introduzir algo como Unity/CMocka para física, ou manter validação manual).
- [ ] `make clean`/`make run` continuam funcionando a cada fase.
- [ ] Remover código morto e comentários de blocos desativados (`#define BALL_MASS`, etc.)
      conforme forem decididos (usar ou remover, não deixar comentado indefinidamente).

---

## Notas de decisão em aberto

- Fidelidade histórica (senha, swing meter puro, 2D) vs. remake modernizado (save file,
  câmera 3D, gráficos maiores) — a Fase 7 e o item de persistência da Fase 4 dependem
  dessa escolha.
- Fonte dos dados dos 18 buracos: hardcoded em `.c`/`.h` é suficiente para o escopo atual;
  formato de arquivo externo (JSON/CSV) só se houver necessidade de editar buracos sem
  recompilar.
