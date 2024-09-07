game:game.c
	gcc game.c mcts.c node.c -o game
clean:
	rm game.exe