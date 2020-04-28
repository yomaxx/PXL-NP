#BLACKJACK SERVER

#imports
import random

#wait for players to join

#Player cards
Player1_cards = []
Player2_cards = []

#server deals cards
while len(Player1_cards) != 2:
    Player1_cards.append(random.randint(1,11))
    if len(Player1_cards) == 2:
        print("Player 1 has: ", Player1_cards)
    
while len(Player2_cards) != 2:
    Player2_cards.append(random.randint(1,11))
    if len(Player2_cards) == 2:
        print("Player 2 has: ", Player2_cards)
        
#Player1 turn
print("Player1 turn")
while sum(Player1_cards) < 21:
    if sum(Player1_cards) == 21:
        printf("Player1 has won")
    Player1_action = str(input("do you want to stay or hit? "))
    if Player1_action == "hit":
        Player1_cards.append(random.randint(1,11))
        print("You now have "+ str(sum(Player1_cards)) +" from these cards ", Player1_cards)
    else:
        print("You ended your turn with: "+ str(sum(Player1_cards)) +" from these cards ", Player1_cards)
        break
if sum(Player1_cards) > 21:
    print("Player 1 busted")
#Player2 turn
print("Player2 turn")
while sum(Player2_cards) < 21:
    if sum(Player2_cards) == 21:
        printf("Player1 has won")
    Player_action = str(input("do you want to stay or hit? "))
    if Player_action == "hit":
        Player2_cards.append(random.randint(1,11))
        print("You now have "+ str(sum(Player2_cards)) +" from these cards ", Player2_cards)
    else:
        print("You ended your turn with: "+ str(sum(Player2_cards)) +" from these cards ", Player2_cards)
        break
if sum(Player2_cards) > 21:
    print("Player 2 busted")
    
#check winner