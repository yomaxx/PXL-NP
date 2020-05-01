######################
###BLACKJACK SERVER###
######################
#imports
import random
import zmq

#setup filter and topic
JoinFilter = "BlackJack>join?>".encode('ascii')
JoinTopic = "BlackJack>join!>"
GameFilter = "BlackJack>game?>".encode('ascii')
GameTopic = "BlackJack>game!>"

#setup network
context = zmq.Context()
publisher = context.socket(zmq.PUSH)
subscriber = context.socket(zmq.SUB)

publisher.connect("tcp://benternet.pxl-ea-ict.be:24041")
subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042")
subscriber.setsockopt(zmq.SUBSCRIBE, JoinFilter)

#store Playernames
PlayerNames = []

#wait for players to join
for x in range(2):
    print("waiting for players, ("+ str(x)+"/2)")
    message = subscriber.recv()
    PlayerNames.append(message.decode('utf8').split('>')[2])
    
print("Players joined, game can begin")
print("Player1: "+ str(PlayerNames[0])+" and Player2: "+ str(PlayerNames[1]))
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
while sum(Player1_cards) <= 21:
    if sum(Player1_cards) == 21:
        print("!!!Player1 has blackjack!!!")
        break
    Player1_action = str(input("do you want to stay or hit? "))
    if Player1_action == "hit":
        Player1_cards.append(random.randint(1,11))
        print("You now have "+ str(sum(Player1_cards)) +" from these cards ", Player1_cards)
    else:
        print("You ended your turn with: "+ str(sum(Player1_cards)) +" from these cards ", Player1_cards)
        break
    
if sum(Player1_cards) < 21:
    #Player2 turn
    print("Player2 turn")
    while sum(Player2_cards) <= 21:
        if sum(Player2_cards) == 21:
            print("!!!Player2 has blackjack!!!")
            break
        Player_action = str(input("do you want to stay or hit? "))
        if Player_action == "hit":
            Player2_cards.append(random.randint(1,11))
            print("You now have "+ str(sum(Player2_cards)) +" from these cards ", Player2_cards)
        else:
            print("You ended your turn with: "+ str(sum(Player2_cards)) +" from these cards ", Player2_cards)
            break
        if sum(Player2_cards) > 21:
            print("Player 2 busted, Player1 wins by default")
            break;
elif sum(Player1_cards) > 21:
    print("Player 1 busted, player 2 wins by default")
    
#check winner
if sum(Player1_cards) == sum(Player2_cards):
    print("no winner")
elif sum(Player1_cards) < 21 and sum(Player1_cards) > sum(Player2_cards):
    print("Player 1 won with: "+ str(sum(Player1_cards)) +" vs "+ str(sum(Player2_cards)) )
elif sum(Player2_cards) < 21 and sum(Player1_cards) < sum(Player2_cards):
    print("Player 2 won with: "+ str(sum(Player2_cards)) +" vs "+ str(sum(Player1_cards)) )