game:game.c
	gcc game.c mcts.c node.c referee.c -o game -g
clean:
	del game.exe