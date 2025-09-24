games: 250
red: 248
yellow: 2

Weights:
-   Weights r = { .line_three_attack = 100 , .line_two_attack = 50 , .line_three_defense = 250 , .line_two_defense = 15 };
-   Weights y = { .line_three_attack = 100 , .line_two_attack = 50 , .line_three_defense = 250 , .line_two_defense = 15 };

games: 1000
red: 984
yellow: 16
Weights:
-  Weights r = { .line_three_attack = 200 , .line_two_attack = 50 , .line_three_defense = 250 , .line_two_defense = 15 };
-  Weights y = { .line_three_attack = 100 , .line_two_attack = 50 , .line_three_defense = 350 , .line_two_defense = 15 };

Heurística Mejorada:

Weights r = { 
.three_attack_open = 5000 ,
.three_attack_semi_open = 500 ,
.two_attack_open = 1000 ,
.two_attack_semi_open = 100 ,
.three_defense_open = 6000,
.three_defense_semi_open = 6000,
.two_defense_open = 1000,
.two_defense_semi_open = 500
};
Weights y = { 
    .three_attack_open = 5000 ,
    .three_attack_semi_open = 500 ,
    .two_attack_open = 1000 ,
    .two_attack_semi_open = 100 ,
    .three_defense_open = 6000,
    .three_defense_semi_open = 6000,
    .two_defense_open = 1000,
    .two_defense_semi_open = 500
};
games : 1000
yellow: 389
red: 611

games: 1000 -> profundidaddes de:
yellow: 370  (9)
red: 630     (7)


Heurística + control del centro: (solo yellow)
games: 1000
red:407
yellow: 593

Heurística + control del centro: (ambos)
games: 1000
red: 711
yellow: 289


Heurística + control del centro + refuerzo (solo yellow):
games: 1000
red: 272
yellow: 728

Heurística + control del centro + refuerzo (ambos):
games: 1000
red: 807
yellow: 193