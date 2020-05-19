# PXL-NP

blackjack server geschreven in python met client in c.

## alegemene uitleg

 De server verwacht dat 2 spelers connecten voordat de game begint. Als de game begonnen is deelt hij 2 kaarten aan beide spelers uit.
 De kaarten worden naar beide spelers gestuurd, ze krijgen dus hun eigen kaarten en die van de andere speler te zien.
speler 1 mag nu kiezen of hij nog een kaart neemt of er mee stopt, vervolgens doet speler 2 hetzelfde.
Tern slotte als beide spelers gestopt zijn wordt de winnaar bekend gemaakt en kunnen beide spelers kiezen of ze blijven of stoppen.

## technische uitleg

De game werkt met 2 soorten commando's, namelijk join en game. deze worden als volgt verstuurd:
  - BlackJack>join?>bericht> & BlackJack>join!>bericht>
  - BlackJack>game?>bericht> & BlackJack>game!>bericht>
  - BlackJack>winner!>bericht>
  
Berichten naar de server worden altijd met een ? verstuurd en de server antwoord met !, zo kan je makkelijk zien wie wat verstuurd.
Als het om algemene berichten gaat komt het bericht achter de game of join, maar als het bericht voor een speler is komt zijn naam eerst.
Dit kan dus tot de volgende commando's lijden:
  - BlackJack>game?>naam>bericht> & BlackJack>game!>naam>bericht>
Op deze manier kan ik er voor zorgen dat spelers niet bij elkaar keuzes kunnen maken.
om een duidelijker beelt te geven hoe het spel werkt met commando's staat hieronder een verloop van commando's.

## verloop game
```
- SERVER : luistert op blackjack>join?>
- CLIENT : verstuurd naam op blackjack>join?>naam>
- SERVER : verstuurd op blackjack>join!> dat alle spelers verbonden zijn
- SERVER : verstuurd op blackjack>game!> de kaarten van alle spelers (alle spelers krijgen dit binnen)
- LOOP   : volgende 2 lopen zolang speler niet blackjack heeft of busted is of speler geen kaart meer wilt.
- SERVER : verstuurd op blackjack>game!>naam1> wat speler1 wilt doen
- CLIENT1: verstuurd op blackjack>game?>naam1> wat hij wilt doen

```
