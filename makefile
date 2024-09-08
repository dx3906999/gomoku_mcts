game:game.c
	gcc game.c mcts.c node.c -o game
clean:
	del game.exe