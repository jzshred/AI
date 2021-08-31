# Tic-Tac-Toe

# import modules
import random

# function draws game board
def drawBoard(board):
	print(board[1] + '|' + board[2] + '|' + board[3])
	print('-+-+-')
	print(board[4] + '|' + board[5] + '|' + board[6])
	print('-+-+-')
	print(board[7] + '|' + board[8] + '|' + board[9])

# function asks human player to choose his letter
def inputPlayerLetter():
	letter = ''
	while letter != 'X' and letter != 'O':
		print('Do you want to be X or O?')
		letter = input().upper()
	if letter == 'X':
		return ['X', 'O']
	else:
		return ['O', 'X']

# function randomly determines who goes first
def whoGoesFirst():
	if random.randint(0, 1) == 0:
		return 'computer'
	else:
		return 'player'

# function marks a move on the board
def makeMove(board, letter, move):
	board[move] = letter

# function checks for win
def isWinner(bo, le):
	return ((bo[7] == le and bo[8] == le and bo[9] == le) or
			(bo[4] == le and bo[5] == le and bo[6] == le) or
			(bo[1] == le and bo[2] == le and bo[3] == le) or
			(bo[7] == le and bo[4] == le and bo[1] == le) or
			(bo[8] == le and bo[5] == le and bo[2] == le) or
			(bo[9] == le and bo[6] == le and bo[3] == le) or
			(bo[7] == le and bo[5] == le and bo[3] == le) or
			(bo[9] == le and bo[5] == le and bo[1] == le))

# function copies the board
def getBoardCopy(board):
	boardCopy = []
	for i in board:
		boardCopy.append(i)
	return boardCopy

# function checks for free spaces
def isSpaceFree(board, move):
	return board[move] == ' '

# function asks human player for his move
def getPlayerMove(board):
	move = ' '
	while move not in '1 2 3 4 5 6 7 8 9'.split() or \
	not isSpaceFree(board, int(move)):
		print('What is your next move? (1-9)')
		move = input()
	return int(move)

# function creates possible moves list
def createPossibleMoves(board, movesList):
	possibleMoves = []
	for i in movesList:
		if isSpaceFree(board, i):
			possibleMoves.append(i)
	return possibleMoves

# function returns a valid move if there is one
def chooseRandomMoveFromList(board, movesList):
	possibleMoves = createPossibleMoves(board, movesList)
	if len(possibleMoves) != 0:
		return random.choice(possibleMoves)
	else:
		return None

# function defines game's difficulty
def getGameDifficulty():
	difficulty = 'X'
	while difficulty not in 'EMH':
		print('Enter difficulty: E - Easy, M - Medium, H - Hard')
		difficulty = input().upper()
	return difficulty

# function min value
def min_value(board, computerLetter, playerLetter, alpha, beta):
	bestScore = 100
	bestMove = None
	
	if isWinner(board, computerLetter):
		return (1, 0)
	elif isWinner(board, playerLetter):
		return (-1, 0)
	elif isBoardFull(board):
		return (0, 0)
	
	for i in range(1, 10):
		if board[i] == ' ':
			makeMove(board, playerLetter, i)
			(score, move) = max_value(board, computerLetter, playerLetter, alpha, beta)
			if score < bestScore:
				bestScore = score
				bestMove = i
			makeMove(board, ' ', i)
			if bestScore <= alpha:
				return(bestScore, bestMove)
			if bestScore < beta:
				beta = bestScore
	return (bestScore, bestMove)

# function max value
def max_value(board, computerLetter, playerLetter, alpha, beta):
	bestScore = -100
	bestMove = None

	if isWinner(board, computerLetter):
		return (1, 0)
	elif isWinner(board, playerLetter):
		return (-1, 0)
	elif isBoardFull(board):
		return (0, 0)
	
	for i in range(1, 10):
		if board[i] == ' ':
			makeMove(board, computerLetter, i)
			(score, move) = min_value(board, computerLetter, playerLetter, alpha, beta)
			if score > bestScore:
				bestScore = score
				bestMove = i
			makeMove(board, ' ', i)
			if bestScore >= beta:
				return(bestScore, bestMove)
			if bestScore > alpha:
				alpha = bestScore
	return (bestScore, bestMove)

# function gets computer's move
def getComputerMove(board, computerLetter, difficulty):
	if computerLetter == 'X':
		playerLetter = 'O'
	else:
		playerLetter = 'X'

	# easy difficulty CPU behavior - random move
	if difficulty == 'E':
		move = chooseRandomMoveFromList(board, [1, 2, 3, \
												4, 5, 6, \
												7, 8, 9 \
												])
		if move != None:
			return move

	# medium difficulty CPU behavior - 5 step check
	elif difficulty == 'M':
		# first, check if the computer can win with the next move
		for i in range(1, 10):
			boardCopy = getBoardCopy(board)
			if isSpaceFree(boardCopy, i):
				makeMove(boardCopy, computerLetter, i)
				if isWinner(boardCopy, computerLetter):
					return i

		# second, block human player if he could win on his next move
		for i in range(1, 10):
			boardCopy = getBoardCopy(board)
			if isSpaceFree(boardCopy, i):
				makeMove(boardCopy, playerLetter, i)
				if isWinner(boardCopy, playerLetter):
					return i

		# third option, try to take a corner if available
		move = chooseRandomMoveFromList(board, [1, 3, 7, 9])
		if move != None:
			return move

		# fourth option, try to take the center if available
		if isSpaceFree(board, 5):
			return 5

		# fifth option, try to take one of the sides if available
		return chooseRandomMoveFromList(board, [2, 4, 6, 8])

	# hard difficulty CPU behavior - minimax algorithm
	else:
		boardCopy = getBoardCopy(board)
		(bestScore, bestMove) = max_value(boardCopy, computerLetter, playerLetter, -100, 100)
		return bestMove
		

# function checks for a full board
def isBoardFull(board):
	for i in range(1, 10):
		if isSpaceFree(board, i):
			return False
	return True

# game loop
print('Welcome to Tic-Tac-Toe!')
while True:
	# choose game difficulty
	chosenDifficulty = getGameDifficulty()
	# reset the board
	theBoard = [' '] * 10
	playerLetter, computerLetter = inputPlayerLetter()
	if chosenDifficulty != 'H':
		turn = whoGoesFirst()
	else:
		turn = 'player'
	print('The ' + turn + ' will go first.')
	gameIsPlaying = True

	while gameIsPlaying:
		if turn == 'player':
			# human player's turn
			drawBoard(theBoard)
			move = getPlayerMove(theBoard)
			makeMove(theBoard, playerLetter, move)

			if isWinner(theBoard, playerLetter):
				drawBoard(theBoard)
				print('Hooray! You have won the game!')
				gameIsPlaying = False
			else:
				if isBoardFull(theBoard):
					drawBoard(theBoard)
					print('The game is a tie!')
					break
				else:
					turn = 'computer'
		else:
			# computer's turn
			move = getComputerMove(theBoard, computerLetter, chosenDifficulty)
			makeMove(theBoard, computerLetter, move)

			if isWinner(theBoard, computerLetter):
				drawBoard(theBoard)
				print('The computer has beaten you! You lose.')
				gameIsPlaying = False
			else:
				if isBoardFull(theBoard):
					drawBoard(theBoard)
					print('The game is a tie!')
					break
				else:
					turn = 'player'

	print('Do you want to play again? (yes or no)')
	if not input().lower().startswith('y'):
		break
