# PXL-NP

blackjack server geschreven in python met client in c.

---algemene uitleg---

 De server verwacht dat 2 spelers connecten voordat de game begint. Als de game begonnen is deelt hij 2 kaarten aan beide spelers uit.
 De kaarten worden naar beide spelers gestuurd, ze krijgen dus hun eigen kaarten en die van de andere speler te zien.
speler 1 mag nu kiezen of hij nog een kaart neemt of er mee stopt, vervolgens doet speler 2 hetzelfde.
Tern slotte als beide spelers gestopt zijn wordt de winnaar bekend gemaakt en kunnen beide spelers kiezen of ze blijven of stoppen.

---technische uitleg---

De game werkt met 2 soorten commando's, namelijk join en game. deze worden als volgt verstuurd:
  - BlackJack>join?>bericht> & BlackJack>join!>bericht>
  - BlackJack>game?>bericht> & BlackJack>game!>bericht>
  - BlackJack>winner!>bericht>
  
Berichten naar de server worden altijd met een ? verstuurd en de servere antwoord op !, zo kan je makkelijk zien wie wat verstuurd.
Als het om algemene berichten gaat komt het bericht achter de game of join, maar als het bericht voor een speler is komt zijn naam eerst.
Dit kan dus tot de volgende commando's lijden:
  - BlackJack>game?>naam>bericht> & BlackJack>game!>naam>bericht>
Op deze manier kan ik er voor zorgen dat spelers niet bij elkaar keuzes kunnen maken.
om een duidelijker beelt te geven hoe het spel werkt met commando's heb ik onderstaand flow diagram gemaakt.
--insert diagram--
