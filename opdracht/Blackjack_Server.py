######################
###BLACKJACK SERVER###
######################
#imports
import time
import random
import zmq

from random import choice as rc

#setup filter and topic
JoinFilter = "BlackJack>join?>".encode('ascii')
JoinTopic = "BlackJack>join!>"
GameFilter = "BlackJack>game?>".encode('ascii')
GameTopic = "BlackJack>game!>"
WinnerFilter = "BlackJack>winner?>".encode('ascii')
WinnerTopic = "BlackJack>winner!>"

#setup network
context = zmq.Context()
publisher = context.socket(zmq.PUSH)
subscriber = context.socket(zmq.SUB)

publisher.connect("tcp://benternet.pxl-ea-ict.be:24041")
subscriber.connect("tcp://benternet.pxl-ea-ict.be:24042")

#store possible cards values
cards = [2,3,4,5,6,7,8,9,10,10,10,11]

while(1):
    
    #Subscribe to join filter
    subscriber.setsockopt(zmq.SUBSCRIBE, JoinFilter)
    
    #store Playernames
    PlayerNames = []
    
    #waiting for players to join
    for x in range(2):
        print("waiting for players, ("+ str(x)+"/2)")
        message = subscriber.recv()
        PlayerNames.append(message.decode('utf8').split('>')[2])
    
    #send starting message
    time.sleep(1)
    message = JoinTopic + "Players joined" + ">"
    publisher.send_string(message)
    print(message)
    print("Player1: "+ str(PlayerNames[0])+" and Player2: "+ str(PlayerNames[1]))
    
    #make and set win amount filter
    Wins1Filter = "BlackJack>join?>aantalWins>".encode('ascii')
    subscriber.setsockopt(zmq.SUBSCRIBE, Wins1Filter)
    subscriber.setsockopt(zmq.UNSUBSCRIBE, JoinFilter)
    
    #Ask players for amount of wins
    time.sleep(1)
    message = JoinTopic + "To how many wins do you want to play?: " + ">"
    publisher.send_string(message)
    for y in range(2):
        message = subscriber.recv()
        Wins = message.decode('utf8').split('>')[3]
        if (y == 0):
            AmountWins = Wins
        elif(Wins < AmountWins):
            AmountWins = Wins
            
    message = JoinTopic + "The game will play until one has " + AmountWins + " wins." + ">"
    publisher.send_string(message)
    
    #unsubscribe to amountwins filter
   
    #set filter to game filter
    subscriber.setsockopt(zmq.SUBSCRIBE, GameFilter)
    
    #make playerwins counters
    Player1Wins = 0
    Player2Wins = 0
    
    while(Player1Wins < int(AmountWins) and Player2Wins <int(AmountWins)):
    
    
        print("player1 wins: " +str(Player1Wins)+" and player2 wins: "+str(Player2Wins))
        #Player cards
        print("making cards")
        Player1_cards = []
        Player2_cards = []

        #server deals cards
        while len(Player1_cards) != 2:
            Player1_cards.append(rc(cards))
            if len(Player1_cards) == 2:
                print("Player 1 has: ", Player1_cards)
        while len(Player2_cards) != 2:
            Player2_cards.append(rc(cards))
            if len(Player2_cards) == 2:
                print("Player 2 has: ", Player2_cards)
            
        #Check if player didnt get 11,11
        if(Player1_cards[0] == 11) and (Player1_cards[1] == 11):
            Player1_cards[1] = 1;
        if(Player2_cards[0] == 11) and (Player2_cards[1] == 11):
            Player2_cards[1] = 1
        
        #send cards to both players
        time.sleep(1)
        message = GameTopic + PlayerNames[0] + "'s cards: "+str(Player1_cards) + " and " +PlayerNames[1]+ "'s cards: "+str(Player2_cards) + '>'
        print(message)
        publisher.send_string(message)

        #Player1 turn
        print("Player1 turn")
        while sum(Player1_cards) <= 21:
            if sum(Player1_cards) == 21:
                print("!!!Player1 has blackjack!!!")
                message = GameTopic + PlayerNames[0] + '>' + "!!!BLACKJACK!!!"+ '>'
                break
            time.sleep(1)   
            message = GameTopic + PlayerNames[0] + '>' + "You now have "+str(Player1_cards)+" stay or hit?" + '>'
            publisher.send_string(message)
            message = subscriber.recv()
            Player1_action = message.decode('utf8').split('>')[3]
        
            print(Player1_action)
            if (Player1_action == "hit") or (Player1_action == "Hit"):
                Player1_cards.append(rc(cards))
                print("You now have "+ str(sum(Player1_cards)) +" from these cards ", Player1_cards)
            elif (Player1_action == "stay") or (Player1_action == "Stay"):
                print("You ended your turn with: "+ str(sum(Player1_cards)) +" from these cards ", Player1_cards)
                message = GameTopic + PlayerNames[0] + '>' + "You ended with: "+str(Player1_cards)+ '>'
                break
        if sum(Player1_cards) > 21:
            message = GameTopic + PlayerNames[0] + '>' + "!!!BUSTED!!!"+ '>'
        publisher.send_string(message)
    
        #Player2 turn
        print("Player2 turn")
        while sum(Player2_cards) <= 21:
            if sum(Player2_cards) == 21:
                print("!!!Player2 has blackjack!!!")
                message = GameTopic + PlayerNames[1] + '>' + "!!!BLACKJACK!!!"+ '>'
                break
            time.sleep(1)   
            message = GameTopic + PlayerNames[1] + '>' + "You now have "+ str(Player2_cards)+ " "+ PlayerNames[0]+ " Has: "+ str(Player1_cards)+" stay or hit?" + '>'
            publisher.send_string(message)
            message = subscriber.recv()
            Player2_action = message.decode('utf8').split('>')[3]
        
            if Player2_action == "hit":
                Player2_cards.append(rc(cards))
                print("You now have "+ str(sum(Player2_cards)) +" from these cards ", Player2_cards)
            elif Player2_action == "stay":
                print("You ended your turn with: "+ str(sum(Player2_cards)) +" from these cards ", Player2_cards)
                message = GameTopic + PlayerNames[1] + '>' + "You ended with: "+str(Player2_cards)+ '>'
                break
        if sum(Player2_cards) > 21:
            message = GameTopic + PlayerNames[1] + '>' + "!!!BUSTED!!!"+ '>'
        publisher.send_string(message)
    
         #check winner
        time.sleep(2)
        if sum(Player1_cards) == sum(Player2_cards):    #both players got same
            print("no winner")
            message = WinnerTopic + "Both players got: "+ str(sum(Player1_cards)) +" so it's a tie..."+ '>'
        
        elif sum(Player1_cards) < 21 and sum(Player1_cards) > sum(Player2_cards):     #player 1 won
            print("Player 1 won with: "+ str(sum(Player1_cards)) +" vs "+ str(sum(Player2_cards)) )
            message = WinnerTopic + PlayerNames[0]+" Won with: "+str(sum(Player1_cards))+'>'
            Player1Wins += 1
        
        elif sum(Player2_cards) < 21 and sum(Player1_cards) < sum(Player2_cards):     #player 2 won
            print("Player 2 won with: "+ str(sum(Player2_cards)) +" vs "+ str(sum(Player1_cards)) )
            message = WinnerTopic + PlayerNames[1]+" Won with: "+str(sum(Player2_cards))+'>'
            Player2Wins += 1
        
        elif sum(Player1_cards) == 21:
            print("Player1 has BLACKJACK!!!")
            message = WinnerTopic + PlayerNames[0]+" Won with a !!!BLACKJACK!!!"+'>'
            Player1Wins += 1
        
        elif sum(Player2_cards) == 21:
            print("Player2 has BLACKJACK!!!")
            message = WinnerTopic + PlayerNames[1]+" Won with a !!!BLACKJACK!!!"+'>'
            Player2Wins += 1
    
        elif sum(Player1_cards) > 21 and sum(Player2_cards) < 21:
            print("Player1 busted")
            message = WinnerTopic + PlayerNames[1]+" won because " + PlayerNames[0]+ " busted"+'>'
            Player1Wins += 1
        
        elif sum(Player2_cards) > 21 and sum(Player1_cards) < 21:
            print("Player2 busted")
            message = WinnerTopic + PlayerNames[0]+" won because " + PlayerNames[1]+ " busted"+'>'
            Player2Wins += 1
    
        else:
            print("both players busted/couldnt find winner")
            message = WinnerTopic + "Both players busted"+'>'
        
        #sending winner message
        publisher.send_string(message)
        
    #sending total wins
    if(Player1Wins > Player2Wins):
        message = WinnerTopic + "!!!" +PlayerNames[0]+ " WON, while " +PlayerNames[1]+" had: "+str(Player2Wins)+" !!!"
    else:
        message = WinnerTopic + "!!!" +PlayerNames[1]+ " WON, while " +PlayerNames[0]+" had: "+str(Player1Wins)+" !!!"
    publisher.send_string(message)
